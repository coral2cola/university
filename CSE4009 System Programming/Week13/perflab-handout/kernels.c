/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "YeonJu Song",              /* name */

    "2018008531",     /* student ID */
    "coral2cola@hanyang.ac.kr",  /* student email */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, k, l;
    for (i = 0; i < dim; i += 8)
    {
        for (j = 0; j < dim; j += 8)
        {
            for (k = i; k < i + 8; k++)
            {
                for (l = j; l < j + 8; l++)
                    dst[RIDX(dim - 1 - l, k, dim)] = src[RIDX(k, l, dim)];
            }
        }
    }
}

char rotate_descr_1[] = "rotate1";
void rotate1(int dim, pixel* src, pixel* dst)
{
    int i, j, k, l;
    for (i = 0; i < dim; i += 32)
    {
        for (j = 0; j < dim; j += 32)
        {
            for (k = i; k < i + 32; k++)
            {
                for (l = j; l < j + 32; l++)
                    dst[RIDX(dim - 1 - l, k, dim)] = src[RIDX(k, l, dim)];
            }
        }
    }
}

char rotate_descr_2[] = "rotate2";
void rotate2(int dim, pixel* src, pixel* dst)
{
    int i, j;
    for (i = 0; i < dim; i += 32)
    {
        for (j = 0; j < dim; j++)
        {
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(dim - 1 - j, i + 1, dim)] = src[RIDX(i + 1, j, dim)];
            dst[RIDX(dim - 1 - j, i + 2, dim)] = src[RIDX(i + 2, j, dim)];
            dst[RIDX(dim - 1 - j, i + 3, dim)] = src[RIDX(i + 3, j, dim)];
            dst[RIDX(dim - 1 - j, i + 4, dim)] = src[RIDX(i + 4, j, dim)];
            dst[RIDX(dim - 1 - j, i + 5, dim)] = src[RIDX(i + 5, j, dim)];
            dst[RIDX(dim - 1 - j, i + 6, dim)] = src[RIDX(i + 6, j, dim)];
            dst[RIDX(dim - 1 - j, i + 7, dim)] = src[RIDX(i + 7, j, dim)];
            dst[RIDX(dim - 1 - j, i + 8, dim)] = src[RIDX(i + 8, j, dim)];
            dst[RIDX(dim - 1 - j, i + 9, dim)] = src[RIDX(i + 9, j, dim)];
            dst[RIDX(dim - 1 - j, i + 10, dim)] = src[RIDX(i + 10, j, dim)];
            dst[RIDX(dim - 1 - j, i + 11, dim)] = src[RIDX(i + 11, j, dim)];
            dst[RIDX(dim - 1 - j, i + 12, dim)] = src[RIDX(i + 12, j, dim)];
            dst[RIDX(dim - 1 - j, i + 13, dim)] = src[RIDX(i + 13, j, dim)];
            dst[RIDX(dim - 1 - j, i + 14, dim)] = src[RIDX(i + 14, j, dim)];
            dst[RIDX(dim - 1 - j, i + 15, dim)] = src[RIDX(i + 15, j, dim)];
            dst[RIDX(dim - 1 - j, i + 16, dim)] = src[RIDX(i + 16, j, dim)];
            dst[RIDX(dim - 1 - j, i + 17, dim)] = src[RIDX(i + 17, j, dim)];
            dst[RIDX(dim - 1 - j, i + 18, dim)] = src[RIDX(i + 18, j, dim)];
            dst[RIDX(dim - 1 - j, i + 19, dim)] = src[RIDX(i + 19, j, dim)];
            dst[RIDX(dim - 1 - j, i + 20, dim)] = src[RIDX(i + 20, j, dim)];
            dst[RIDX(dim - 1 - j, i + 21, dim)] = src[RIDX(i + 21, j, dim)];
            dst[RIDX(dim - 1 - j, i + 22, dim)] = src[RIDX(i + 22, j, dim)];
            dst[RIDX(dim - 1 - j, i + 23, dim)] = src[RIDX(i + 23, j, dim)];
            dst[RIDX(dim - 1 - j, i + 24, dim)] = src[RIDX(i + 24, j, dim)];
            dst[RIDX(dim - 1 - j, i + 25, dim)] = src[RIDX(i + 25, j, dim)];
            dst[RIDX(dim - 1 - j, i + 26, dim)] = src[RIDX(i + 26, j, dim)];
            dst[RIDX(dim - 1 - j, i + 27, dim)] = src[RIDX(i + 27, j, dim)];
            dst[RIDX(dim - 1 - j, i + 28, dim)] = src[RIDX(i + 28, j, dim)];
            dst[RIDX(dim - 1 - j, i + 29, dim)] = src[RIDX(i + 29, j, dim)];
            dst[RIDX(dim - 1 - j, i + 30, dim)] = src[RIDX(i + 30, j, dim)];
            dst[RIDX(dim - 1 - j, i + 31, dim)] = src[RIDX(i + 31, j, dim)];
        }
    }
}

