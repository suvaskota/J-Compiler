/*
 * stack.h
 * 
 */


#include <stdbool.h>

typedef struct stack_node_st {
	int numer;
	int denom;

	struct stack_node_st *next;

} Stack_Node;

typedef struct stack_st {
	Stack_Node *first;

} Stack; 


void Stack_Init(Stack *s);

bool empty(Stack *s);

void push(Stack *s, int numer, int denom);

Stack_Node* pop(Stack *s);

void duplicate(Stack *s);

bool swap(Stack *s);

void clear(Stack *s);

void print(Stack *s);

int gcd(int numerator, int denominator);