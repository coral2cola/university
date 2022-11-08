#include "pch.h"

#define I 2

float muller(float (*func)(float), float f1, float f2, float f3, float xacc)
{
    int i, itr, maxmitr;
    float x[4], li, di, mu, s, l, allerr;
    float (*f)(float x);
    f = func;

    x[0] = f1;
    x[1] = f2;
    x[2] = f3;

    allerr = xacc;
    maxmitr = 20;
    for (itr = 1; itr <= maxmitr; itr++)
    {
        li = (x[I] - x[I - 1]) / (x[I - 1] - x[I - 2]);
        di = (x[I] - x[I - 2]) / (x[I - 1] - x[I - 2]);
        mu = f(x[I - 2]) * li * li - f(x[I - 1]) * di * di + f(x[I]) * (di + li);
        s = sqrt((mu * mu - 4 * f(x[I]) * di * li * (f(x[I - 2]) * li - f(x[I - 1]) * di + f(x[I]))));
        if (mu < 0)
            l = (2 * f(x[I]) * di) / (-mu + s);
        else
            l = (2 * f(x[I]) * di) / (-mu - s);
        x[I + 1] = x[I] + l * (x[I] - x[I - 1]);
        //printf("At iteration no. %2d, x = %7.6f\n", itr, x[I + 1]);
        if (fabs(x[I + 1] - x[I]) < allerr)
        {
            //printf("After %3d iterations, the required root is %6.4f\n", itr, x[I + 1]);
            return x[I + 1];
        }
        for (i = I - 2; i < 3; i++)
            x[i] = x[i + 1];
    }
    printf("The required solution does not converge or iterations are insufficient\n");
    return 1;
}

#undef MAXIT