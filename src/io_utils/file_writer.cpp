#include "file_writer.h"
#include "../defines.h"

#include <cassert>


FileWriter::FileWriter(const std::string &filename)
		: filename(filename),
		  file(std::fstream(filename, std::ios::out | std::ios::binary)),
		  buffer(BYTES_PER_BUFFER)
{
	
}

FileWriter::~FileWriter()
{
	flushBuffer();
	file.close();
}

size_t FileWriter::valuesNumber()
{
	return offset / sizeof(float);
}

void FileWriter::write(float value)
{
	if (buffer.isFull()) {
		flushBuffer();
	}
	assert(!buffer.isFull());
	buffer.write(value);
}

void FileWriter::flushBuffer()
{
	size_t values_number = buffer.valuesNumber();
	if (values_number > 0) {
		file.write(buffer.ptr(), values_number * sizeof(float));
		buffer.clear();
	}
}
