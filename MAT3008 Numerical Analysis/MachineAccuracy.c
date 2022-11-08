#include <stdio.h>
#include <math.h>

#define CONV(i) ((float)(i))

void machar(int* ibeta, int* it, int* irnd, int* ngrd, int* machep, int* negep,
	int* iexp, int* minexp, int* maxexp, float* eps, float* epsneg,
	float* xmin, float* xmax)
{
	int i, itemp, iz, j, k, mx, nxres;
	float a, b, beta, betah, betain, one, t, temp, temp1, tempa, two, y, z, zero;

	one = CONV(1);
	two = one + one;
	zero = one - one;
	a = one;
	do {
		a += a; // 2
		temp = a + one; // 3
		temp1 = temp - a; // 1
	} while (temp1 - one == zero);
	b = one;
	do {
		b += b; // 2
		temp = a + b; // 4
		itemp = (int)(temp - a); // 1
	} while (itemp == 0);
	*ibeta = itemp;
	beta = CONV(*ibeta);
	*it = 0;
	b = one;
	do {
		++(*it);
		b *= beta;
		temp = b + one;
		temp1 = temp - b;
	} while (temp1 - one == zero);
	*irnd = 0;
	betah = beta / two;
	temp = a + betah;
	if (temp - a != zero) *irnd = 1;
	tempa = a + beta;
	temp = tempa + betah;
	if (*irnd == 0 && temp - tempa != zero) *irnd = 2;
	*negep = (*it) + 3;
	betain = one / beta;
	a = one;
	for (i = 1; i <= (*negep); i++) a *= betain;
	b = a;
	for (;;) {
		temp = one - a;
		if (temp - one != zero) break;
		a *= beta;
		--(*negep);
	}
	*negep = -(*negep);
	*epsneg = a;
	*machep = -(*it) - 3;
	a = b;
	for (;;) {
		temp = one + a;
		if (temp - one != zero) break;
		a *= beta;
		++(*machep);
	}
	*eps = a;
	*ngrd = 0;
	temp = one + (*eps);
	if (*irnd == 0 && temp * one - one != zero) *ngrd = 1;
	i = 0;
	k = 1;
	z = betain;
	t = one + (*eps);
	nxres = 0;
	for (;;) {
		y = z;
		z = y * y;
		a = z * one;
		temp = z * t;
		if (a + a == zero || fabs(z) >= y) break;
		temp1 = temp * betain;
		if (temp1 * beta == z) break;
		++i;
		k += k;
	}
	if (*ibeta != 10) {
		*iexp = i + 1;
		mx = k + k;
	}
	else {
		*iexp = 2;
		iz = (*ibeta);
		while (k >= iz) {
			iz *= *ibeta;
			++(*iexp);
		}
		mx = iz + iz - 1;
	}
	for (;;) {
		*xmin = y;
		y *= betain;
		a = y * one;
		temp = y * t;
		if (a + a != zero && fabs(y) < *xmin) {
			++k;
			temp1 = temp * betain;
			if (temp1 * beta == y && temp != y) {
				nxres = 3;
				*xmin = y;
				break;
			}
		}
		else break;
	}
	*minexp = -k;
	if (mx <= k + k - 3 && *ibeta != 10) {
		mx += mx;
		++(*iexp);
	}
	*maxexp = mx + (*minexp);
	*irnd += nxres;
	if (*irnd >= 2) *maxexp -= 2;
	i = (*maxexp) + (*minexp);
	if (*ibeta == 2 && !i) --(*maxexp);
	if (i > 20) --(*maxexp);
	if (a != y) *maxexp -= 2;
	*xmax = one - (*epsneg);
	if ((*xmax) * one != *xmax) *xmax = one - beta * (*epsneg);
	*xmax /= (*xmin * beta * beta * beta);
	i = (*maxexp) + (*minexp) + 3;
	for (j = 1; j <= i; j++) {
		if (*ibeta == 2) *xmax += *xmax;
		else *xmax *= beta;
	}

	//***** modified *****
	printf("The machine accuracy of FLOAT is %e\n", *eps);
}
#undef CONV

