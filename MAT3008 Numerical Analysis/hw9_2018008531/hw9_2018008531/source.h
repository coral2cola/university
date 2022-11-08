#pragma once

void solve_inverse(float** A, float* b);
void solve_gaussj(int row, int col, float** A, float* b);
void solve_LU(int row, int col, float** A, float* b);
void solve_SVD(int row, int col, float** A, float* b);
void solve_mprove(int row, int col, float** A, float* b);