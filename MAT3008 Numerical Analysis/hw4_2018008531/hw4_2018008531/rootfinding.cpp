#include "pch.h"
#include <math.h>
#include <iostream>
#include <cmath>
#include <algorithm>

// nrutil.c
void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
	fprintf(stderr, "Numerical Recipes run-time error...\n");
	fprintf(stderr, "%s\n", error_text);
	fprintf(stderr, "...now exiting to system...\n");
	exit(1);
}

void print(char text[], float f1, float f2)
{
	std::cout << "** " << text << f1 << ", " << f2 << " **" << std::endl;
}

// rtbis.c
#define JMAX 40

float rtbis(float (*func)(float), float x1, float x2, float xacc)
{
	//void nrerror(char error_text[]);
	int j;
	int cnt = 0;
	float dx, f, fmid, xmid, rtb;

	f = (*func)(x1);
	fmid = (*func)(x2);
	if (f * fmid >= 0.0) nrerror("Root must be bracketed for bisection in rtbis");
	rtb = f < 0.0 ? (dx = x2 - x1, x1) : (dx = x1 - x2, x2);
	for (j = 1; j <= JMAX; j++) {
		fmid = (*func)(xmid = rtb + (dx *= 0.5));
		if (fmid <= 0.0) rtb = xmid;
		//printf("At iteration no. %2d, x = %7.6f\n", j, rtb);
		if (fabs(dx) < xacc || fmid == 0.0)
		{
			printf("number of iterations : %2d\n", j);
			return rtb;
		}
	}
	nrerror("Too many bisections in rtbis");
	return 0.0;
}
#undef JMAX

// rtflsp.c
#define MAXIT 30

float rtflsp(float (*func)(float), float x1, float x2, float xacc)
{
	//void nrerror(char error_text[]);
	int j;
	float fl, fh, xl, xh, swap, dx, del, f, rtf;

	fl = (*func)(x1);
	fh = (*func)(x2);
	if (fl * fh > 0.0) nrerror("Root must be bracketed in rtflsp");
	if (fl < 0.0) {
		xl = x1;
		xh = x2;
	}
	else {
		xl = x2;
		xh = x1;
		swap = fl;
		fl = fh;
		fh = swap;
	}
	dx = xh - xl;
	for (j = 1; j <= MAXIT; j++) {
		rtf = xl + dx * fl / (fl - fh);
		f = (*func)(rtf);
		if (f < 0.0) {
			del = xl - rtf;
			xl = rtf;
			fl = f;
		}
		else {
			del = xh - rtf;
			xh = rtf;
			fh = f;
		}
		dx = xh - xl;
		//printf("At iteration no. %2d, x = %7.6f\n", j, rtf);
		if (fabs(del) < xacc || f == 0.0)
		{
			printf("number of iterations : %2d\n", j);
			return rtf;
		}
	}
	nrerror("Maximum number of iterations exceeded in rtflsp");
	return 0.0;
}
#undef MAXIT

// rtnewt.c
#define JMAX 20


float rtnewt(void (*funcd)(float, float*, float*), float x1, float x2,
	float xacc)
{
	//void nrerror(char error_text[]);
	int j;
	float df, dx, f, rtn;

	rtn = 0.5 * (x1 + x2);
	for (j = 1; j <= JMAX; j++) {
		(*funcd)(rtn, &f, &df);
		dx = f / df;
		rtn -= dx;
		if ((x1 - rtn) * (rtn - x2) < 0.0)
			nrerror("Jumped out of brackets in rtnewt");
		//printf("At iteration no. %2d, x = %7.6f\n", j, rtn);
		if (fabs(dx) < xacc)
		{
			printf("number of iterations : %2d\n", j);
			return rtn;
		}
	}
	nrerror("Maximum number of iterations exceeded in rtnewt");
	return 0.0;
}


#undef JMAX

// rtsafe.c
#define MAXIT 100

