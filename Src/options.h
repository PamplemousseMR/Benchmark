#ifndef OPTIONS_HEADER_
#define OPTIONS_HEADER_

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <windows.h>

#include "getopt.h"

#define NAME "Benchmark"
#define VERSION 0

extern int VERBOSE;
extern int use_RMAT;
extern char *dumpname;
extern char *rootname;

#define A_PARAM 0.57
#define B_PARAM 0.19
#define C_PARAM 0.19

extern double A, B, C, D;

#define NBFS_max 64
extern int NBFS;

#define default_SCALE ((int64_t)14)
#define default_edgefactor ((int64_t)16)

extern int64_t SCALE;
extern int64_t edgefactor;

void get_options (int, char**);

#endif /*OPTIONS_HEADER_*/
