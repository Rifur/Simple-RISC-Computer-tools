#ifndef __STACK__H__
#define __STACK__H__

#define MAX_STACK_SIZE 512

typedef struct{
	int element[MAX_STACK_SIZE];
	int sp;
} Stack;

void	stCreate(Stack *stack);	// initial the stack
void	stPush(Stack *stack, int t);	// push()
int	stPop(Stack *stack);		// pop()
int	stIsEmpty(Stack *stack);	// return true if the stack is empty
int	stIsFull(Stack *stack);	// return true if the stack is full
int	stTop(Stack *stack);	// return the top of stack
void	stDump(Stack *stack);	// dump the stack 


#endif
