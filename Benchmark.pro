#===========================================================================================================================================
# Work with :
#   Windows MSVC and MINGW
#       NVIDIA 32/64
#       AMD 32/64
#   Unix gcc/mpicxx (AMD tester en 32)
#       NVIDIA (TODO 32)
#       AMD 32/64 (tester 32)
#===========================================================================================================================================

#----------Configuration----------
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
PLATFORM = AMD
#----------------------------------

#---------OpenMPI Settings---------
message("-----Loading OpenMPI-----")
unix:{
    message("        Unix")
    QMAKE_CXX = mpic++
    QMAKE_CXX_RELEASE = $$QMAKE_CXX
    QMAKE_CXX_DEBUG = $$QMAKE_CXX
    QMAKE_LINK = $$QMAKE_CXX
    QMAKE_CXXFLAGS += -std=c++11
    message("        Done")
}
win32:{
    message("        Windows")
    INCLUDEPATH += $$PWD/Libs/OpenMPI_Windows/Include
    DEPENDPATH += $$PWD/Libs/OpenMPI_Windows/Include
    contains(QT_ARCH, i386) {
        message("        32-bit")
        LIBS += -L$$PWD/Libs/OpenMPI_Windows/Lib/Win32
        LIBS += -lcxx -lfmpich2 -lfmpich2g -lfmpich2s -lmpe -lmpi
    } else {
        message("        64-bit")
        LIBS += -L$$PWD/Libs/OpenMPI_Windows/Lib/x64
        LIBS += -lcxx -lfmpich2 -lfmpich2g -lirlog2rlog -lmpe -lmpi -lrlog
    }
    message("        Done")
}
#----------------------------------

#---------OpenMP Settings----------
message("-----Loading OpenMP-----")
unix:{
    message("        Unix")
    QMAKE_CXXFLAGS+= -fopenmp
    QMAKE_CFLAGS+= -fopenmp
    QMAKE_LFLAGS +=  -fopenmp
    message("        Done")
}
win32-msvc*:{
    message("        Windows msvc")
    QMAKE_CXXFLAGS += -openmp
    QMAKE_CFLAGS += -openmp
    message("        Done")
}
win32-g++:{
    message("        Windows mingw")
    QMAKE_CXXFLAGS+= -fopenmp
    QMAKE_CFLAGS+= -fopenmp
    QMAKE_LFLAGS +=  -fopenmp
    message("        Done")
}
#----------------------------------

#--------------OpenCL--------------
message("-----Loading OpenCL-----")
unix:{
    message("        Unix test")
    equals(PLATFORM, NVIDIA){
        message("        NVIDIA")
        INCLUDEPATH += $$PWD/Libs/OpenCL_Unix/NVIDIA/Include
        DEPENDPATH += $$PWD/Libs/OpenCL_Unix/NVIDIA/Include
        contains(QT_ARCH, i386) {
            error("        32-bit TODO nvidia unix")
        } else {
            message("        64-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Unix/NVIDIA/Lib/x86_64
            LIBS += -lOpenCL
        }
    } equals(PLATFORM, AMD) {
        message("        AMD")
        INCLUDEPATH += $$PWD/Libs/OpenCL_Unix/AMD/Include
        DEPENDPATH += $$PWD/Libs/OpenCL_Unix/AMD/Include
        contains(QT_ARCH, i386) {
            message("        32-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Unix/AMD/Lib/x86
            LIBS += -lamdocl32
            LIBS += -lamdocl12cl32
        } else {
            message("        64-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Unix/AMD/Lib/x86_64/sdk
            LIBS += -L$$PWD/Libs/OpenCL_Unix/AMD/Lib/x86_64
            LIBS += -lamdocl64
            LIBS += -lamdocl12cl64
        }
    }
    message("        Done")
}
win32:{
    message("        Windows msvc and mingw")
    equals(PLATFORM, NVIDIA){
        message("        NVIDIA")
        INCLUDEPATH += $$PWD/Libs/OpenCL_Windows/NVIDIA/Include
        DEPENDPATH += $$PWD/Libs/OpenCL_Windows/NVIDIA/Include
        contains(QT_ARCH, i386) {
            message("        32-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Windows/NVIDIA/Lib/Win32
        } else {
            message("        64-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Windows/NVIDIA/Lib/x64
            Debug:LIBS += -loclUtils64D
            Release:LIBS += -loclUtils64
        }
        LIBS += -lOpenCL
    } equals(PLATFORM, AMD) {
        message("        AMD")
        INCLUDEPATH += $$PWD/Libs/OpenCL_Windows/AMD/Include
        DEPENDPATH += $$PWD/Libs/OpenCL_Windows/AMD/Include
        contains(QT_ARCH, i386) {
            message("        32-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Windows/AMD/Lib/x86
            LIBS += -lamdocl
            LIBS += -lamdocl12cl
        } else {
            message("        64-bit")
            LIBS += -L$$PWD/Libs/OpenCL_Windows/AMD/Lib/x86_64
            LIBS += -lamdocl12cl64
            LIBS += -lamdocl64
        }
        LIBS += -lOpenCL
    }
    message("        Done")
}
#----------------------------------

#--------------Define--------------
#GRAPH_GENERATOR_OMP For activate edges generation with OpenMP
#GRAPH_GENERATOR_OCL For activate edges generation with OpenCL

#GRAPH_VERIFY_OMP For activate graph verification with OpenMP

#FAST_64BIT_ARITHMETIC Use 64-bit arithmetic when possible, else, 32 bits(more speed)

#OPENCL_GPU only GPU for OpenCL
#OPENCL_CPU only CPU for OpenCL
#OPENCL_ACCELERATOR only accelerator for OpenCL

#USE_MMAP_LARGE
#USE_MMAP_LARGE_EXT     A FAIRE FONCTIONNE

DEFINES += GRAPH_GENERATOR_OCL GRAPH_GENERATOR_OMP GRAPH_VERIFY_OMP
#----------------------------------

message("-------QMAKE END--------")

#---------------Src----------------

HEADERS += \
    Src/bfs/bfs.h \
    Src/verify/verify.h \
    Src/compat.h \
    Src/getopt.h \
    Src/mod_arith.h \
    Src/mod_arith_32bit.h \
    Src/mod_arith_64bit.h \
    Src/options.h \
    Src/splittable_mrg.h \
    Src/timer.h \
    Src/xalloc.h \
    Src/edge_struct.h \
    Src/generator/edge_generator.h \
    Src/mman_win.h \
    Src/generator/kronecker_generator.h \
    Src/opencl.h

SOURCES += \
    Src/bfs/omp-csr/omp-csr.c \
    Src/bfs/seq-csr/seq-csr.c \
    Src/bfs/seq-list/seq-list.c \
    Src/verify/verify.c \
    Src/getopt.c \
    Src/mrg_transitions.c \
    Src/options.c \
    Src/splittable_mrg.c \
    Src/timer.c \
    Src/xalloc.c \
    Src/benchmark.c \
    Src/generator/edge_generator.c \
    Src/generator/kronecker_generator_seq.c \
    Src/generator/kronecker_generator_ocl.c \
    Src/generator/kernel_kronecker.c \
    Src/mman_win.c
