#pragma once

#include "buffer_writer.h"

#include <fstream>


class FileWriter {
public:
	explicit FileWriter(const std::string &filename);
	~FileWriter();

	size_t valuesNumber();
	void write(float value);

private:
	void flushBuffer();

	const std::string				filename;
	std::basic_fstream<char>		file;
	BufferWriter					buffer;
	size_t							offset;
};
