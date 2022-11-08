#include <iostream>
#include "nr.h"

using namespace std;

int main()
{
	float** a;
	float** v;
	float s1[11], * d;
	d = s1 - 1;
	a = matrix(1, 11, 1, 11);
	v = matrix(1, 11, 1, 11);
	int nrot;

	long idum = -1.234;

	for (int i = 1; i <= 11; i++)
	{
		for (int j = i; j <= 11; j++)
		{
			a[i][j] = gasdev(&idum);
		}
	}

	jacobi(a, 11, d, v, &nrot);
	eigsrt(d, v, 11);

	cout << "Eigenvalues" << endl;
	for (int i = 1; i <= 11; i++) cout << d[i] << ' ';
	cout << endl << endl << "Eigenvectors" << endl;
	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			cout << v[i][j] << ' ';
		}
		cout << endl;
	}

	free_matrix(a, 1, 11, 1, 11);
	free_matrix(v, 1, 11, 1, 11);
	return 0;
}