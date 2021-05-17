cmake_minimum_required(VERSION 3.0)

project(zipfgen)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Google test and benchmark
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/benchmark)

# the -I flag in gcc
include_directories(
  ${PROJECT_SOURCE_DIR}/include/
) 

set(LIB_SOURCES include/zipf/generator.h
                include/zipf/rejection_inversion_generator.h
                include/zipf/ycsb_generator.h
                src/rejection_inversion_generator.cpp
                src/ycsb_generator.cpp)
set(BENCH_SOURCES src/bench.cpp)
set(TEST_SOURCES src/test.cpp)

# shared library
set(LIBRARY_NAME zipf)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# creates ./build/bin/bench
add_executable(bench ${BENCH_SOURCES})
target_link_libraries(bench ${LIBRARY_NAME} benchmark::benchmark)

# creates ./build/bin/test
add_executable(test ${TEST_SOURCES})
target_link_libraries(test ${LIBRARY_NAME})

