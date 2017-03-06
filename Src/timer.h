#ifndef __TIMER__
#define __TIMER__

#include "compat.h"
#include <time.h>	/*	timespec	*/

#ifdef _WIN32
#include <windows.h>	/*	timeval	*/
#endif

void tic (void);
double toc (void);

#define TIME(timevar, what) do { tic (); what; timevar = toc(); } while (0)

#endif
