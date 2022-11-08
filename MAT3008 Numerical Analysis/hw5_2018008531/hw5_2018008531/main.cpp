#include "equation.h"
#include <iostream>
#include <fstream>

using namespace std;

void gauss(float** a, int n, float** b, int m)
{
	gaussj(a, n, b, m);
	std::cout << "***********Gauss-Jordan Elimination***********" << std::endl;
	for (int i = 1; i <= n; i++)
	{
		cout << b[i][1] << endl;
	}
}

void lu(float** a, int n, int* indx, float d, float* b)
{
	std::cout << "***************LU Decomposition***************" << std::endl;
	ludcmp(a, n, indx, &d);
	lubksb(a, n, indx, b);

	for (int i = 1; i <= n; i++)
	{
		cout << b[i] << endl;
	}
}

float* svd(float** a, int m, int n, float w[], float** v, float b[], float x[])
{
	std::cout << "*********Singular Value Decomposition*********" << std::endl;
	svdcmp(a, m, n, w, v);
	svbksb(a, w, v, m, n, b, x);

	for (int i = 1; i <= n; i++)
		cout << x[i] << endl;
	return x;
}

void ii(float** a, int n, int* indx, float d, float b[], float x[])
{
	std::cout << "************Iterative Improvement*************" << std::endl;
	float** aa = matrix(1, n, 1, n);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			aa[i][j] = a[i][j];
		}
	}

	ludcmp(a, n, indx, &d);
	mprove(aa, a, n, indx, b, x);

	for (int i = 1; i <= n; i++)
		cout << x[i] << endl;

	free_matrix(aa, 1, n, 1, n);
}

void inverse(float** a, int n, int* indx, float d, float* col)
{
	std::cout << "****************Inverse Matrix****************" << std::endl;

	float** y = matrix(1, n, 1, n);
	ludcmp(a, n, indx, &d);
	for (int j = 1; j <= n; j++)
	{
		for (int i = 1; i <= n; i++) col[i] = 0.0;
		col[j] = 1.0;
		lubksb(a, n, indx, col);
		for (int i = 1; i <= n; i++) y[i][j] = col[i];
	}

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << y[i][j] << "      ";
		}
		cout << endl;
	}

	free_matrix(y, 1, n, 1, n);
}

void determinant(float** a, int n, int* indx, float d)
{
	std::cout << "******************Determinant*****************" << std::endl;
	ludcmp(a, n, indx, &d);
	for (int j = 1; j <= n; j++) d *= a[j][j];

	cout << "Determinant : " << d << endl;
}