char rotate_descr_3[] = "rotate3";
void rotate3(int dim, pixel* src, pixel* dst)
{
    int i, j, ii, jj, bsize = 32;

    for (ii = 0; ii < dim; ii += bsize)
        for (jj = 0; jj < dim; jj += bsize)
            for (i = ii; i < ii + bsize; i += 4)
                for (j = jj; j < jj + bsize; j += 8) {
                    dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
                    dst[RIDX(dim - 1 - j, i + 1, dim)] = src[RIDX(i + 1, j, dim)];
                    dst[RIDX(dim - 1 - j, i + 2, dim)] = src[RIDX(i + 2, j, dim)];
                    dst[RIDX(dim - 1 - j, i + 3, dim)] = src[RIDX(i + 3, j, dim)];

                    dst[RIDX(dim - 1 - j - 1, i, dim)] = src[RIDX(i, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 1, i + 1, dim)] = src[RIDX(i + 1, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 1, i + 2, dim)] = src[RIDX(i + 2, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 1, i + 3, dim)] = src[RIDX(i + 3, j + 1, dim)];

                    dst[RIDX(dim - 1 - j - 2, i, dim)] = src[RIDX(i, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 2, i + 1, dim)] = src[RIDX(i + 1, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 2, i + 2, dim)] = src[RIDX(i + 2, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 2, i + 3, dim)] = src[RIDX(i + 3, j + 2, dim)];

                    dst[RIDX(dim - 1 - j - 3, i, dim)] = src[RIDX(i, j + 3, dim)];
                    dst[RIDX(dim - 1 - j - 3, i + 1, dim)] = src[RIDX(i + 1, j + 3, dim)];
                    dst[RIDX(dim - 1 - j - 3, i + 2, dim)] = src[RIDX(i + 2, j + 3, dim)];
                    dst[RIDX(dim - 1 - j - 3, i + 3, dim)] = src[RIDX(i + 3, j + 3, dim)];

                    dst[RIDX(dim - 1 - j - 4, i, dim)] = src[RIDX(i, j + 4, dim)];
                    dst[RIDX(dim - 1 - j - 4, i + 1, dim)] = src[RIDX(i + 1, j + 4, dim)];
                    dst[RIDX(dim - 1 - j - 4, i + 2, dim)] = src[RIDX(i + 2, j + 4, dim)];
                    dst[RIDX(dim - 1 - j - 4, i + 3, dim)] = src[RIDX(i + 3, j + 4, dim)];

                    dst[RIDX(dim - 1 - j - 5, i, dim)] = src[RIDX(i, j + 5, dim)];
                    dst[RIDX(dim - 1 - j - 5, i + 1, dim)] = src[RIDX(i + 1, j + 5, dim)];
                    dst[RIDX(dim - 1 - j - 5, i + 2, dim)] = src[RIDX(i + 2, j + 5, dim)];
                    dst[RIDX(dim - 1 - j - 5, i + 3, dim)] = src[RIDX(i + 3, j + 5, dim)];

                    dst[RIDX(dim - 1 - j - 6, i, dim)] = src[RIDX(i, j + 6, dim)];
                    dst[RIDX(dim - 1 - j - 6, i + 1, dim)] = src[RIDX(i + 1, j + 6, dim)];
                    dst[RIDX(dim - 1 - j - 6, i + 2, dim)] = src[RIDX(i + 2, j + 6, dim)];
                    dst[RIDX(dim - 1 - j - 6, i + 3, dim)] = src[RIDX(i + 3, j + 6, dim)];

                    dst[RIDX(dim - 1 - j - 7, i, dim)] = src[RIDX(i, j + 7, dim)];
                    dst[RIDX(dim - 1 - j - 7, i + 1, dim)] = src[RIDX(i + 1, j + 7, dim)];
                    dst[RIDX(dim - 1 - j - 7, i + 2, dim)] = src[RIDX(i + 2, j + 7, dim)];
                    dst[RIDX(dim - 1 - j - 7, i + 3, dim)] = src[RIDX(i + 3, j + 7, dim)];
                }
}

