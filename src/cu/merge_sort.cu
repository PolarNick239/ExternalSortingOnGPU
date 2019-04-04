#include <libgpu/cuda/cu/opencl_translator.cu>

#include "../cl/merge_sort.cl"

void cuda_merge_sort(const gpu::WorkSize &workSize, cudaStream_t stream,
					 const float* as, float* bs, unsigned int n, unsigned int sorted_chunks_size) {
	merge_sort<<<workSize.cuGridSize(), workSize.cuBlockSize()>>>(as, bs, n, sorted_chunks_size);
	CUDA_CHECK_KERNEL(stream);
}
