#ifdef __CLION_IDE__
#include <libgpu/opencl/cl/clion_defines.cl>
#define WORKGROUP_SIZE 256
#endif

#line 7

__attribute__((reqd_work_group_size(WORKGROUP_SIZE, 1, 1)))
__kernel void merge(__global const float* as,
					__global       float* bs,
					        unsigned int  n,
					        unsigned int  sortedChunksSize)
{
	const unsigned int index = get_global_id(0);

	if (index >= n)
		return;

	unsigned int chunk1 = 2 * (index / (2 * sortedChunksSize));
	unsigned int chunk2 = chunk1 + 1;

	unsigned int chunkSize1 = (n > chunk1 * sortedChunksSize) ? min(sortedChunksSize, n - chunk1 * sortedChunksSize) : 0;
	unsigned int chunkSize2 = (n > chunk2 * sortedChunksSize) ? min(sortedChunksSize, n - chunk2 * sortedChunksSize) : 0;

	if (chunkSize2 == 0) {
		// If second chunk is empty - we should joust copy first chunk as is
		bs[index] = as[index];
		return;
	}
	// assert(chunkSize1 == sortedChunksSize);
	// assert(chunkSize2 <= sortedChunksSize);
	// If chunkSize2 < sortedChunkSize - we extend it with imaginary +infinity (FLT_MAX) values up to chunkSize1

	unsigned int diagonalIndex = index - chunk1 * sortedChunksSize;
	unsigned int diagonalSize;
	if (diagonalIndex <= chunkSize1) {
		diagonalSize = diagonalIndex + 1;
	} else {
		diagonalSize = diagonalIndex + 1 - (diagonalIndex - chunkSize1);
	}

	int l = -1;
	int r = diagonalSize;

	while (l < r - 1) {
		int m = (l + r) / 2;
		unsigned int index1 = m - 1;
		unsigned int index2 = diagonalIndex - 1 - index1;
		float value1 = (index1 == -1) ? -FLT_MAX : ((index1 >= chunkSize1) ? FLT_MAX : as[chunk1 * sortedChunksSize + index1]);
		float value2 = (index2 == -1) ? -FLT_MAX : ((index2 >= chunkSize2) ? FLT_MAX : as[chunk2 * sortedChunksSize + index2]);
		if (value2 >= value1) {
			l = m;
		} else {
			r = m;
		}
	}
	// assert(0 <= l);
	// assert(l < diagonalSize);

	unsigned int diagonalCrossingIndex = l;
	unsigned int index1 = diagonalCrossingIndex;
	unsigned int index2 = diagonalIndex - index1;
	float value1 = (index1 >= chunkSize1) ? FLT_MAX : as[chunk1 * sortedChunksSize + index1];
	float value2 = (index2 >= chunkSize2) ? FLT_MAX : as[chunk2 * sortedChunksSize + index2];

	bs[index] = min(value1, value2);
}
