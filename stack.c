#include "stack.h"
#include "stdio.h"
#include "stdlib.h"

void stCreate(Stack *stack)
{
	stack->sp = -1;
}

void stPush(Stack *stack, int t)
{
	if(stIsFull(stack))
	{
		printf("Fatal error: Nested procedure declaration is too deep!\n");
		exit(0);
		return;
	}
	stack->element[++stack->sp] = t;
}

int	stPop(Stack *stack)
{
	if(stIsEmpty(stack))
		return -1;
	return stack->element[stack->sp--];
}

int	stIsEmpty(Stack *stack)
{
	return (stack->sp == -1);
}

int	stIsFull(Stack *stack)
{
	return (stack->sp == MAX_STACK_SIZE-1);
}

int	stTop(Stack *stack)
{
	if(stIsEmpty(stack))
		return -1;
	return (stack->element[stack->sp]);
}

void stDump(Stack *stack)
{
	if(stIsEmpty(stack))
		return;

	int i;
	for(i=0; i<=stack->sp; ++i)
		printf("| %d\t\t\t|\n", stack->element[i]);
}
