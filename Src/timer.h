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

#define TIME(timevar, what) do { Timer::tic (); what; timevar = Timer::toc(); } while (0)

class Timer
{

	private:

		#ifdef _WIN32
		static struct timeval tic_ts;
		#else
		static struct timespec tic_ts;
		#endif

	public:

		static void tic();
		static double toc();

};

#endif /* TIMER_HEADER_ */
