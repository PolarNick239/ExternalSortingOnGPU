#ifdef __CLION_IDE__
#include <libgpu/opencl/cl/clion_defines.cl>
#define WORKGROUP_SIZE 256
#endif

#line 6

__attribute__((reqd_work_group_size(WORKGROUP_SIZE, 1, 1)))
__kernel void merge_sort(__global const float* as,
						 __global       float* bs,
						 unsigned int n,
						 unsigned int sorted_chunks_size)
{
	const unsigned int global_index = get_global_id(0);
	const unsigned int local_index = get_local_id(0);

	__local float as_local[WORKGROUP_SIZE];
	__local float bs_local[WORKGROUP_SIZE];

	if (2 * sorted_chunks_size <= WORKGROUP_SIZE) {
		as_local[local_index] = (global_index < n) ? as[global_index] : FLT_MAX;
		barrier(CLK_LOCAL_MEM_FENCE);
		if (local_index % (2 * sorted_chunks_size) == 0) {
			// Merging two sorted chunks in new one
			int i = 0;
			int j = 0;
			for (int _ = 0; _ < 2 * sorted_chunks_size; ++_) {
				
			}
		}
	}
}