void machar2(int* ibeta, int* it, int* irnd, int* ngrd, int* machep, int* negep,
	int* iexp, int* minexp, int* maxexp, double* eps, double* epsneg,
	double* xmin, double* xmax)
{
	int i, itemp, iz, j, k, mx, nxres;
	double a, b, beta, betah, betain, one, t, temp, temp1, tempa, two, y, z, zero;

	one = (double)(1);
	two = one + one;
	zero = one - one;
	a = one;
	do {
		a += a; // 2
		temp = a + one; // 3
		temp1 = temp - a; // 1
	} while (temp1 - one == zero);
	b = one;
	do {
		b += b; // 2
		temp = a + b; // 4
		itemp = (int)(temp - a); // 1
	} while (itemp == 0);
	*ibeta = itemp;
	beta = (double)(*ibeta);
	*it = 0;
	b = one;
	do {
		++(*it);
		b *= beta;
		temp = b + one;
		temp1 = temp - b;
	} while (temp1 - one == zero);
	*irnd = 0;
	betah = beta / two;
	temp = a + betah;
	if (temp - a != zero) *irnd = 1;
	tempa = a + beta;
	temp = tempa + betah;
	if (*irnd == 0 && temp - tempa != zero) *irnd = 2;
	*negep = (*it) + 3;
	betain = one / beta;
	a = one;
	for (i = 1; i <= (*negep); i++) a *= betain;
	b = a;
	for (;;) {
		temp = one - a;
		if (temp - one != zero) break;
		a *= beta;
		--(*negep);
	}
	*negep = -(*negep);
	*epsneg = a;
	*machep = -(*it) - 3;
	a = b;
	for (;;) {
		temp = one + a;
		if (temp - one != zero) break;
		a *= beta;
		++(*machep);
	}
	*eps = a;
	*ngrd = 0;
	temp = one + (*eps);
	if (*irnd == 0 && temp * one - one != zero) *ngrd = 1;
	i = 0;
	k = 1;
	z = betain;
	t = one + (*eps);
	nxres = 0;
	for (;;) {
		y = z;
		z = y * y;
		a = z * one;
		temp = z * t;
		if (a + a == zero || fabs(z) >= y) break;
		temp1 = temp * betain;
		if (temp1 * beta == z) break;
		++i;
		k += k;
	}
	if (*ibeta != 10) {
		*iexp = i + 1;
		mx = k + k;
	}
	else {
		*iexp = 2;
		iz = (*ibeta);
		while (k >= iz) {
			iz *= *ibeta;
			++(*iexp);
		}
		mx = iz + iz - 1;
	}
	for (;;) {
		*xmin = y;
		y *= betain;
		a = y * one;
		temp = y * t;
		if (a + a != zero && fabs(y) < *xmin) {
			++k;
			temp1 = temp * betain;
			if (temp1 * beta == y && temp != y) {
				nxres = 3;
				*xmin = y;
				break;
			}
		}
		else break;
	}
	*minexp = -k;
	if (mx <= k + k - 3 && *ibeta != 10) {
		mx += mx;
		++(*iexp);
	}
	*maxexp = mx + (*minexp);
	*irnd += nxres;
	if (*irnd >= 2) *maxexp -= 2;
	i = (*maxexp) + (*minexp);
	if (*ibeta == 2 && !i) --(*maxexp);
	if (i > 20) --(*maxexp);
	if (a != y) *maxexp -= 2;
	*xmax = one - (*epsneg);
	if ((*xmax) * one != *xmax) *xmax = one - beta * (*epsneg);
	*xmax /= (*xmin * beta * beta * beta);
	i = (*maxexp) + (*minexp) + 3;
	for (j = 1; j <= i; j++) {
		if (*ibeta == 2) *xmax += *xmax;
		else *xmax *= beta;
	}

	//***** modified *****
	printf("The machine accuracy of DOUBLE is %e\n", *eps);
}


void get_eps()
{
	float f, f1, f2;
	f1 = 1;
	do
	{
		f = f1;
		f1 /= 2;
		f2 = f1 + 1;
	} while (f2 != 1);

	printf("The machine accuracy of FLOAT is %e\n", f);

	double u, u1, u2;
	u1 = 1;
	do
	{
		u = u1;
		u1 /= 2;
		u2 = u1 + 1;
	} while (u2 != 1);

	printf("The machine accuracy of DOUBLE is %e\n", u);
}

void method1()
{
	/*
	void machar(int* ibeta, int* it, int* irnd, int* ngrd, int* machep, int* negep,
		int* iexp, int* minexp, int* maxexp, float* eps, float* epsneg,
		float* xmin, float* xmax)
	*/

	int ibeta = 0;
	int it = 0;
	int machep = 0;
	float eps = 0;
	int negep = 0;
	float epsneg = 0;
	int iexp = 0;
	int minexp = 0;
	float xmin = 0;
	int maxexp = 0;
	float xmax = 0;
	int irnd = 0;
	int ngrd = 0;

	double eps2 = 0;
	double epsneg2 = 0;
	double xmin2 = 0;
	double xmax2 = 0;

	// Float
	machar(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &maxexp, &eps, &epsneg, &xmin, &xmax);

	// Double (modified)
	machar2(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &maxexp, &eps2, &epsneg2, &xmin2, &xmax2);

	printf("\n");
}


int main()
{
	printf("# Method 1\n");
	method1();

	printf("# Method 2\n");
	get_eps();

	return 0;
}