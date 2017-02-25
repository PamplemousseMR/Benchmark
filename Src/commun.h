#ifndef COMMUN_H
#define COMMUN_H

#ifdef __linux__
#define thr std::exception
#elif _WIN32
#define thr ...
#endif

#define A_PROB 0.57
#define B_PROB 0.19
#define C_PROB 0.19

#endif // COMMUN_H