char rotate_descr_4[] = "rotate4";
void rotate4(int dim, pixel* src, pixel* dst)
{
    int i, j, ii, jj, a, bsize = 32;

    for (ii = 0; ii < dim; ii += bsize)
        for (jj = 0; jj < dim; jj += bsize)
            for (i = ii; i < ii + bsize; i += 4)
                for (j = jj; j < jj + bsize; j += 8) {
                    a = dim - 1 - j;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 1, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 1, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 1, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 1, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 2, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 2, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 2, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 2, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 3, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 3, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 3, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 3, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 4, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 4, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 4, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 4, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 5, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 5, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 5, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 5, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 6, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 6, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 6, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 6, dim)];
                    a--;
                    dst[RIDX(a, i, dim)] = src[RIDX(i, j + 7, dim)];
                    dst[RIDX(a, i + 1, dim)] = src[RIDX(i + 1, j + 7, dim)];
                    dst[RIDX(a, i + 2, dim)] = src[RIDX(i + 2, j + 7, dim)];
                    dst[RIDX(a, i + 3, dim)] = src[RIDX(i + 3, j + 7, dim)];
                }
}

char rotate_descr_5[] = "rotate5";
void rotate5(int dim, pixel* src, pixel* dst)
{
    int i, j;
    int tmpd = (dim - 1) * dim;
    int tmps = 0;

    for (i = 0; i < dim; i += 32)
    {
        for (j = 0; j < dim; j++)
        {
            //No.1
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.2
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.3
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.4
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.5
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.6
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.7
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.8
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.9
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.10
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.11
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.12
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.13
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.14
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.15
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.16
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.17
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.18
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.19
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.20
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.21
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.22
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.23
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.24
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.25
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.26
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.27
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.28
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.29
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.30
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.31
            dst[tmpd] = src[tmps];
            tmpd++;
            tmps += dim;

            //No.32
            dst[tmpd] = src[tmps];
            tmps -= 31 * dim;
            tmps++;
            tmpd -= 31;
            tmpd -= dim;
        }
        tmpd += dim * dim;
        tmpd += 32;
        tmps += 31 * dim;
    }
}


char rotate_descr_6[] = "rotate6";
void rotate6(int dim, pixel* src, pixel* dst)
{
    int i, j;
    int buff = dim * dim;

    dst += buff - dim;
    for (i = 0; i < dim; i += 32)
    {
        for (j = 0; j < dim; j++)
        {
            *dst = *src;
            src += dim;

            *(dst + 1) = *src;
            src += dim;

            *(dst + 2) = *src;
            src += dim;

            *(dst + 3) = *src;
            src += dim;

            *(dst + 4) = *src;
            src += dim;

            *(dst + 5) = *src;
            src += dim;

            *(dst + 6) = *src;
            src += dim;

            *(dst + 7) = *src;
            src += dim;

            *(dst + 8) = *src;
            src += dim;

            *(dst + 9) = *src;
            src += dim;

            *(dst + 10) = *src;
            src += dim;

            *(dst + 11) = *src;
            src += dim;

            *(dst + 12) = *src;
            src += dim;

            *(dst + 13) = *src;
            src += dim;

            *(dst + 14) = *src;
            src += dim;

            *(dst + 15) = *src;
            src += dim;

            *(dst + 16) = *src;
            src += dim;

            *(dst + 17) = *src;
            src += dim;

            *(dst + 18) = *src;
            src += dim;

            *(dst + 19) = *src;
            src += dim;

            *(dst + 20) = *src;
            src += dim;

            *(dst + 21) = *src;
            src += dim;

            *(dst + 22) = *src;
            src += dim;

            *(dst + 23) = *src;
            src += dim;

            *(dst + 24) = *src;
            src += dim;

            *(dst + 25) = *src;
            src += dim;

            *(dst + 26) = *src;
            src += dim;

            *(dst + 27) = *src;
            src += dim;

            *(dst + 28) = *src;
            src += dim;

            *(dst + 29) = *src;
            src += dim;

            *(dst + 30) = *src;
            src += dim;

            *(dst + 31) = *src;

            dst -= dim;
            src -= dim * 31 - 1;
        }
        dst += 32 + buff;
        src += dim * 31;
    }
    return;
}

