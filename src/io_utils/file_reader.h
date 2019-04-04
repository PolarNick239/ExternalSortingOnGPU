#pragma once

#include "buffer_reader.h"

#include <fstream>


class FileReader {
public:
	explicit FileReader(const std::string &filename);
	~FileReader();

	size_t valuesNumber();
	bool isEmpty();
	float next();
	void seek(size_t index);
	std::string getFilename(); 

private:
	const std::string				filename;
	std::basic_fstream<char>		file;
	BufferReader					buffer;
	size_t							size;
	size_t							offset;
};
