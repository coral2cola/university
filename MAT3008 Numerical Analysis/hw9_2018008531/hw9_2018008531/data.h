#pragma once
#include <stdio.h>


void read_data(FILE* file, float** x, float** y, float** _x, float** _y);
void make_matrix(float** x, float** y, float** _x, float** _y, float*** A, float** b1, float** b2);
void free_data(float** A, float* b1, float* b2, float* x, float* y, float* _x, float* _y);
void check_arr2D(float** arr);
void check_arr1D(float* arr);