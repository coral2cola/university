// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"


#include "pch.h"
#include <math.h>


// rootfinding.cpp
void nrerror(char error_text[]);
void print(char text[], float f1, float f2);
float rtbis(float (*func)(float), float x1, float x2, float xacc);
float rtflsp(float (*func)(float), float x1, float x2, float xacc);
float rtnewt(void (*funcd)(float, float*, float*), float x1, float x2, float xacc);
float rtsafe(void (*funcd)(float, float*, float*), float x1, float x2, float xacc);
float rtsec(float (*func)(float), float x1, float x2, float xacc);
float muller(float (*func)(float), float f1, float f2, float f3, float xacc);
float func(float f);
float func0(float f);
void dfunc(float f, float* a, float* b);

#endif //PCH_H