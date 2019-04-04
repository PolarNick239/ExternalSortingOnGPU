#include "file_reader.h"
#include "../defines.h"

#include <cassert>
#include <stdexcept>
#include <algorithm>


FileReader::FileReader(const std::string &filename) :
		filename(filename),
		file(std::fstream(filename, std::ios::in | std::ios::binary | std::ios::ate)),
		buffer(BufferReader(std::vector<char>()))
{
	ptrdiff_t file_size = file.tellg();
	if (file_size < 0) {
		throw std::runtime_error("Failed to get size of file " + filename + "!");
	}
	file.seekg(0);

	size = (size_t) file_size;
	offset = 0;
	assert(size % sizeof(float) == 0);
}

FileReader::~FileReader()
{
	file.close();
}

size_t FileReader::valuesNumber()
{
	return size / sizeof(float);
}

bool FileReader::isEmpty()
{
	if (offset >= size) {
		assert(buffer.isEmpty() && file.tellg() == size);
		return true;
	} else {
		return false;
	}
}

float FileReader::next()
{
	if (isEmpty()) {
		throw std::runtime_error("Empty file!");
	}
	if (buffer.isEmpty()) {
		size_t buffer_size = std::min(BYTES_PER_BUFFER, size - offset);
		std::vector<char> data(buffer_size, 0);
		file.read(data.data(), buffer_size);
		buffer = BufferReader(data);
	}
	assert(!buffer.isEmpty());
	offset += sizeof(float);
	return buffer.next();
}

void FileReader::seek(size_t index)
{
	buffer = BufferReader(std::vector<char>());
	offset = index * sizeof(float);
	assert(offset < size);
	file.seekg(offset);
}

std::string FileReader::getFilename()
{
	return filename;
}
