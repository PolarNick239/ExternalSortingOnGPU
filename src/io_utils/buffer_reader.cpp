#include "buffer_reader.h"

#include <cassert>
#include <stdexcept>


BufferReader::BufferReader(const std::vector<char> &data) : data(data), offset(0)
{
	
}

bool BufferReader::isEmpty()
{
	if (offset >= data.size()) {
		assert(offset == data.size());
		return true;
	} else {
		return false;
	}
}

float BufferReader::next()
{
	if (isEmpty()) {
		throw std::runtime_error("Empty buffer!");
	}
	float value = *((float*) (data.data() + offset));
	offset += sizeof(float);
	return value;
}