#define move(a) dst[RIDX(dim-1-j, i+(a), dim)] = src[RIDX(i+(a), j, dim)];

char rotate_descr_7[] = "rotate7";
void rotate7(int dim, pixel* src, pixel* dst)
{

    int i, j;

    for (i = 0; i < dim; i += 16)
        for (j = 0; j < dim; j++) {
            move(0); move(1); move(2); move(3);
            move(4); move(5); move(6); move(7);
            move(8); move(9); move(10); move(11);
            move(12); move(13); move(14); move(15);
        }

}


/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
    add_rotate_function(&rotate1, rotate_descr_1);
    add_rotate_function(&rotate2, rotate_descr_2);
    add_rotate_function(&rotate3, rotate_descr_3);
    add_rotate_function(&rotate4, rotate_descr_4);
    add_rotate_function(&rotate5, rotate_descr_5);
    add_rotate_function(&rotate6, rotate_descr_6);
    add_rotate_function(&rotate7, rotate_descr_7);
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	        dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel* src, pixel* dst)
{
    int i, j, k;
    //four angles
    dst[0].red = (src[0].red + src[1].red + src[dim].red + src[dim + 1].red) / 4;
    dst[0].green = (src[0].green + src[1].green + src[dim].green + src[dim + 1].green) / 4;
    dst[0].blue = (src[0].blue + src[1].blue + src[dim].blue + src[dim + 1].blue) / 4;
    i = dim - 1;
    dst[i].red = (src[i - 1].red + src[i].red + src[i - 1 + dim].red + src[i + dim].red) / 4;
    dst[i].green = (src[i - 1].green + src[i].green + src[i - 1 + dim].green + src[i + dim].green) / 4;
    dst[i].blue = (src[i - 1].blue + src[i].blue + src[i - 1 + dim].blue + src[i + dim].blue) / 4;
    i = (dim - 1) * dim;
    dst[i].red = (src[i - dim].red + src[i - dim + 1].red + src[i].red + src[i + 1].red) / 4;
    dst[i].green = (src[i - dim].green + src[i - dim + 1].green + src[i].green + src[i + 1].green) / 4;
    dst[i].blue = (src[i - dim].blue + src[i - dim + 1].blue + src[i].blue + src[i + 1].blue) / 4;
    i = dim * dim - 1;
    dst[i].red = (src[i - dim].red + src[i - dim - 1].red + src[i - 1].red + src[i].red) / 4;
    dst[i].green = (src[i - dim].green + src[i - dim - 1].green + src[i - 1].green + src[i].green) / 4;
    dst[i].blue = (src[i - dim].blue + src[i - dim - 1].blue + src[i - 1].blue + src[i].blue) / 4;

    //four edge
    j = dim - 1;
    for (i = 1; i < j; i++)
    {
        dst[i].red = (src[i - 1].red + src[i].red + src[i + 1].red + src[i - 1 + dim].red + src[i + dim].red + src[i + 1 + dim].red) / 6;
        dst[i].green = (src[i - 1].green + src[i].green + src[i + 1].green + src[i - 1 + dim].green + src[i + dim].green + src[i + 1 + dim].green) / 6;
        dst[i].blue = (src[i - 1].blue + src[i].blue + src[i + 1].blue + src[i - 1 + dim].blue + src[i + dim].blue + src[i + 1 + dim].blue) / 6;
    }
    i = (dim - 1) * (dim)+1;
    j = (dim * dim) - 1;
    for (; i < j; i++)
    {
        dst[i].red = (src[i - 1].red + src[i].red + src[i + 1].red + src[i - dim].red + src[i - 1 - dim].red + src[i + 1 - dim].red) / 6;
        dst[i].green = (src[i - 1].green + src[i].green + src[i + 1].green + src[i - dim].green + src[i - 1 - dim].green + src[i + 1 - dim].green) / 6;
        dst[i].blue = (src[i - 1].blue + src[i].blue + src[i + 1].blue + src[i - dim].blue + src[i - 1 - dim].blue + src[i + 1 - dim].blue) / 6;
    }
    i = dim; j = (dim - 1) * dim;
    for (; i < j; i += dim)
    {
        dst[i].red = (src[i - dim].red + src[i - dim + 1].red + src[i].red + src[i + 1].red + src[i + dim].red + src[i + dim + 1].red) / 6;
        dst[i].green = (src[i - dim].green + src[i - dim + 1].green + src[i].green + src[i + 1].green + src[i + dim].green + src[i + dim + 1].green) / 6;
        dst[i].blue = (src[i - dim].blue + src[i - dim + 1].blue + src[i].blue + src[i + 1].blue + src[i + dim].blue + src[i + dim + 1].blue) / 6;
    }
    i = dim * 2 - 1; j = (dim) * (dim)-1;
    for (; i < j; i += dim)
    {
        dst[i].red = (src[i - dim].red + src[i - dim - 1].red + src[i - 1].red + src[i].red + src[i + dim - 1].red + src[i + dim].red) / 6;
        dst[i].green = (src[i - dim].green + src[i - dim - 1].green + src[i - 1].green + src[i].green + src[i + dim - 1].green + src[i + dim].green) / 6;
        dst[i].blue = (src[i - dim].blue + src[i - dim - 1].blue + src[i - 1].blue + src[i].blue + src[i + dim - 1].blue + src[i + dim].blue) / 6;
    }
    //center
    for (i = 1; i < dim - 1; i++)
        for (j = 1; j < dim - 1; j++)
        {
            k = (i * dim) + j;
            dst[k].red = (src[k - dim - 1].red + src[k - dim].red + src[k - dim + 1].red + src[k - 1].red + src[k].red + src[k + 1].red + src[k - 1 + dim].red + src[k + dim].red + src[k + dim + 1].red) / 9;
            dst[k].green = (src[k - dim - 1].green + src[k - dim].green + src[k - dim + 1].green + src[k - 1].green + src[k].green + src[k + 1].green + src[k - 1 + dim].green + src[k + dim].green + src[k + dim + 1].green) / 9;
            dst[k].blue = (src[k - dim - 1].blue + src[k - dim].blue + src[k - dim + 1].blue + src[k - 1].blue + src[k].blue + src[k + 1].blue + src[k - 1 + dim].blue + src[k + dim].blue + src[k + dim + 1].blue) / 9;
        }
}

