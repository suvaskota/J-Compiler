/*
 * queue.c
 * 
 */

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>


void Stack_Init(Stack *s) {
	s -> first = NULL; 
}

bool empty(Stack *s) {
	if (s -> first == NULL) {
		return true; 
	}

	return false;
}

void push(Stack *s, int numer, int denom) {

	Stack_Node *node = malloc(sizeof(Stack_Node));

	if (node == NULL) {
		printf("Memory Allocation has failed");
		exit(EXIT_FAILURE); 
	}

	node -> numer = numer;
	node -> denom = denom; 
	node -> next = s -> first; 

	s -> first = node; 
}

Stack_Node* pop(Stack *s) {

	Stack_Node *node = s -> first; 
	s -> first  = s -> first -> next;

	return node;
}

void duplicate(Stack *s) {

	int numerator; 
	int denominator; 

	numerator = s -> first -> numer; 
	denominator = s -> first -> denom;

	push(s, numerator, denominator);
}


bool swap(Stack *s) {

	if (s -> first == NULL) {
		return false; 
	}

	if (s -> first -> next == NULL) {
		return false; 
	}

	Stack_Node *node = s -> first -> next;


	s -> first -> next = node -> next; 
	node -> next = s -> first;
	s -> first = node; 


	return true;
}

void clear(Stack *s) {

	while(!empty(s)) {
		Stack_Node *node = pop(s);
		free(node);
	}
}

void print(Stack *s) {
	Stack_Node *current = s -> first;
	printf("\nSTACK CONTAINS\n"); 

	while(current) {
		printf("%d/%d\n", current -> numer, current -> denom);
		current = current -> next; 
	}
}

int gcd(int numerator, int denominator) {
	if (numerator == 0) {
		return denominator; 
	}

	if (denominator == 0) {
		return numerator; 
	}

	if (numerator > denominator) {
		return gcd((numerator % denominator), denominator);
	} else {
		return gcd(numerator, (denominator % numerator));
	}
}


