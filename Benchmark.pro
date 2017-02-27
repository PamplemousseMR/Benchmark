#=================================================
# Work with :
#   Windows MSVC and MINGW
#       NVIDIA 32/64
#       AMD 32/64
#   Unix gcc/mpicxx (AMD tester en 32)
#       NVIDIA (TODO 32)
#       AMD 32/64 (tester 32)
#=================================================

#----------Configuration----------
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
PLATFORM = NVIDIA
#----------------------------------

#---------OpenMPI Settings---------
message("-----Loading OpenMPI-----")
unix:{
    message("        Unix")
    QMAKE_CXX = mpic++
    QMAKE_CXX_RELEASE = $$QMAKE_CXX
    QMAKE_CXX_DEBUG = $$QMAKE_CXX
    QMAKE_LINK = $$QMAKE_CXX

    #QMAKE_CFLAGS += $$system(mpicc --showme:compile)
    #QMAKE_LFLAGS += $$system(mpic++ --showme:link)
    QMAKE_CXXFLAGS += -std=c++11

    #QMAKE_CXXFLAGS_DEBUG += $$system(mpic++ --showme:compile) -DMPICH_IGNORE_CXX_SEEK -DOMPI_BUILD_CXX_BINDINGS
    #QMAKE_CXXFLAGS_RELEASE += $$system(mpic++ --showme:compile) -DMPICH_IGNORE_CXX_SEEK -DOMPI_BUILD_CXX_BINDINGS
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
    QMAKE_LFLAGS +=  -fopenmp
    message("        Done")
}
win32-msvc*:{
    message("        Windows msvc")
    QMAKE_CXXFLAGS += -openmp
    message("        Done")
}
win32-g++:{
    message("        Windows mingw")
    QMAKE_CXXFLAGS+= -fopenmp
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
#DEBUG_EXCEPTION for activate exceptions
#DEBUG_OUTPUT for activate constructor/destructor message
#RANDOM for activate random seed

#PROG for main programme
Debug:DEFINES += DEBUG_EXCEPTION
DEFINES += PROG
#----------------------------------

#---------------Src----------------
SOURCES += Src/main.cpp \
    Src/node.cpp \
    Src/tree.cpp \
    Src/graph.cpp \
    Src/matrix.cpp
#----------------------------------

message("-------QMAKE END--------")

HEADERS += \
    Src/node.h \
    Src/tree.h \
    Src/graph.h \
    Src/matrix.h \
    Src/commun.h