char smooth_descr_2[] = "smooth2";
void smooth2(int dim, pixel* src, pixel* dst)
{
    int i, j;

    for (i = 0; i < dim; i += 4)
    {
        for (j = 0; j < dim; j += 8)
        {
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
            dst[RIDX(i + 1, j, dim)] = avg(dim, i + 1, j, src);
            dst[RIDX(i + 2, j, dim)] = avg(dim, i + 2, j, src);
            dst[RIDX(i + 3, j, dim)] = avg(dim, i + 3, j, src);

            dst[RIDX(i, j + 1, dim)] = avg(dim, i, j + 1, src);
            dst[RIDX(i + 1, j + 1, dim)] = avg(dim, i + 1, j + 1, src);
            dst[RIDX(i + 2, j + 1, dim)] = avg(dim, i + 2, j + 1, src);
            dst[RIDX(i + 3, j + 1, dim)] = avg(dim, i + 3, j + 1, src);

            dst[RIDX(i, j + 2, dim)] = avg(dim, i, j + 2, src);
            dst[RIDX(i + 1, j + 2, dim)] = avg(dim, i + 1, j + 2, src);
            dst[RIDX(i + 2, j + 2, dim)] = avg(dim, i + 2, j + 2, src);
            dst[RIDX(i + 3, j + 2, dim)] = avg(dim, i + 3, j + 2, src);

            dst[RIDX(i, j + 3, dim)] = avg(dim, i, j + 3, src);
            dst[RIDX(i + 1, j + 3, dim)] = avg(dim, i + 1, j + 3, src);
            dst[RIDX(i + 2, j + 3, dim)] = avg(dim, i + 2, j + 3, src);
            dst[RIDX(i + 3, j + 3, dim)] = avg(dim, i + 3, j + 3, src);

            dst[RIDX(i, j + 4, dim)] = avg(dim, i, j + 4, src);
            dst[RIDX(i + 1, j + 4, dim)] = avg(dim, i + 1, j + 4, src);
            dst[RIDX(i + 2, j + 4, dim)] = avg(dim, i + 2, j + 4, src);
            dst[RIDX(i + 3, j + 4, dim)] = avg(dim, i + 3, j + 4, src);

            dst[RIDX(i, j + 5, dim)] = avg(dim, i, j + 5, src);
            dst[RIDX(i + 1, j + 5, dim)] = avg(dim, i + 1, j + 5, src);
            dst[RIDX(i + 2, j + 5, dim)] = avg(dim, i + 2, j + 5, src);
            dst[RIDX(i + 3, j + 5, dim)] = avg(dim, i + 3, j + 5, src);

            dst[RIDX(i, j + 6, dim)] = avg(dim, i, j + 6, src);
            dst[RIDX(i + 1, j + 6, dim)] = avg(dim, i + 1, j + 6, src);
            dst[RIDX(i + 2, j + 6, dim)] = avg(dim, i + 2, j + 6, src);
            dst[RIDX(i + 3, j + 6, dim)] = avg(dim, i + 3, j + 6, src);

            dst[RIDX(i, j + 7, dim)] = avg(dim, i, j + 7, src);
            dst[RIDX(i + 1, j + 7, dim)] = avg(dim, i + 1, j + 7, src);
            dst[RIDX(i + 2, j + 7, dim)] = avg(dim, i + 2, j + 7, src);
            dst[RIDX(i + 3, j + 7, dim)] = avg(dim, i + 3, j + 7, src);
        }
    }
}

