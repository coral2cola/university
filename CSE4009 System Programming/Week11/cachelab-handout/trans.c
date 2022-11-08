//2018008531

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    #define min(a,b) ((a)<(b)?(a):(b))
    #define loop(i,l,r) for(i=(l);i<(r);++i)
    #define loopd(i,l,r,d) for(i=(l);i<(r);i+=(d))
    #define bloop(i,l,r) for(i=(r)-1;i>=(l);--i)
    #define bloopd(i,l,r,d) for(i=(r)-1;i>=(l);i-=(d))

    int x, y, i, j, a[8];
    loopd(x, 0, N, 8) loopd(y, 0, M, 8)
        if (x != y || x + 8 > min(N, M) || y + 8 > min(N, M)) {
            loop(j, y, min(M, y + 8)) a[j - y] = A[x][j];
            loop(j, y, min(M, y + 4)) B[j][x] = a[j - y];

            loop(i, x + 1, min(N, x + 8)) {
                loop(j, y, min(M, y + 4)) a[j - y] = A[i][j];
                loop(j, y, min(M, y + 4)) B[j][i] = a[j - y];
            }
            y += 4;
            bloop(i, x + 1, min(N, x + 8)) {
                loop(j, y, min(M, y + 4)) a[j - y] = A[i][j];
                loop(j, y, min(M, y + 4)) B[j][i] = a[j - y];
            }
            loop(j, y, min(M, y + 4)) B[j][x] = a[4 + j - y];
            y -= 4;
        } else {
            a[0] = x; x = y; y = a[0];

            loop(i, 0, 4) {
                loop(j, 0, 8) a[j] = A[y + i][x + j];
                loop(j, 0, 8) B[x + i][y + j] = a[j];
            }

            loop(i, 0, 4) loop(j, i + 1, 4) {
                a[0] = B[x + i][y + j];
                B[x + i][y + j] = B[x + j][y + i];
                B[x + j][y + i] = a[0];
            }

            y += 4;
            loop(i, 0, 4) loop(j, i + 1, 4) {
                a[0] = B[x + i][y + j];
                B[x + i][y + j] = B[x + j][y + i];
                B[x + j][y + i] = a[0];
            }
            y -= 4;
 
            loop(i, 4, 8) {
                loop(j, 0, 8) a[j] = A[y + i][x + j];
                loop(j, 0, 8) B[x + i][y + j] = a[j];
            }
 
            x += 4;
            loop(i, 0, 4) loop(j, i + 1, 4) {
                a[0] = B[x + i][y + j];
                B[x + i][y + j] = B[x + j][y + i];
                B[x + j][y + i] = a[0];
            }

            y += 4;
            loop(i, 0, 4) loop(j, i + 1, 4) {
                a[0] = B[x + i][y + j];
                B[x + i][y + j] = B[x + j][y + i];
                B[x + j][y + i] = a[0];
            }
            y -= 4;
            x -= 4;

            loop(i, 0, 4) {
                loop(j, 0, 4) a[j] = B[x + 4 + i][y + j];
                loop(j, 4, 8) a[j] = B[x + i][y + j];
                loop(j, 4, 8) B[x + i][y + j] = a[j - 4];
                loop(j, 0, 4) B[x + 4 + i][y + j] = a[j + 4];
            }
            a[0] = x; x = y; y = a[0];
        }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     bloopformance. This is a handy way to exbloopiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helbloop function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

