#ifndef TIMER_HEADER_
#define TIMER_HEADER_

#include "compat.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <windows.h>
#include <time.h>

void tic (void);
double toc (void);

#define TIME(timevar, what) do { tic (); what; timevar = toc(); } while (0)

#endif /* TIMER_HEADER_ */
