#pragma once

void nrerror(char error_text[]);
float** matrix(long nrl, long nrh, long ncl, long nch);
void free_matrix(float** m, long nrl, long nrh, long ncl, long nch);
void free_ivector(int* v, long nl, long nh);
int* ivector(long nl, long nh);
float* vector(long nl, long nh);
void free_vector(float* v, long nl, long nh);
void gaussj(float** a, int n, float** b, int m);
void ludcmp(float** a, int n, int* indx, float* d);
void lubksb(float** a, int n, int* indx, float b[]);
void svdcmp(float** a, int m, int n, float w[], float** v);
void svbksb(float** u, float w[], float** v, int m, int n, float b[], float x[]);
void mprove(float** a, float** alud, int n, int indx[], float b[], float x[]);