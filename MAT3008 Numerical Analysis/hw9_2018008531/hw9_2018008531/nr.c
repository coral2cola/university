#include <math.h>
#include <stdio.h>

#define SWAP(a,b) {float temp=(a);(a)=(b);(b)=temp;}
#define TINY 1.0e-20;
static float at, bt, ct;
#define PYTHAG(a,b) ((at=fabs(a)) > (bt=fabs(b)) ? \
(ct=bt/at,at*sqrt(1.0+ct*ct)) : (bt ? (ct=at/bt,bt*sqrt(1.0+ct*ct)): 0.0))

static float maxarg1, maxarg2;
#define MAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
	(maxarg1) : (maxarg2))
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))


void nrerror(error_text)
char error_text[];
/* Numerical Recipes standard error handler */
{
	void _exit();
	printf("exit");
	fprintf(stderr, "Numerical Recipes run-time error...\n");
	fprintf(stderr, "%s\n", error_text);
	fprintf(stderr, "...now exiting to system...\n");
	//_exit(1);
}

float* vector(nl, nh)
int nl, nh;
/* allocate a float vector with subscript range v[nl..nh] */
{
	float* v;

	v = (float*)malloc((unsigned)(nh - nl + 1) * sizeof(float)) - nl;
	if (!v) nrerror("allocation failure in vector()");
	return v;
}

int* ivector(nl, nh)
int nl, nh;
/* allocate an int vector with subscript range v[nl..nh] */
{
	int* v;

	v = (int*)malloc((unsigned)(nh - nl + 1) * sizeof(int)) - nl;
	if (!v) nrerror("allocation failure in ivector()");
	return v;
}

double* dvector(nl, nh)
int nl, nh;
/* allocate a double vector with subscript range v[nl..nh] */
{
	double* v;

	v = (double*)malloc((unsigned)(nh - nl + 1) * sizeof(double)) - nl;
	if (!v) nrerror("allocation failure in dvector()");
	return v;
}

float** matrix(nrl, nrh, ncl, nch)
int nrl, nrh, ncl, nch;
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	int i;
	float** m;

	/* allocate pointers to rows */
	m = (float**)malloc((unsigned)(nrh - nrl + 1) * sizeof(float*)) - nrl;
	if (!m) nrerror("allocation failure 1 in matrix()");

	/* allocate rows and set pointers to them */
	for (i = nrl; i <= nrh; i++) {
		m[i] = (float*)malloc((unsigned)(nch - ncl + 1) * sizeof(float)) - ncl;
		if (!m[i]) nrerror("allocation failure 2 in matrix()");
	}
	/* return pointer to array of pointers to rows */
	return m;
}

double** dmatrix(nrl, nrh, ncl, nch)
int nrl, nrh, ncl, nch;
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	int i;
	double** m;

	/* allocate pointers to rows */
	m = (double**)malloc((unsigned)(nrh - nrl + 1) * sizeof(double*)) - nrl;
	if (!m) nrerror("allocation failure 1 in dmatrix()");

	/* allocate rows and set pointers to them */
	for (i = nrl; i <= nrh; i++) {
		m[i] = (double*)malloc((unsigned)(nch - ncl + 1) * sizeof(double)) - ncl;
		if (!m[i]) nrerror("allocation failure 2 in dmatrix()");
	}
	/* return pointer to array of pointers to rows */
	return m;
}

int** imatrix(nrl, nrh, ncl, nch)
int nrl, nrh, ncl, nch;
/* allocate an int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	int i, ** m;

	/* allocate pointers to rows */
	m = (int**)malloc((unsigned)(nrh - nrl + 1) * sizeof(int*)) - nrl;
	if (!m) nrerror("allocation failure 1 in imatrix()");

	/* allocate rows and set pointers to them */
	for (i = nrl; i <= nrh; i++) {
		m[i] = (int*)malloc((unsigned)(nch - ncl + 1) * sizeof(int)) - ncl;
		if (!m[i]) nrerror("allocation failure 2 in imatrix()");
	}
	/* return pointer to array of pointers to rows */
	return m;
}

float** submatrix(a, oldrl, oldrh, oldcl, oldch, newrl, newcl)
/* point a submatrix [newrl..][newcl..] to a[oldrl..oldrh][oldcl..oldch] */
float** a;
int oldrl, oldrh, oldcl, oldch, newrl, newcl;
{
	int i, j;
	float** m;

	/* allocate array of pointers to rows */
	m = (float**)malloc((unsigned)(oldrh - oldrl + 1) * sizeof(float*)) - newrl;
	if (!m) nrerror("allocation failure in submatrix()");

	/* set pointers to rows */
	for (i = oldrl, j = newrl; i <= oldrh; i++, j++) m[j] = a[i] + oldcl - newcl;

	/* return pointer to array of pointers to rows */
	return m;
}

