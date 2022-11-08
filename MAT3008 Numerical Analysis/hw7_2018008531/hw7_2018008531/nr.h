#pragma once

void eigsrt(float d[], float** v, int n);
void jacobi(float** a, int n, float d[], float** v, int* nrot);
float ran1(long* idum);
float gasdev(long* idum);
float** matrix(long nrl, long nrh, long ncl, long nch);
void free_matrix(float** m, long nrl, long nrh, long ncl, long nch);