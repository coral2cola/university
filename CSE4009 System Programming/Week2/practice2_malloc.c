#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


int main(void){
	char** pp_data = NULL;
	int n;
	int length; //최대길이
	int i;

	printf("문자 포인터의 개수를 입력하세요 : ");
	scanf("%d", &n);

	//1. define & dynamic allocation
	pp_data = (char**)malloc(sizeof(char*) * n);

	printf("입력할 문자의 최대 길이를 입력하세요 : ");
	scanf("%d", &length);

	printf("\n");

	for (int i = 0; i < n; i++)
	{ 
		*(pp_data + i) = (char*)malloc(sizeof(char) * length+1);
		printf("%d번째 단어를 입력하세요\n", i + 1);
		scanf("%s", *(pp_data + i));
	}
	//2. print string

	printf("\n***** 입력된 문자 *****\n");
	for (int i = 0; i < n; i++)
	{
		printf("%s\n", *(pp_data + i));
	}

	//3. free memory

	for (int i = 0; i < n; i++)
	{
		free(*(pp_data + i));
	}
	free(pp_data);

	return 0;
}
