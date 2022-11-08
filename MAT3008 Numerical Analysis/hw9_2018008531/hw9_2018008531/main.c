#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "source.h"


int main() {
	int tc;
	for (tc = 1; tc <= 3; tc++) {
		FILE* file;
		float* x;
		float* y;
		float* _x;
		float* _y;

		float** A;
		float* b1;
		float* b2;
		int i, j;

		char problem[200];
		printf("\n\n***** fitdata%d.dat *****\n", tc);


		sprintf(problem, "fitdata%d.dat", tc);
		file = fopen(problem, "r");
		read_data(file, &x, &y, &_x, &_y);
		fclose(file);
		make_matrix(&x, &y, &_x, &_y, &A, &b1, &b2);

		solve_inverse(A, b1);
		solve_inverse(A, b2);

		free_data(A, b1, b2, x, y, _x, _y);
	}
	return 0;
}
