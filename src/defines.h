#pragma once

#define MAX_IN_MEMORY_VALUES (16*1024*1024)

#define PARTS_MERGED_PER_PASS 16

// BYTES_PER_BUFFER = 512 KB
#define BYTES_PER_BUFFER ((size_t) (128 * 1024 * sizeof(float)))

#define GPU_THREADS 1