float** convert_matrix(a, nrl, nrh, ncl, nch)
float* a;
int nrl, nrh, ncl, nch;
/* allocate a float matrix m[nrl..nrh][ncl..nch] that points to the matrix
declared in the standard C manner as a[nrow][ncol], where nrow=nrh-nrl+1
and ncol=nch-ncl+1. The routine should be called with the address
&a[0][0] as the first argument. */
{
	int i, j, nrow, ncol;
	float** m;

	nrow = nrh - nrl + 1;
	ncol = nch - ncl + 1;
	/* allocate pointers to rows */
	if ((m = (float**)malloc((unsigned)(nrow) * sizeof(float*))) == NULL)
		nrerror("allocation failure in convert_matrix()");
	m -= nrl;

	/* set pointers to rows */
	for (i = 0, j = nrl; i <= nrow - 1; i++, j++) m[j] = a + ncol * i - ncl;
	/* return pointer to array of pointers to rows */
	return m;
}

void free_vector(v, nl, nh)
float* v;
int nl, nh;
/* free a float vector allocated with vector() */
{
	free((char*)(v + nl));
}

void free_ivector(v, nl, nh)
int* v, nl, nh;
/* free an int vector allocated with ivector() */
{
	free((char*)(v + nl));
}

void free_dvector(v, nl, nh)
double* v;
int nl, nh;
/* free a double vector allocated with dvector() */
{
	free((char*)(v + nl));
}

void free_matrix(m, nrl, nrh, ncl, nch)
float** m;
int nrl, nrh, ncl, nch;
/* free a float matrix allocated by matrix() */
{
	int i;

	for (i = nrh; i >= nrl; i--) free((char*)(m[i] + ncl));
	free((char*)(m + nrl));
}

void free_dmatrix(m, nrl, nrh, ncl, nch)
double** m;
int nrl, nrh, ncl, nch;
/* free a double matrix allocated by dmatrix() */
{
	int i;

	for (i = nrh; i >= nrl; i--) free((char*)(m[i] + ncl));
	free((char*)(m + nrl));
}

void free_imatrix(m, nrl, nrh, ncl, nch)
int** m;
int nrl, nrh, ncl, nch;
/* free an int matrix allocated by imatrix() */
{
	int i;

	for (i = nrh; i >= nrl; i--) free((char*)(m[i] + ncl));
	free((char*)(m + nrl));
}

void free_submatrix(b, nrl, nrh, ncl, nch)
/* free a submatrix allocated by submatrix() */
float** b;
int nrl, nrh, ncl, nch;
{
	free((char*)(b + nrl));
}

void free_convert_matrix(b, nrl, nrh, ncl, nch)
/* free a matrix allocated by convert_matrix() */
float** b;
int nrl, nrh, ncl, nch;
{
	free((char*)(b + nrl));
}

void gaussj(float** a, int n, float** b, int m)
{
	int* indxc, * indxr, * ipiv;
	int i, icol, irow, j, k, l, ll, * ivector();
	float big, dum, pivinv;
	void nrerror(), free_ivector();
	indxc = ivector(1, n);
	indxr = ivector(1, n);
	ipiv = ivector(1, n);
	for (j = 1; j <= n; j++) ipiv[j] = 0;
	for (i = 1; i <= n; i++) {
		big = 0.0;
		for (j = 1; j <= n; j++)
			if (ipiv[j] != 1)
				for (k = 1; k <= n; k++) {

					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big = fabs(a[j][k]);
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1) nrerror("GAUSSJ: Singular Matrix-1");
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l = 1; l <= n; l++) SWAP(a[irow][l], a[icol][l])
				for (l = 1; l <= m; l++) SWAP(b[irow][l], b[icol][l])
		}

		indxr[i] = irow;
		indxc[i] = icol;
		if (a[icol][icol] == 0.0) nrerror("GAUSSJ: Singular Matrix-2");
		pivinv = 1.0 / a[icol][icol];
		a[icol][icol] = 1.0;
		for (l = 1; l <= n; l++) a[icol][l] *= pivinv;
		for (l = 1; l <= m; l++) b[icol][l] *= pivinv;
		for (ll = 1; ll <= n; ll++)
			if (ll != icol) {
				dum = a[ll][icol];
				a[ll][icol] = 0.0;
				for (l = 1; l <= n; l++) a[ll][l] -= a[icol][l] * dum;
				for (l = 1; l <= m; l++) b[ll][l] -= b[icol][l] * dum;
			}
	}
	for (l = n; l >= 1; l--) {
		if (indxr[l] != indxc[l])
			for (k = 1; k <= n; k++)
				SWAP(a[k][indxr[l]], a[k][indxc[l]]);
	}
	free_ivector(ipiv, 1, n);
	free_ivector(indxr, 1, n);
	free_ivector(indxc, 1, n);
}

