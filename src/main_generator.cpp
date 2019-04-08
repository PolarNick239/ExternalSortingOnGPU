#include <libutils/timer.h>
#include <libutils/fast_random.h>

#include <string>
#include <fstream>
#include <iostream>

#include "io_utils/file_reader.h"
#include "io_utils/file_writer.h"


int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <numberOfValuesToGenerate> <outputFilename>" << std::endl;
		return 1;
	}

	long long n = strtoll(argv[1], nullptr, 10);
	if (n <= 0) {
		std::cout << "Number of values to generate should be positive integer value, but " << argv[1] << " found!" << std::endl;
		return 1;
	}

	std::string outputFilename(argv[2]);
	std::cout << "Saving " << n << " random floats (" << (n * sizeof(float) / 1024 / 1024) << " MB) to file " << outputFilename << std::endl;

	timer t;

	{
		FastRandom r;
		FileWriter out(outputFilename);
		for (size_t i = 0; i < n; ++i) {
			float v = r.nextf();
			out.write(v);
		}
	}

	double dt = t.elapsed();

	std::cout << "Done in " << dt << " seconds";
	if (dt > 0.0) {
		std::cout << " (" << ((double) n * sizeof(float) / dt / 1024 / 1024) << " MB/s)";
	}
	std::cout << std::endl;

	return 0;
}
