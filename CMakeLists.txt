#=======================================
#                PROJECT
#=======================================

cmake_minimum_required(VERSION 3.0)
project("Benchmark")

#=======================================
#               LIBRARIES
#=======================================

find_package("OpenCL" REQUIRED)
find_package("OpenMP" REQUIRED)

#=======================================
#              EXECUTABLE
#=======================================

set(benchmark_src
    benchmark/benchmark.c
    benchmark/bfs/omp-csr/omp-csr.c
    benchmark/bfs/seq-csr/seq-csr.c
    benchmark/bfs/seq-list/seq-list.c
    benchmark/generator/edge_generator.c
    benchmark/generator/kernel_kronecker.c
    benchmark/generator/kronecker_generator.c
    benchmark/getopt.c
    benchmark/mman_win.c
    benchmark/opencl.c
    benchmark/options.c
    benchmark/splittable_mrg.c
    benchmark/timer.c
    benchmark/verify/verify.c
    benchmark/xalloc.c)

set(benchmark_header
    benchmark/bfs/bfs.h
    benchmark/compat.h
    benchmark/edge_struct.h
    benchmark/generator/edge_generator.h
    benchmark/generator/kernel_kronecker.h
    benchmark/generator/kronecker_generator.h
    benchmark/getopt.h
    benchmark/mman_win.h
    benchmark/mod_arith.h
    benchmark/mod_arith_32bit.h
    benchmark/mod_arith_64bit.h
    benchmark/opencl.h
    benchmark/options.h
    benchmark/splittable_mrg.h
    benchmark/timer.h
    benchmark/verify/verify.h
    benchmark/xalloc.h)

add_executable(benchmark ${benchmark_src} ${benchmark_header})
target_include_directories(benchmark PRIVATE "benchmark")
set_target_properties(benchmark PROPERTIES C_STANDARD 99)
target_link_libraries(benchmark PRIVATE OpenCL::OpenCL OpenMP::OpenMP_CXX)

if(UNIX)
  target_link_libraries(benchmark PRIVATE m)
endif(UNIX)