int main()
{
	float read;
	ifstream fin;

	////////////////////////////////
	///////// lineq1.dat
	////////////////////////////////
	cout << "[ lineq1.dat ]" << endl;
	fin.open("lineq1.dat");
	
	if (!fin)
	{
		cout << "file open error" << endl;
		exit(1);
	}

	int n = 0;
	float s1[4][4];
	float** a;
	a = matrix(1, 4, 1, 4);
	
	float** b;
	float s2[4][1];
	b = matrix(1, 4, 1, 1);

	fin >> n;
	fin >> n;

	// ***********Gauss-Jordan Elimination***********

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fin >> read;
			s1[i][j] = read;
			a[i + 1][j + 1] = read;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		fin >> read;
		s2[i][0] = read;
		b[i + 1][1] = read;
	}

	gauss(a, 4, b, 1);


	free_matrix(a, 1, 4, 1, 4);
	free_matrix(b, 1, 4, 1, 1);

	// ***************LU Decomposition***************

	a = matrix(1, 4, 1, 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	float bb[4], *bbb;
	bbb = bb - 1;
	for (int i = 0; i < 4; i++)
	{
		bb[i] = s2[i][0];
	}


	int tmp[4], *indx;
	indx = tmp - 1;
	float d = 0;

	lu(a, 4, indx, d, bbb);

	free_matrix(a, 1, 4, 1, 4);

	// *********Singular Value Decomposition*********

	a = matrix(1, 4, 1, 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	float w[4], * ww;
	ww = w - 1;
	float** v = matrix(1, 4, 1, 4);
	float x[4], * xx;
	xx = x - 1;

	for (int i = 0; i < 4; i++)
	{
		bbb[i + 1] = s2[i][0];
	}

	float* sol1 = svd(a, 4, 4, ww, v, bbb, xx);

	free_matrix(a, 1, 4, 1, 4);
	free_matrix(v, 1, 4, 1, 4);

	// ************Iterative Improvement*************

	a = matrix(1, 4, 1, 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		bbb[i + 1] = s2[i][0];
	}


	int tmp11[4];
	indx = tmp11 - 1;
	d = 0;

	ii(a, 4, indx, d, bbb, sol1);

	free_matrix(a, 1, 4, 1, 4);

	// ****************Inverse Matrix****************

	a = matrix(1, 4, 1, 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	int tmp21[4];
	indx = tmp21 - 1;
	d = 0;

	inverse(a, 4, indx, d, bbb);

	free_matrix(a, 1, 4, 1, 4);

	// ******************Determinant*****************

	a = matrix(1, 4, 1, 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	int tmp31[4];
	indx = tmp31 - 1;
	d = 0;

	determinant(a, 4, indx, d);

	free_matrix(a, 1, 4, 1, 4);


	fin.close();

	cout << endl << endl;

	////////////////////////////////
	///////// lineq2.dat
	////////////////////////////////

	cout << "[ lineq2.dat ]" << endl;

	fin.open("lineq2.dat");

	if (!fin)
	{
		cout << "file open error" << endl;
		exit(1);
	}

	a = matrix(1, 5, 1, 5);
	b = matrix(1, 5, 1, 1);

	fin >> n;
	fin >> n;

	float s3[5][5];
	float s4[5][1];

	// ***********Gauss-Jordan Elimination***********

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			fin >> read;
			s3[i][j] = read;
			a[i + 1][j + 1] = read;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		fin >> read;
		s4[i][0] = read;
		b[i + 1][1] = read;
	}

	gauss(a, 5, b, 1);

	free_matrix(a, 1, 5, 1, 5);
	free_matrix(b, 1, 5, 1, 1);

	// ***************LU Decomposition***************

	a = matrix(1, 5, 1, 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			a[i + 1][j + 1] = s3[i][j];
		}
	}

	float cc[5], * ccc;
	ccc = cc - 1;
	for (int i = 0; i < 5; i++)
	{
		cc[i] = s4[i][0];
	}

	int tmp2[5];
	indx = tmp2 - 1;
	d = 0;
	lu(a, 5, indx, d, ccc);

	free_matrix(a, 1, 5, 1, 5);

	// *********Singular Value Decomposition*********

	a = matrix(1, 5, 1, 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			a[i + 1][j + 1] = s3[i][j];
		}
	}

	float p[5];
	ww = p - 1;
	v = matrix(1, 5, 1, 5);
	float y[5];
	xx = y - 1;

	for (int i = 0; i < 5; i++)
	{
		ccc[i + 1] = s4[i][0];
	}

	float* sol2 = svd(a, 5, 5, ww, v, ccc, xx);
	

	free_matrix(a, 1, 5, 1, 5);
	free_matrix(v, 1, 5, 1, 5);

	// ************Iterative Improvement*************

	a = matrix(1, 5, 1, 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			a[i + 1][j + 1] = s3[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		ccc[i + 1] = s4[i][0];
	}

	int tmp12[5];
	indx = tmp12 - 1;
	d = 0;

	ii(a, 5, indx, d, ccc, sol2);

	free_matrix(a, 1, 5, 1, 5);

	// ****************Inverse Matrix****************

	a = matrix(1, 5, 1, 5);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i + 1][j + 1] = s3[i][j];
		}
	}

	int tmp22[5];
	indx = tmp22 - 1;
	d = 0;

	inverse(a, 5, indx, d, ccc);

	free_matrix(a, 1, 5, 1, 5);

	// ******************Determinant*****************

	a = matrix(1, 5, 1, 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	int tmp32[5];
	indx = tmp32 - 1;
	d = 0;

	determinant(a, 5, indx, d);

	free_matrix(a, 1, 5, 1, 5);

	fin.close();

	cout << endl << endl;

	////////////////////////////////
	///////// lineq3.dat
	////////////////////////////////

	cout << "[ lineq3.dat ]" << endl;

	fin.open("lineq3.dat");

	if (!fin)
	{
		cout << "file open error" << endl;
		exit(1);
	}

	a = matrix(1, 6, 1, 6);
	b = matrix(1, 6, 1, 1);

	float s5[6][6];
	float s6[6][1];

	// ***********Gauss-Jordan Elimination***********

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			fin >> read;
			s5[i][j] = read;
			a[i + 1][j + 1] = read;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		fin >> read;
		s6[i][0] = read;
		b[i + 1][1] = read;
	}

	gauss(a, 6, b, 1);

	free_matrix(a, 1, 5, 1, 5);

	// ***************LU Decomposition***************

	a = matrix(1, 6, 1, 6);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			a[i + 1][j + 1] = s5[i][j];
		}
	}

	float dd[6], * ddd;
	ddd = dd - 1;
	for (int i = 0; i < 6; i++)
	{
		dd[i] = s6[i][0];
	}

	int tmp3[6];
	indx = tmp3 - 1;
	d = 0;
	lu(a, 6, indx, d, ddd);

	free_matrix(a, 1, 6, 1, 6);
	free_matrix(b, 1, 6, 1, 1);

	// *********Singular Value Decomposition*********

	a = matrix(1, 6, 1, 6);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			a[i + 1][j + 1] = s5[i][j];
		}
	}

	float q[6];
	ww = q - 1;
	v = matrix(1, 6, 1, 6);
	float z[6];
	xx = z - 1;

	for (int i = 0; i < 6; i++)
	{
		ddd[i + 1] = s6[i][0];
	}

	float* sol3 = svd(a, 6, 6, ww, v, ddd, xx);

	free_matrix(a, 1, 6, 1, 6);
	free_matrix(v, 1, 6, 1, 6);

	// ************Iterative Improvement*************

	a = matrix(1, 6, 1, 6);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			a[i + 1][j + 1] = s5[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		ddd[i + 1] = s6[i][0];
	}

	int tmp13[6];
	indx = tmp13 - 1;
	d = 0;

	ii(a, 6, indx, d, ddd, sol3);

	free_matrix(a, 1, 6, 1, 6);

	// ****************Inverse Matrix****************

	a = matrix(1, 6, 1, 6);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			a[i + 1][j + 1] = s5[i][j];
		}
	}

	int tmp23[6];
	indx = tmp23 - 1;
	d = 0;

	inverse(a, 6, indx, d, ddd);

	free_matrix(a, 1, 6, 1, 6);

	// ******************Determinant*****************

	a = matrix(1, 6, 1, 6);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			a[i + 1][j + 1] = s1[i][j];
		}
	}

	int tmp33[6];
	indx = tmp33 - 1;
	d = 0;

	determinant(a, 6, indx, d);

	free_matrix(a, 1, 6, 1, 6);

	fin.close();
	
	
	return 0;
}