cmake_minimum_required(VERSION 3.1)

project(external_sort)

set(CMAKE_CXX_STANDARD 11)

option(GPU_CUDA_SUPPORT "CUDA support." OFF)

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
    message(WARNING "OpenMP not found!")
endif()

add_subdirectory(libs)

convertIntoHeader(src/cl/merge_sort.cl src/cl/merge_sort_cl.h merge_sort_kernel)

set(SOURCES
        src/cl/merge_sort_cl.h
        src/io_utils/buffer_reader.cpp
        src/io_utils/buffer_reader.h
        src/io_utils/buffer_writer.cpp
        src/io_utils/buffer_writer.h
        src/io_utils/file_reader.cpp
        src/io_utils/file_reader.h
        src/io_utils/file_writer.cpp
        src/io_utils/file_writer.h
)

add_executable(input_generator src/main_generator.cpp ${SOURCES})
target_link_libraries(input_generator libclew libgpu libutils)

if (GPU_CUDA_SUPPORT)
    find_package(CUDA REQUIRED)
    add_definitions(-DCUDA_SUPPORT)
    set(SOURCES ${SOURCES} src/cu/merge_sort.cu)
    cuda_add_executable(${PROJECT_NAME} src/main_sorter.cpp ${SOURCES})
else()
    add_executable(${PROJECT_NAME} src/main_sorter.cpp ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} libclew libgpu libutils)
