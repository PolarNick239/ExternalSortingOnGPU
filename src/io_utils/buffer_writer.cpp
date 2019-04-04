#include "buffer_writer.h"

#include <cassert>
#include <string.h>


BufferWriter::BufferWriter(size_t size) : data(std::vector<char>(size, 0)), offset(0)
{
	
}

void BufferWriter::write(float value)
{
	assert(!isFull());
	*((float*) (data.data() + offset)) = value;
	offset += sizeof(float);
}

bool BufferWriter::isFull()
{
	if (offset >= data.size()) {
		assert(offset == data.size());
		return true;
	} else {
		return false;
	}
}

char* BufferWriter::ptr()
{
	return data.data();
}


size_t BufferWriter::valuesNumber()
{
	return offset / sizeof(float);
}

void BufferWriter::clear()
{
	memset(data.data(), 0, data.size());
	offset = 0;
}
