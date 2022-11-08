//Template
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int compare(int a, int b){
    if(a>b)  
    	return 1;
    else 
       	return -1;
}
    

/***************************************************************
define bubble sort that uses pointer to 'compare' function above
***************************************************************/


int main(){
    int n_array[SIZE] = { 2,-9, 10, 15, 1, 3, -12, 5, 4, 1 };

    printf("정렬 전 : ");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", n_array[i]);
    printf("\n");

    int (*fp)(int, int);
    fp = compare;


    while (1)
    {
        for (int i = 0; i < SIZE - 1; i++)
        {
            if (fp(n_array[i], n_array[i + 1]) == 1)
            {
                int temp = n_array[i];
                n_array[i] = n_array[i + 1];
                n_array[i + 1] = temp;
            }
        }

        int sum = 0;
        for (int i = 0; i < SIZE - 1; i++)
        {
            sum += fp(n_array[i], n_array[i + 1]);
        }

        if (sum == -1 * (SIZE - 1))
            break;
    }

    printf("정렬 후 : ");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", n_array[i]);

    return 0;
}
