#include "io_utils/file_reader.h"
#include "defines.h"
#include "io_utils/file_writer.h"

#include <memory>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <libutils/string_utils.h>
#include <libutils/timer.h>


std::string getFilename(const std::string &outputFilename, size_t pass, size_t part_index)
{
	return outputFilename + "." + to_string(pass) + "." + to_string(part_index) + ".tmp";
}


std::string toPercent(double part, double total)
{
	if (total == 0.0)
		return "0";
	return to_string((int) std::floor(100.0 * part / total + 0.5));
}


int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <inputFilename> <outputFilename>" << std::endl;
		return 1;
	}

	std::string inputFilename(argv[1]);
	std::string outputFilename(argv[2]);

	size_t n;
	{
		FileReader reader(inputFilename);
		n = reader.valuesNumber();
	}
	std::cout << "Values number: " << n << " (" << (n * sizeof(float) / 1024 / 1024) << " MB)" << std::endl;

	size_t pass = 0;
	size_t prevpass_nparts = 0;
	size_t prevpass_nvalues = 0;

	timer full_t;

	{
		size_t in_core_parts = (n + MAX_IN_MEMORY_VALUES - 1) / MAX_IN_MEMORY_VALUES; 
		std::cout << "Pass #" << pass << ": sorting part by part in core..." << std::endl;
		std::cout << "    In core parts number: " << in_core_parts << std::endl;
		std::cout << "    Limit for values in core: " << MAX_IN_MEMORY_VALUES << " (" << (MAX_IN_MEMORY_VALUES * sizeof(float) / 1024 / 1024) << " MB)" << std::endl;
		double reading_time = 0.0;
		double sorting_time = 0.0;
		double writing_time = 0.0;
		timer total_t;
		#pragma omp parallel reduction(+:reading_time,sorting_time,writing_time)
		{
			FileReader reader(inputFilename);
			std::vector<float> data(MAX_IN_MEMORY_VALUES, 0.0f);
			#pragma omp parallel for schedule(dynamic, 1)
			for (size_t part_index = 0; part_index < in_core_parts; ++part_index) {
				size_t from = part_index * MAX_IN_MEMORY_VALUES;
				size_t to = std::min(n, (part_index + 1) * MAX_IN_MEMORY_VALUES);

				timer reading_t;
				{
					reader.seek(from);
					for (size_t i = from; i < to; ++i) {
						data[i - from] = reader.next();
					}
				}
				reading_time += reading_t.elapsed();

				timer sorting_t;
				// TODO: implement sort on GPU
				std::sort(data.begin(), data.begin() + (to - from));
				sorting_time += sorting_t.elapsed();

				timer writing_t;
				{
					FileWriter writer(in_core_parts == 1 ? outputFilename : getFilename(outputFilename, pass, part_index));
					for (size_t i = 0; i < to - from; ++i) {
						writer.write(data[i]);
					}
				}
				writing_time += writing_t.elapsed();
			}
		}
		double sum_time = reading_time + sorting_time + writing_time;
		size_t total_values = 2 * n;
		std::cout << "    IO: " << (total_values / total_t.elapsed() / 1024 / 1024 * sizeof(float)) << " MB/s" << std::endl;
		std::cout << "    Finished in " << total_t.elapsed() << " s (" << toPercent(reading_time, sum_time) << "% reading + " << toPercent(sorting_time, sum_time) << "% sorting + " << toPercent(writing_time, sum_time) << "% writing)" << std::endl;
		prevpass_nparts = in_core_parts;
		prevpass_nvalues = MAX_IN_MEMORY_VALUES;
	}

	while (prevpass_nparts > 1) {
		size_t prevpass = pass;
		++pass;
		size_t merged_parts = (prevpass_nparts + PARTS_MERGED_PER_PASS - 1) / PARTS_MERGED_PER_PASS;
		size_t merged_nvalues = prevpass_nvalues * PARTS_MERGED_PER_PASS;
		std::cout << "Pass #" << pass << ": merging groups of " << PARTS_MERGED_PER_PASS << " parts..." << std::endl;
		std::cout << "    Input parts: " << prevpass_nparts << " with " << prevpass_nvalues << " values (" << (prevpass_nvalues * sizeof(float) / 1024 / 1024) << " MB) in each" << std::endl;

		timer total_t;
		#pragma omp parallel for schedule(dynamic, 1)
		for (size_t part_index = 0; part_index < merged_parts; ++part_index) {
			std::vector<std::unique_ptr<FileReader>> readers;
			for (size_t i = 0; i < std::min((size_t) PARTS_MERGED_PER_PASS, prevpass_nparts - PARTS_MERGED_PER_PASS * part_index); ++i) {
				size_t prevpass_part_index = PARTS_MERGED_PER_PASS * part_index + i;
				readers.emplace_back(new FileReader(getFilename(outputFilename, prevpass, prevpass_part_index)));
			}
			FileWriter writer(merged_parts == 1 ? outputFilename : getFilename(outputFilename, pass, part_index));
			const float NO_VALUE = std::numeric_limits<float>::max();
			float min_values[PARTS_MERGED_PER_PASS];
			bool is_empty[PARTS_MERGED_PER_PASS];
			for (size_t i = 0; i < readers.size(); ++i) {
				min_values[i] = readers[i]->next();
				is_empty[i] = false;
			}
			while (true) {
				float min = NO_VALUE;
				ptrdiff_t min_reader = -1;
				for (size_t i = 0; i < readers.size(); ++i) {
					if (is_empty[i]) continue;
					if (min_reader == -1 || min_values[i] < min) {
						min = min_values[i];
						min_reader = i;
					}
				}
				if (min_reader == -1) {
					// i.e. all readers are empty
					for (size_t i = 0; i < readers.size(); ++i) {
						assert(min_values[i] == NO_VALUE);
						assert(is_empty[i]);
						assert(readers[i]->isEmpty());
					}
					break;
				}
				assert(min_values[min_reader] == min);
				if (readers[min_reader]->isEmpty()) {
					is_empty[min_reader] = true;
					min_values[min_reader] = NO_VALUE;
				} else {
					min_values[min_reader] = readers[min_reader]->next();
				}
				writer.write(min);
			}
			for (size_t i = 0; i < readers.size(); ++i) {
				std::remove(readers[i]->getFilename().c_str());
			}
		}
		size_t total_values = 2 * n;
		std::cout << "    IO: " << (total_values / total_t.elapsed() / 1024 / 1024 * sizeof(float)) << " MB/s" << std::endl;
		std::cout << "    Finished in " << total_t.elapsed() << " s" << std::endl;

		prevpass_nparts = merged_parts;
		prevpass_nvalues = merged_nvalues;
	}

	std::cout << "Finished in " << full_t.elapsed() << " s" << std::endl;

	return 0;
}
