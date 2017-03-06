#ifndef __OPTIONS__
#define __OPTIONS__

#include "compat.h"
#include <stdio.h>		/*	printf	*/
#include <stdlib.h>		/*	EXIT_SUCCESS	*/
#include <errno.h>		/*	errno	*/
#include <inttypes.h>	/*	int64_t	*/
#include <string.h>		/*	strdup	*/

#ifdef _WIN32
#include <windows.h>
#include "getopt.h"		/*	getopt	*/
#else
#include <unistd.h>		/*	getopt	*/
#endif


#define NAME "Benchmark"
#define VERSION 1.0

#define A_PARAM 0.57
#define B_PARAM 0.19
#define C_PARAM 0.19

#define NBFS_max 64

#define default_SCALE ((int64_t)14)
#define default_edgefactor ((int64_t)16)

extern int VERBOSE;
extern char *dumpname;
extern char *rootname;


extern double A, B, C, D;

extern int NBFS;

extern int64_t SCALE;
extern int64_t edgefactor;

void get_options (int, char**);

#endif
