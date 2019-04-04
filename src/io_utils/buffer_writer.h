#pragma once

#include <cstddef>
#include <vector>


class BufferWriter {
public:
	explicit BufferWriter(size_t size);

	void write(float value);
	bool isFull();

	char* ptr();
	size_t valuesNumber();
	void clear();

private:
	std::vector<char>	data;
	size_t				offset;
};