#undef SWAP

void lubksb(float** a, int n, int* indx, float* b)
{
	int i, ii = 0, ip, j;
	float sum;

	for (i = 1; i <= n; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii)
			for (j = ii; j <= i - 1; j++) sum -= a[i][j] * b[j];
		else if (sum) ii = i;
		b[i] = sum;
	}
	for (i = n; i >= 1; i--) {
		sum = b[i];
		for (j = i + 1; j <= n; j++) sum -= a[i][j] * b[j];
		b[i] = sum / a[i][i];
	}
}


void ludcmp(float** a, int n, int* indx, float* d)
{
	int i, imax, j, k;
	float big, dum, sum, temp;
	float* vv, * vector();
	void nrerror(), free_vector();

	vv = vector(1, n);
	*d = 1.0;
	for (i = 1; i <= n; i++) {
		big = 0.0;
		for (j = 1; j <= n; j++)
			if ((temp = fabs(a[i][j])) > big) big = temp;
		if (big == 0.0) nrerror("Singular matrix in routine LUDCMP");
		vv[i] = 1.0 / big;
	}
	for (j = 1; j <= n; j++) {
		for (i = 1; i < j; i++) {
			sum = a[i][j];
			for (k = 1; k < i; k++) sum -= a[i][k] * a[k][j];
			a[i][j] = sum;
		}
		big = 0.0;
		for (i = j; i <= n; i++) {
			sum = a[i][j];
			for (k = 1; k < j; k++)
				sum -= a[i][k] * a[k][j];
			a[i][j] = sum;
			if ((dum = vv[i] * fabs(sum)) >= big) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) {
			for (k = 1; k <= n; k++) {
				dum = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k] = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (a[j][j] == 0.0) a[j][j] = TINY;
		if (j != n) {
			dum = 1.0 / (a[j][j]);
			for (i = j + 1; i <= n; i++) a[i][j] *= dum;
		}
	}
	free_vector(vv, 1, n);
}

#undef TINY

void mprove(float** a, float** alud, int n, int* indx, float* b, float* x)
{
	int j, i;
	double sdp;
	float* r, * vector();
	void lubksb(), free_vector();

	r = vector(1, n);
	for (i = 1; i <= n; i++) {
		sdp = -b[i];
		for (j = 1; j <= n; j++) sdp += a[i][j] * x[j];
		r[i] = sdp;
	}
	lubksb(alud, n, indx, r);
	for (i = 1; i <= n; i++) x[i] -= r[i];
	free_vector(r, 1, n);
}

void svbksb(float** u, float* w, float** v, int m, int n, float* b, float* x)
{
	int jj, j, i;
	float s, * tmp, * vector();
	void free_vector();

	tmp = vector(1, n);
	for (j = 1; j <= n; j++) {
		s = 0.0;
		if (w[j]) {
			for (i = 1; i <= m; i++) s += u[i][j] * b[i];
			s /= w[j];
		}
		tmp[j] = s;
	}
	for (j = 1; j <= n; j++) {
		s = 0.0;
		for (jj = 1; jj <= n; jj++) s += v[j][jj] * tmp[jj];
		x[j] = s;
	}
	free_vector(tmp, 1, n);
}


void svdcmp(float** a, int m, int n, float* w, float** v)
{
	int flag, i, its, j, jj, k, l, nm;
	float c, f, h, s, x, y, z;
	float anorm = 0.0, g = 0.0, scale = 0.0;
	float* rv1, * vector();
	void nrerror(), free_vector();

	if (m < n) nrerror("SVDCMP: You must augment A with extra zero rows");
	rv1 = vector(1, n);
	for (i = 1; i <= n; i++) {
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m) {
			for (k = i; k <= m; k++) scale += fabs(a[k][i]);
			if (scale) {
				for (k = i; k <= m; k++) {
					a[k][i] /= scale;
					s += a[k][i] * a[k][i];
				}
				f = a[i][i];
				g = -SIGN(sqrt(s), f);
				h = f * g - s;
				a[i][i] = f - g;
				if (i != n) {
					for (j = l; j <= n; j++) {
						for (s = 0.0, k = i; k <= m; k++) s += a[k][i] * a[k][j];
						f = s / h;
						for (k = i; k <= m; k++) a[k][j] += f * a[k][i];
					}
				}
				for (k = i; k <= m; k++) a[k][i] *= scale;
			}
		}
		w[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m && i != n) {
			for (k = l; k <= n; k++) scale += fabs(a[i][k]);
			if (scale) {
				for (k = l; k <= n; k++) {
					a[i][k] /= scale;
					s += a[i][k] * a[i][k];
				}
				f = a[i][l];
				g = -SIGN(sqrt(s), f);
				h = f * g - s;
				a[i][l] = f - g;
				for (k = l; k <= n; k++) rv1[k] = a[i][k] / h;
				if (i != m) {
					for (j = l; j <= m; j++) {
						for (s = 0.0, k = l; k <= n; k++) s += a[j][k] * a[i][k];
						for (k = l; k <= n; k++) a[j][k] += s * rv1[k];
					}
				}
				for (k = l; k <= n; k++) a[i][k] *= scale;
			}
		}
		anorm = MAX(anorm, (fabs(w[i]) + fabs(rv1[i])));
	}
	for (i = n; i >= 1; i--) {
		if (i < n) {
			if (g) {
				for (j = l; j <= n; j++)
					v[j][i] = (a[i][j] / a[i][l]) / g;
				for (j = l; j <= n; j++) {
					for (s = 0.0, k = l; k <= n; k++) s += a[i][k] * v[k][j];
					for (k = l; k <= n; k++) v[k][j] += s * v[k][i];
				}
			}
			for (j = l; j <= n; j++) v[i][j] = v[j][i] = 0.0;
		}
		v[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}
	for (i = n; i >= 1; i--) {
		l = i + 1;
		g = w[i];
		if (i < n)
			for (j = l; j <= n; j++) a[i][j] = 0.0;
		if (g) {
			g = 1.0 / g;
			if (i != n) {
				for (j = l; j <= n; j++) {
					for (s = 0.0, k = l; k <= m; k++) s += a[k][i] * a[k][j];
					f = (s / a[i][i]) * g;
					for (k = i; k <= m; k++) a[k][j] += f * a[k][i];
				}
			}
			for (j = i; j <= m; j++) a[j][i] *= g;
		}
		else {
			for (j = i; j <= m; j++) a[j][i] = 0.0;
		}
		++a[i][i];
	}
	for (k = n; k >= 1; k--) {
		for (its = 1; its <= 30; its++) {
			flag = 1;
			for (l = k; l >= 1; l--) {
				nm = l - 1;
				if ((float)(fabs(rv1[l]) + anorm) == anorm) {
					flag = 0;
					break;
				}
				if ((float)(fabs(w[nm]) + anorm) == anorm) break;
			}
			if (flag) {
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++) {
					f = s * rv1[i];
					rv1[i] = c * rv1[i];
					if ((float)(fabs(f) + anorm) == anorm) break;
					g = w[i];
					h = PYTHAG(f, g);
					w[i] = h;
					h = 1.0 / h;
					c = g * h;
					s = (-f * h);
					for (j = 1; j <= m; j++) {
						y = a[j][nm];
						z = a[j][i];
						a[j][nm] = y * c + z * s;
						a[j][i] = z * c - y * s;
					}
				}
			}
			z = w[k];
			if (l == k) {
				if (z < 0.0) {
					w[k] = -z;
					for (j = 1; j <= n; j++) v[j][k] = (-v[j][k]);
				}
				break;
			}
			if (its == 30) nrerror("No convergence in 30 SVDCMP iterations");
			x = w[l];
			nm = k - 1;
			y = w[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
			g = PYTHAG(f, 1.0);
			f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
			c = s = 1.0;
			for (j = l; j <= nm; j++) {
				i = j + 1;
				g = rv1[i];
				y = w[i];
				h = s * g;
				g = c * g;
				z = PYTHAG(f, h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 1; jj <= n; jj++) {
					x = v[jj][j];
					z = v[jj][i];
					v[jj][j] = x * c + z * s;
					v[jj][i] = z * c - x * s;
				}
				z = PYTHAG(f, h);
				w[j] = z;
				if (z) {
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 1; jj <= m; jj++) {
					y = a[jj][j];
					z = a[jj][i];
					a[jj][j] = y * c + z * s;
					a[jj][i] = z * c - y * s;
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			w[k] = x;
		}
	}
	free_vector(rv1, 1, n);
}

#undef SIGN
#undef MAX
#undef PYTHAG