#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


int main(void){
	char** pp_data = NULL;
	int n;
	int length; //�ִ����
	int i;

	printf("���� �������� ������ �Է��ϼ��� : ");
	scanf("%d", &n);

	//1. define & dynamic allocation
	pp_data = (char**)malloc(sizeof(char*) * n);

	printf("�Է��� ������ �ִ� ���̸� �Է��ϼ��� : ");
	scanf("%d", &length);

	printf("\n");

	for (int i = 0; i < n; i++)
	{ 
		*(pp_data + i) = (char*)malloc(sizeof(char) * length+1);
		printf("%d��° �ܾ �Է��ϼ���\n", i + 1);
		scanf("%s", *(pp_data + i));
	}
	//2. print string

	printf("\n***** �Էµ� ���� *****\n");
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
