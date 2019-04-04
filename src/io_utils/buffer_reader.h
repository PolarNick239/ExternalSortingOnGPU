#pragma once

#include <cstddef>
#include <vector>


class BufferReader {
public:
	explicit BufferReader(const std::vector<char> &data);

	float next();
	bool isEmpty();

private:
	std::vector<char>	data;
	size_t				offset;
};
