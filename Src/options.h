#ifndef OPTIONS_HEADER_
#define OPTIONS_HEADER_

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#ifdef _WIN32
	#include <windows.h>
	#include "getopt.h"
#else
	#include <unistd.h>
	#include <getopt.h>
#endif

#include "BFS/BFS.h"

#define A_PARAM 0.57
#define B_PARAM 0.19
#define C_PARAM 0.19
#define NBFS_max 64
#define default_SCALE ((int64_t)14)
#define default_edgefactor ((int64_t)16)

class Options
{

	private:

		static bool VERBOSE;
		static bool use_RMAT;
		static char* dumpname;
		static char* rootname;
		static double A, B, C, D;
		static int NBFS;
		static int64_t SCALE;
		static int64_t edgefactor;

	public:

		static void getOptions(int, char**);
		static bool isVerbose();
		static bool isUseMat();
		static char* getDumpname();
		static char* getRootname();
		static double getA();
		static double getB();
		static double getC();
		static double getD();
		static int getNbsf();
		static void setNbfs(int);
		static int64_t getScale();
		static int64_t getEdgefactor();

};

#endif /*OPTIONS_HEADER_*/