char smooth_descr_3[] = "smooth3";
void smooth3(int dim, pixel* src, pixel* dst)
{
    int i, j, ii, jj, min_i, min_j;
    pixel_sum sum;
    pixel current_pixel;
    pixel p;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++) {
            sum.red = sum.green = sum.blue = 0;
            // sum.num = 0;
            if ((i == 0 && j == 0) || (i == 0 && j == dim - 1) || (i == dim - 1 && j == 0) || (i == dim - 1 && j == dim - 1)) {
                sum.num = 4;
            }
            else if (i == 0 || i == dim - 1 || j == 0 || j == dim - 1) {
                sum.num = 6;
            }
            else {
                sum.num = 9;
            }


            min_i = min(i + 1, dim - 1);
            min_j = min(j + 1, dim - 1);
            for (ii = max(i - 1, 0); ii <= min_i; ii++)
                for (jj = max(j - 1, 0); jj <= min_j; jj++) {
                    p = src[RIDX(ii, jj, dim)];
                    sum.red += (int)p.red;
                    sum.green += (int)p.green;
                    sum.blue += (int)p.blue;
                }
            current_pixel.red = (unsigned short)(sum.red / sum.num);
            current_pixel.green = (unsigned short)(sum.green / sum.num);
            current_pixel.blue = (unsigned short)(sum.blue / sum.num);
            dst[RIDX(i, j, dim)] = current_pixel;
        }

}

