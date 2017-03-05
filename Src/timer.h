#ifndef TIMER_HEADER_
#define TIMER_HEADER_

#include "compat.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void tic (void);
double toc (void);

#define TIME(timevar, what) do { tic (); what; timevar = toc(); } while (0)

#endif
