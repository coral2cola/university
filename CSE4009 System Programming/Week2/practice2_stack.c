//Template
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int nData;
	struct Node *next;
}Node;

typedef struct Stack{
	Node *top;
}Stack;

void InitializeStack(Stack *stack);
void Push(Stack *stack, int nData);
int Pop(Stack *stack);
void PrintStack(Stack* stack);


int main(void){
	//source code
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	InitializeStack(stack);

	Push(stack, 1);
	Push(stack, 2);
	Push(stack, 3);
	Push(stack, 4);
	Push(stack, 5);

	Pop(stack);
	Pop(stack);

	PrintStack(stack);

	free(stack);

	return 0;
}

void InitializeStack(Stack *stack){
	//source code
	stack->top = NULL;
	printf("stack 초기화\n");
}

void Push(Stack *stack, int nData){
	//source code
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->nData = nData;
	temp->next = stack->top;
	stack->top = temp;

	printf("stack에 %d 삽입\n", nData);
}

int Pop(Stack *stack){
	//source code
	Node* temp = stack->top;
	int data = temp->nData;
	stack->top = temp->next;
	free(temp);
	printf("stack에서 %d 제거\n", data);
	return data;
}

void PrintStack(Stack* stack)
{
	printf("stack 내부의 데이터 : ");

	Node* ptr = stack->top;
	while (ptr != NULL)
	{
		printf("%d ", ptr->nData);
		ptr = ptr->next;
	}
	printf("\n");
}
