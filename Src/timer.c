#include "timer.h"

static struct timeval tic_ts;

#ifdef _WIN32
LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime(int X, struct timeval *tv)
{
    LARGE_INTEGER           t;
    FILETIME            f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = (LONGLONG)microseconds;
    tv->tv_sec = (long)(t.QuadPart / 1000000);
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}
#endif

void tic (void)
{
#if defined(HAVE_MACH_ABSOLUTE_TIME)
    tic_ts = mach_absolute_time();
#else
    clock_gettime (0, &tic_ts);
#endif
}

double toc (void)
{
    double out;
#if defined(HAVE_MACH_ABSOLUTE_TIME)
    uint64_t ts, nanosec;
    static mach_timebase_info_data_t info = {0,0};
    if (info.denom == 0) {
        mach_timebase_info(&info);
    }
    ts = mach_absolute_time();
    nanosec = (ts - tic_ts) * (info.numer / info.denom);
    out = 1.0e-9 * nanosec;
#else
    struct timeval ts;
    clock_gettime (0, &ts);
    out = (ts.tv_usec - (double)tic_ts.tv_usec) * 1.0e-9;
    out += (ts.tv_sec - (double)tic_ts.tv_sec);
#endif

    return out;
}
