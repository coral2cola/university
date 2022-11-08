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

void print(char text[], float f1, float f2, float f3)
{
	std::cout << text << f1 << ", " << f2 << ", " << f3 << std::endl;
}

void print2(char text[], float f)
{
	std::cout << text << f << std::endl;
}

void error(char text[], float root, float f1, float f2, float f3)
{
	float tmp[3];
	tmp[0] = abs(root - f1);
	tmp[1] = abs(root - f2);
	tmp[2] = abs(root - f3);

	std::sort(tmp, tmp + 3);

	float e1 = tmp[0];
	float e2 = tmp[1];
	float e3 = tmp[2];

	float alpha = log(e3 / e2) / log(e2 / e1);
	float m = e3 / pow(e2, alpha);
	std::cout << "alpha of " << text << " is " << alpha << std::endl;
	//std::cout << "M     of " << text << " is " << m << std::endl;
}

// bessj0.c
float bessj0(float x)
{
	float ax, z;
	double xx, y, ans, ans1, ans2;

	if ((ax = fabs(x)) < 8.0) {
		y = x * x;
		ans1 = 57568490574.0 + y * (-13362590354.0 + y * (651619640.7
			+ y * (-11214424.18 + y * (77392.33017 + y * (-184.9052456)))));
		ans2 = 57568490411.0 + y * (1029532985.0 + y * (9494680.718
			+ y * (59272.64853 + y * (267.8532712 + y * 1.0))));
		ans = ans1 / ans2;
	}
	else {
		z = 8.0 / ax;
		y = z * z;
		xx = ax - 0.785398164;
		ans1 = 1.0 + y * (-0.1098628627e-2 + y * (0.2734510407e-4
			+ y * (-0.2073370639e-5 + y * 0.2093887211e-6)));
		ans2 = -0.1562499995e-1 + y * (0.1430488765e-3
			+ y * (-0.6911147651e-5 + y * (0.7621095161e-6
				- y * 0.934945152e-7)));
		ans = sqrt(0.636619772 / ax) * (cos(xx) * ans1 - z * sin(xx) * ans2);
	}
	return ans;
}

// bessj1.c
float bessj1(float x)
{
	float ax, z;
	double xx, y, ans, ans1, ans2;

	if ((ax = fabs(x)) < 8.0) {
		y = x * x;
		ans1 = x * (72362614232.0 + y * (-7895059235.0 + y * (242396853.1
			+ y * (-2972611.439 + y * (15704.48260 + y * (-30.16036606))))));
		ans2 = 144725228442.0 + y * (2300535178.0 + y * (18583304.74
			+ y * (99447.43394 + y * (376.9991397 + y * 1.0))));
		ans = ans1 / ans2;
	}
	else {
		z = 8.0 / ax;
		y = z * z;
		xx = ax - 2.356194491;
		ans1 = 1.0 + y * (0.183105e-2 + y * (-0.3516396496e-4
			+ y * (0.2457520174e-5 + y * (-0.240337019e-6))));
		ans2 = 0.04687499995 + y * (-0.2002690873e-3
			+ y * (0.8449199096e-5 + y * (-0.88228987e-6
				+ y * 0.105787412e-6)));
		ans = sqrt(0.636619772 / ax) * (cos(xx) * ans1 - z * sin(xx) * ans2);
		if (x < 0.0) ans = -ans;
	}
	return ans;
}

void bessj(float x, float* a, float* b)
{
	float ax, z;
	double xx, y, ans, ans1, ans2;

	if ((ax = fabs(x)) < 8.0) {
		y = x * x;
		ans1 = 57568490574.0 + y * (-13362590354.0 + y * (651619640.7
			+ y * (-11214424.18 + y * (77392.33017 + y * (-184.9052456)))));
		ans2 = 57568490411.0 + y * (1029532985.0 + y * (9494680.718
			+ y * (59272.64853 + y * (267.8532712 + y * 1.0))));
		ans = ans1 / ans2;
	}
	else {
		z = 8.0 / ax;
		y = z * z;
		xx = ax - 0.785398164;
		ans1 = 1.0 + y * (-0.1098628627e-2 + y * (0.2734510407e-4
			+ y * (-0.2073370639e-5 + y * 0.2093887211e-6)));
		ans2 = -0.1562499995e-1 + y * (0.1430488765e-3
			+ y * (-0.6911147651e-5 + y * (0.7621095161e-6
				- y * 0.934945152e-7)));
		ans = sqrt(0.636619772 / ax) * (cos(xx) * ans1 - z * sin(xx) * ans2);
	}

	*a = bessj0(x);
	*b = -bessj1(x);
}

// zbrak.c
void zbrak(float (*fx)(float), float x1, float x2, int n, float xb1[],
	float xb2[], int* nb)
{
	int nbb, i;
	float x, fp, fc, dx;

	nbb = 0;
	dx = (x2 - x1) / n;
	fp = (*fx)(x = x1);
	for (i = 1; i <= n; i++) {
		fc = (*fx)(x += dx);
		if (fc * fp <= 0.0) {
			xb1[++nbb] = x - dx;
			xb2[nbb] = x;
			if (*nb == nbb) return;

		}
		fp = fc;
	}
	*nb = nbb;
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
		if (fabs(dx) < xacc || fmid == 0.0) return rtb;
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
		if (fabs(del) < xacc || f == 0.0) return rtf;
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
		if (fabs(dx) < xacc) return rtn;
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
			if (xl == rts) return rts;
		}
		else {
			dxold = dx;
			dx = f / df;
			temp = rts;
			rts -= dx;
			//printf("At iteration no. %2d, x = %7.6f\n", j, rts);
			if (temp == rts) return rts;
		}
		if (fabs(dx) < xacc) return rts;
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
		if (fabs(dx) < xacc || f == 0.0) return rts;
	}
	nrerror("Maximum number of iterations exceeded in rtsec");
	return 0.0;
}
#undef MAXIT

float func1(float f)
{
	// 10e^(-x)*sin(2(pi)x)-2=0
	float tmp = 10 * exp(-f) * sin(2 * f * 3.14159265359) - 2;
	return tmp;
}

void dfunc1(float f, float* a, float* b)
{
	*a = 10 * exp(-f) * sin(2 * f * 3.14159265359) - 2;
	*b = -10 * exp(-f) * sin(2 * f * 3.14159265359) + 10 * exp(-f) * cos(2 * f * 3.14159265359) * 2 * 3.14159265359;
}

float func2(float f)
{
	// x^2-2xe^(-x)+e^(-2x)=0
	float tmp = pow(f, 2) - 2 * f * exp(-f) + exp(-2 * f);
	return tmp;
}

void dfunc2(float f, float* a, float* b)
{
	*a = pow(f, 2) - 2 * f * exp(-f) + exp(-2 * f);
	*b = 2 * f + 2 * f * exp(-f) - 2 * exp(-2 * f);
}

float func3(float f)
{
	// cos(x+sqrt(2))+x(x/2+sqrt(2))=0
	float tmp = cos(f + sqrt(2)) + f * (f / 2 + sqrt(2));
	return tmp;
}

void dfunc3(float f, float* a, float* b)
{
	*a = cos(f + sqrt(2)) + f * (f / 2 + sqrt(2));
	*b = -sin(f + sqrt(2)) + f + sqrt(2);
}

float func4(float f)
{
	// x+sin(x+sqrt(2))=0
	float tmp = f + sin(f + sqrt(2));
	return tmp;
}

void dfunc4(float f, float* a, float* b)
{
	*a = f + sin(f + sqrt(2));
	*b = 1 + cos(f + sqrt(2));
}