float rtsafe(void (*funcd)(float, float*, float*), float x1, float x2,
	float xacc)
{
	//void nrerror(char error_text[]);
	int j;
	float df, dx, dxold, f, fh, fl;
	float temp, xh, xl, rts;

	(*funcd)(x1, &fl, &df);
	(*funcd)(x2, &fh, &df);
	if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
		nrerror("Root must be bracketed in rtsafe");
	if (fl == 0.0) return x1;
	if (fh == 0.0) return x2;
	if (fl < 0.0) {
		xl = x1;
		xh = x2;
	}
	else {
		xh = x1;
		xl = x2;
	}
	rts = 0.5 * (x1 + x2);
	dxold = fabs(x2 - x1);
	dx = dxold;
	(*funcd)(rts, &f, &df);
	for (j = 1; j <= MAXIT; j++) {
		if ((((rts - xh) * df - f) * ((rts - xl) * df - f) > 0.0)
			|| (fabs(2.0 * f) > fabs(dxold * df))) {
			dxold = dx;
			dx = 0.5 * (xh - xl);
			rts = xl + dx;
			//printf("At iteration no. %2d, x = %7.6f\n", j, rts);
			if (xl == rts)
			{
				printf("number of iterations : %2d\n", j);
				return rts;
			}
		}
		else {
			dxold = dx;
			dx = f / df;
			temp = rts;
			rts -= dx;
			//printf("At iteration no. %2d, x = %7.6f\n", j, rts);
			if (temp == rts)
			{
				printf("number of iterations : %2d\n", j);
				return rts;
			}
		}
		if (fabs(dx) < xacc)
		{
			printf("number of iterations : %2d\n", j);
			return rts;
		}
		(*funcd)(rts, &f, &df);
		if (f < 0.0)
			xl = rts;
		else
			xh = rts;
	}
	nrerror("Maximum number of iterations exceeded in rtsafe");
	return 0.0;
}
#undef MAXIT

// rtsec.c
#define MAXIT 30

float rtsec(float (*func)(float), float x1, float x2, float xacc)
{
	//void nrerror(char error_text[]);
	int j;
	float fl, f, dx, swap, xl, rts;

	fl = (*func)(x1);
	f = (*func)(x2);
	if (fabs(fl) < fabs(f)) {
		rts = x1;
		xl = x2;
		swap = fl;
		fl = f;
		f = swap;
	}
	else {
		xl = x1;
		rts = x2;
	}
	for (j = 1; j <= MAXIT; j++) {
		dx = (xl - rts) * f / (f - fl);
		xl = rts;
		fl = f;
		rts += dx;
		f = (*func)(rts);
		//printf("At iteration no. %2d, x = %7.6f\n", j, rts);
		if (fabs(dx) < xacc || f == 0.0)
		{
			printf("number of iterations : %2d\n", j);
			return rts;
		}
	}
	nrerror("Maximum number of iterations exceeded in rtsec");
	return 0.0;
}
#undef MAXIT

#define I 2
float muller(float (*func)(float), float f1, float f2, float f3, float xacc)
{
	int i, itr, maxmitr;
	float x[4], li, di, mu, s, l, allerr;
	float (*f)(float x);
	f = func;

	x[0] = f1;
	x[1] = f2;
	x[2] = f3;

	allerr = xacc;
	maxmitr = 20;
	for (itr = 1; itr <= maxmitr; itr++)
	{
		li = (x[I] - x[I - 1]) / (x[I - 1] - x[I - 2]);
		di = (x[I] - x[I - 2]) / (x[I - 1] - x[I - 2]);
		mu = f(x[I - 2]) * li * li - f(x[I - 1]) * di * di + f(x[I]) * (di + li);
		s = sqrt((mu * mu - 4 * f(x[I]) * di * li * (f(x[I - 2]) * li - f(x[I - 1]) * di + f(x[I]))));
		if (mu < 0)
			l = (2 * f(x[I]) * di) / (-mu + s);
		else
			l = (2 * f(x[I]) * di) / (-mu - s);
		x[I + 1] = x[I] + l * (x[I] - x[I - 1]);
		//printf("At iteration no. %2d, x = %7.6f\n", itr, x[I + 1]);
		if (fabs(x[I + 1] - x[I]) < allerr)
		{
			//printf("After %3d iterations, the required root is %6.4f\n", itr, x[I + 1]);
			printf("number of iterations : %2d\n", itr);
			return x[I + 1];
		}
		for (i = I - 2; i < 3; i++)
			x[i] = x[i + 1];
	}
	printf("The required solution does not converge or iterations are insufficient\n");
	return 1;
}
#undef I

float func(float r)
{
	float tmp = exp(-0.005 * r) * cos(sqrt(2000 - (0.01 * r * r)) * 0.05) - 0.01;
	return tmp;
}

float func0(float r)
{
	float tmp1 = (r * exp(-r / 200) * sin(sqrt(2000 - (r * r / 100)) / 20)) / (2000 * sqrt(2000 - (r * r / 100)));
	float tmp2 = (exp(-r / 200) * cos(sqrt(2000 - (r * r / 100)) / 20)) / 200;
	return tmp1 - tmp2;
}

void dfunc(float r, float* a, float* b)
{
	*a = exp(-0.005 * r) * cos(sqrt(2000 - (0.01 * r * r)) * 0.05) - 0.01;
	*b = func0(r);
}