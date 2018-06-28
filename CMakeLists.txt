CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#------------ project ------------
PROJECT("Benchmark" C)

#------------ variables ------------
SET(benchmark_src_dir "src")
SET(benchmark_header_dir "include")
SET(benchmark_project_name "benchmark")

#------ find files ------
FILE(GLOB_RECURSE benchmark_src_files ${benchmark_src_dir}/*)
FILE(GLOB_RECURSE benchmark_header_files ${benchmark_header_dir}/*)

#------------ create project ------------
INCLUDE_DIRECTORIES(${benchmark_header_dir})
ADD_EXECUTABLE(${benchmark_project_name} ${benchmark_src_files} ${benchmark_header_files})
SET_TARGET_PROPERTIES(${benchmark_project_name} PROPERTIES LINKER_LANGUAGE C)

#------ find OpenCL ------
FIND_PACKAGE("OpenCL")

IF(NOT ${OpenCL_FOUND})
    MESSAGE(FATAL_ERROR "Can't find OpenCL")
ENDIF()

INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${benchmark_project_name} ${OpenCL_LIBRARY})