char smooth_descr_4[] = "smooth4";
void smooth4(int dim, pixel* src, pixel* dst)
{
    int i, j, ii, jj, min_i, min_j;
    pixel_sum sum;
    pixel current_pixel;
    pixel p;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++) {
            sum.red = sum.green = sum.blue = 0;

            if (!(i | j)) {  //if upper left corner
                sum.num = 4;
                sum.red = src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red
                    + src[RIDX(i, j + 1, dim)].red + src[RIDX(i + 1, j + 1, dim)].red;
            }
            else if (!i && (j == dim - 1)) {  //if upper right corner
                sum.num = 4;
                sum.red = src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red
                    + src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red;
            }
            else

                // sum.num = 0;
                if ((i == 0 && j == 0) || (i == 0 && j == dim - 1) || (i == dim - 1 && j == 0) || (i == dim - 1 && j == dim - 1))
                {
                    sum.num = 4;
                }
                else if (i == 0 || i == dim - 1 || j == 0 || j == dim - 1)
                {
                    sum.num = 6;
                }
                else
                {
                    sum.num = 9;
                }


            min_i = min(i + 1, dim - 1);
            min_j = min(j + 1, dim - 1);
            for (ii = max(i - 1, 0); ii <= min_i; ii++)
            {
                for (jj = max(j - 1, 0); jj <= min_j; jj++) {
                    p = src[RIDX(ii, jj, dim)];
                    sum.red += (int)p.red;
                    sum.green += (int)p.green;
                    sum.blue += (int)p.blue;
                }
            }
            current_pixel.red = (unsigned short)(sum.red / sum.num);
            current_pixel.green = (unsigned short)(sum.green / sum.num);
            current_pixel.blue = (unsigned short)(sum.blue / sum.num);
            dst[RIDX(i, j, dim)] = current_pixel;
        }

}

