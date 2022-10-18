/*
 * token.h
 */

#define MAX_TOKEN_LENGTH 250


#include <stdio.h>

typedef enum { DEFUN, IDENT, RETURN, 
	       PLUS, MINUS, MUL, DIV, MOD, 
	       AND, OR, NOT, 
	       LT, LE, EQ, GE, GT,
	       IF, ELSE, ENDIF, 
	       DROP, DUP, SWAP, ROT,
	       ARG,
	       LITERAL,
	       BAD_TOKEN } token_type;

typedef struct {
  token_type type;
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH];  // This character array stores the token string
} token;

// line_number : You can use this to keep track of the current line number which can be useful in reporting syntax errors.
// Be sure to include a line that says 'int line_number;' in token.c outside of any functions if you would like to make use of this.
extern int line_number;

//int read_token (token *theToken, FILE *theFile);

int make_token_write(token *theToken, FILE *theFile, char* token_word);

