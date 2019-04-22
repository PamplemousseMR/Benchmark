#========================================
#	project
#========================================
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("Benchmark" C)

#================================================================================
#		init
#================================================================================

#========================================
#	variables
#========================================
SET(benchmark_src_dir "src")
SET(benchmark_header_dir "include")
SET(benchmark_project_name "benchmark")

#========================================
#	compiler specifications
#========================================
SET(CMAKE_C_STANDARD 99)

#========================================
#	find files
#========================================
FILE(GLOB_RECURSE benchmark_src_files ${benchmark_src_dir}/*)
FILE(GLOB_RECURSE benchmark_header_files ${benchmark_header_dir}/*)

#================================================================================
#		create
#================================================================================

#========================================
#	create project
#========================================
ADD_EXECUTABLE(${benchmark_project_name} ${benchmark_src_files} ${benchmark_header_files})
INCLUDE_DIRECTORIES(${benchmark_header_dir})

#========================================
#	find OpenCL
#========================================
FIND_PACKAGE("OpenCL")

IF(NOT ${OpenCL_FOUND})
    MESSAGE(FATAL_ERROR "Can't find OpenCL")
ENDIF()

INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${benchmark_project_name} ${OpenCL_LIBRARY})

#========================================
#	find OpenMP
#========================================
FIND_PACKAGE("OpenMP")

IF(NOT ${OPENMP_FOUND})
    MESSAGE(FATAL_ERROR "Can't find OpenMP")
ENDIF()

IF(UNIX)
    TARGET_LINK_LIBRARIES(${benchmark_project_name} PRIVATE ${OpenMP_C_FLAGS})
ENDIF(UNIX)
TARGET_COMPILE_OPTIONS(${benchmark_project_name} PRIVATE ${OpenMP_C_FLAGS})

#========================================
#	find math
#========================================
IF(UNIX)
  TARGET_LINK_LIBRARIES(${benchmark_project_name} m)
ENDIF(UNIX)