char smooth_descr_5[] = "smooth5";
void smooth5(int dim, pixel* src, pixel* dst)
{
    int i, j, tmp, tmpi;

    //UpLeft
    dst[0].red = (src[0].red + src[1].red + src[dim].red + src[dim + 1].red) >> 2;
    dst[0].blue = (src[0].blue + src[1].blue + src[dim].blue + src[dim + 1].blue) >> 2;
    dst[0].green = (src[0].green + src[1].green + src[dim].green + src[dim + 1].green) >> 2;
    //UpRight
    dst[dim - 1].red = (src[dim - 1].red + src[dim - 2].red +
        src[dim * 2 - 1].red + src[dim * 2 - 2].red) >> 2;
    dst[dim - 1].blue = (src[dim - 1].blue + src[dim - 2].blue +
        src[dim * 2 - 1].blue + src[dim * 2 - 2].blue) >> 2;
    dst[dim - 1].green = (src[dim - 1].green + src[dim - 2].green +
        src[dim * 2 - 1].green + src[dim * 2 - 2].green) >> 2;
    //DownLeft
    dst[dim * (dim - 1)].red = (src[dim * (dim - 1)].red + src[dim * (dim - 1) + 1].red +
        src[dim * (dim - 2)].red + src[dim * (dim - 2) + 1].red) >> 2;
    dst[dim * (dim - 1)].blue = (src[dim * (dim - 1)].blue + src[dim * (dim - 1) + 1].blue +
        src[dim * (dim - 2)].blue + src[dim * (dim - 2) + 1].blue) >> 2;
    dst[dim * (dim - 1)].green = (src[dim * (dim - 1)].green + src[dim * (dim - 1) + 1].green +
        src[dim * (dim - 2)].green + src[dim * (dim - 2) + 1].green) >> 2;
    //DownRight
    dst[dim * dim - 1].red = (src[dim * dim - 1].red + src[dim * dim - 2].red +
        src[dim * (dim - 1) - 1].red + src[dim * (dim - 1) - 2].red) >> 2;
    dst[dim * dim - 1].blue = (src[dim * dim - 1].blue + src[dim * dim - 2].blue +
        src[dim * (dim - 1) - 1].blue + src[dim * (dim - 1) - 2].blue) >> 2;
    dst[dim * dim - 1].green = (src[dim * dim - 1].green + src[dim * dim - 2].green +
        src[dim * (dim - 1) - 1].green + src[dim * (dim - 1) - 2].green) >> 2;
    //UpEdge
    for (j = 1; j < dim - 1; j++)
    {
        dst[j].red = (src[j].red + src[j - 1].red + src[j + 1].red +
            src[j + dim].red + src[j + 1 + dim].red + src[j - 1 + dim].red) / 6;
        dst[j].blue = (src[j].blue + src[j - 1].blue + src[j + 1].blue +
            src[j + dim].blue + src[j + 1 + dim].blue + src[j - 1 + dim].blue) / 6;
        dst[j].green = (src[j].green + src[j - 1].green + src[j + 1].green +
            src[j + dim].green + src[j + 1 + dim].green + src[j - 1 + dim].green) / 6;
    }
    //DownEdge
    for (j = dim * (dim - 1) + 1; j < dim * dim - 1; j++)
    {
        dst[j].red = (src[j].red + src[j - 1].red + src[j + 1].red +
            src[j - dim].red + src[j + 1 - dim].red + src[j - 1 - dim].red) / 6;
        dst[j].blue = (src[j].blue + src[j - 1].blue + src[j + 1].blue +
            src[j - dim].blue + src[j + 1 - dim].blue + src[j - 1 - dim].blue) / 6;
        dst[j].green = (src[j].green + src[j - 1].green + src[j + 1].green +
            src[j - dim].green + src[j + 1 - dim].green + src[j - 1 - dim].green) / 6;
    }
    //LeftEdge
    for (j = dim; j < dim * (dim - 1); j += dim)
    {
        dst[j].red = (src[j].red + src[j - dim].red + src[j + 1].red +
            src[j + dim].red + src[j + 1 + dim].red + src[j - dim + 1].red) / 6;
        dst[j].blue = (src[j].blue + src[j - dim].blue + src[j + 1].blue +
            src[j + dim].blue + src[j + 1 + dim].blue + src[j - dim + 1].blue) / 6;
        dst[j].green = (src[j].green + src[j - dim].green + src[j + 1].green +
            src[j + dim].green + src[j + 1 + dim].green + src[j - dim + 1].green) / 6;
    }
    //RightEdge
    for (j = dim + dim - 1; j < dim * dim - 1; j += dim)
    {
        dst[j].red = (src[j].red + src[j - 1].red + src[j - dim].red +
            src[j + dim].red + src[j - dim - 1].red + src[j - 1 + dim].red) / 6;
        dst[j].blue = (src[j].blue + src[j - 1].blue + src[j - dim].blue +
            src[j + dim].blue + src[j - dim - 1].blue + src[j - 1 + dim].blue) / 6;
        dst[j].green = (src[j].green + src[j - 1].green + src[j - dim].green +
            src[j + dim].green + src[j - dim - 1].green + src[j - 1 + dim].green) / 6;
    }
    //Center
    tmpi = dim;
    for (i = 1; i < dim - 1; i++)
    {
        for (j = 1; j < dim - 1; j++)
        {
            tmp = tmpi + j;
            dst[tmp].red = (src[tmp].red + src[tmp - 1].red + src[tmp + 1].red + src[tmp - dim].red + src[tmp - dim - 1].red +
                src[tmp - dim + 1].red + src[tmp + dim].red + src[tmp + dim + 1].red + src[tmp + dim - 1].red) / 9;
            dst[tmp].green = (src[tmp].green + src[tmp - 1].green + src[tmp + 1].green + src[tmp - dim].green + src[tmp - dim - 1].green +
                src[tmp - dim + 1].green + src[tmp + dim].green + src[tmp + dim + 1].green + src[tmp + dim - 1].green) / 9;
            dst[tmp].blue = (src[tmp].blue + src[tmp - 1].blue + src[tmp + 1].blue + src[tmp - dim].blue + src[tmp - dim - 1].blue +
                src[tmp - dim + 1].blue + src[tmp + dim].blue + src[tmp + dim + 1].blue + src[tmp + dim - 1].blue) / 9;
        }
        tmpi += dim;
    }
}




/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
    add_smooth_function(&smooth2, smooth_descr_2);
    add_smooth_function(&smooth3, smooth_descr_3);
    //add_smooth_function(&smooth4, smooth_descr_4);
    add_smooth_function(&smooth5, smooth_descr_5);
}

