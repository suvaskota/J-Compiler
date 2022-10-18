/*
 * token.c
 */ 

#include "token.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int defining = 0;
int for_comparison = 0;
int ifs = 0;
int elses = 0; 
int stack_made = 0; 


Stack stack;


int make_token_write(token *theToken, FILE *theFile, char* token_word) {

    if (stack_made == 0) {
        Stack_Init(&stack);
        stack_made = 1; 
    }

    if(strcmp("+", token_word) == 0) {

        theToken -> type = PLUS;
        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nADD R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("-", token_word) == 0) {
        theToken -> type = MINUS;


        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nSUB R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("*", token_word) == 0) {
        theToken -> type = MUL;


        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nMUL R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("/", token_word) == 0) {
        theToken -> type = DIV;


        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nDIV R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("%", token_word) == 0) {
        theToken -> type = DIV;

        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nMOD R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("and", token_word) == 0) {
        theToken -> type = AND;

        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nAND R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("or", token_word) == 0) {
        theToken -> type = OR;

        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nOR R1, R1, R2\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("not", token_word) == 0) {
        theToken -> type = NOT;

        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nNOT R1, R1");
    }


    else if (strcmp("if", token_word) == 0) {
        theToken -> type = IF;
        ifs = ifs + 1;
        fprintf(theFile, "%d_IF\nLDR R1, R6, #0\nADD R6, R6, #1\nCMPI R1, #0\nBRnz %d_ELSE\n", ifs, ifs);
        push(&stack, ifs, 0);
    }

    else if (strcmp("else", token_word) == 0) {
        elses = elses + 1;
        theToken -> type = ELSE; 
        int count = stack.first -> numer; 
        fprintf(theFile, "BRnzp %d_IF_FINISH\n%d_ELSE\n", count, count);
    }

    else if (strcmp("endif", token_word) == 0) {

        theToken -> type = ENDIF;
        int count = stack.first -> numer;
        Stack_Node *node = pop(&stack); 
        free(node);
        if (elses == 0) {
            fprintf(theFile, "BRnzp %d_IF_FINISH\n%d_ELSE\n", count, count);
        }
        if (elses != 0) {
            elses = elses - 1; 
        }
        fprintf(theFile, "%d_IF_FINISH\n", count);
    }

    else if (strcmp("eq", token_word) == 0) {
        theToken -> type = EQ;
        for_comparison = for_comparison + 1;

        fprintf(theFile, "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nCMP R1, R2\nBRz EQ_%d", for_comparison - 1); 
        fprintf(theFile, "\nCONST R0, #0\nSTR R0, R6, #-1\nADD R6, R6, #-1\nJMP EQ_END_%d\nEQ_%d", for_comparison - 1, for_comparison - 1);
        fprintf(theFile, "\nCONST R0, #1\nSTR R0, R6, #-1\nADD R6, R6, #-1\nEQ_END_%d\n", for_comparison - 1);
    }

    else if (strcmp("gt", token_word) == 0) {
        theToken -> type = GT;
        for_comparison = for_comparison + 1; 

        fprintf(theFile, "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nCMP R1, R2\nBRp GT_%d", for_comparison - 1); 
        fprintf(theFile, "\nCONST R0, #0\nSTR R0, R6, #-1\nADD R6, R6, #-1\nJMP GT_END_%d\nGT_%d", for_comparison - 1, for_comparison - 1);
        fprintf(theFile, "\nCONST R0, #1\nSTR R0, R6, #-1\nADD R6, R6, #-1\nGT_END_%d\n", for_comparison - 1);
    }

    else if (strcmp("ge", token_word) == 0) {
        theToken -> type = GE;
        for_comparison = for_comparison + 1; 

        fprintf(theFile, "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nCMP R1, R2\nBRzp GE_%d", for_comparison - 1); 
        fprintf(theFile, "\nCONST R0, #0\nSTR R0, R6, #-1\nADD R6, R6, #-1\nJMP GE_END_%d\nGE_%d", for_comparison - 1, for_comparison - 1);
        fprintf(theFile, "\nCONST R0, #1\nSTR R0, R6, #-1\nADD R6, R6, #-1\nGE_END_%d\n", for_comparison - 1);
    }

    else if (strcmp("lt", token_word) == 0) {
        theToken -> type = LT;
        for_comparison = for_comparison + 1;

        fprintf(theFile, "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nCMP R1, R2\nBRnz LE_%d", for_comparison - 1); 
        fprintf(theFile, "\nCONST R0, #0\nSTR R0, R6, #-1\nADD R6, R6, #-1\nJMP LE_END_%d\nLE_%d", for_comparison - 1, for_comparison - 1);
        fprintf(theFile, "\nCONST R0, #1\nSTR R0, R6, #-1\nADD R6, R6, #-1\nLE_END_%d\n", for_comparison - 1);
    }

    else if (strcmp("le", token_word) == 0) {
        theToken -> type = LE;
        for_comparison = for_comparison + 1;

        fprintf(theFile, "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nCMP R1, R2\nBRn LT_%d", for_comparison - 1); 
        fprintf(theFile, "\nCONST R0, #0\nSTR R0, R6, #-1\nADD R6, R6, #-1\nJMP LT_END_%d\nLT_%d", for_comparison - 1, for_comparison - 1);
        fprintf(theFile, "\nCONST R0, #1\nSTR R0, R6, #-1\nADD R6, R6, #-1\nLT_END_%d\n", for_comparison - 1);

    }


    else if (strcmp("return", token_word) == 0) {
        theToken -> type = RETURN;
        fprintf(theFile, "%s", "LDR R7, R6, #0\nSTR R7, R5, #2\nADD R6, R5, #0\nLDR R5, R6, #0\nLDR R7, R6, #1\nADD R6, R6, #3\nJMPR R7\n");
    }

    else if (strcmp("defun", token_word) == 0) {

        theToken -> type = DEFUN;
        defining = 1; 
    }

    else if (strcmp("rot", token_word) == 0) {
        theToken -> type = ROT;
        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nLDR R3, R6, #0\nADD R6, R6, #1\nSTR R2, R6, #-1\nADD R6, R6, #-1\nSTR R1, R6, #-1\nADD R6, R6, #-1\nSTR R3, R6, #-1\nADD R6, R6, #-1\n");
        
    }

    else if (strcmp("dup", token_word) == 0) {
        theToken -> type = DUP;
        fprintf(theFile, "%s", "LDR R1, R6, #0\nSTR R1, R6, #-1\nADD R6, R6, #-1\n");
    }

    else if (strcmp("swap", token_word) == 0) {
        theToken -> type = SWAP;
        fprintf(theFile, "%s", "LDR R1, R6, #0\nADD R6, R6, #1\nLDR R2, R6, #0\nADD R6, R6, #1\nSTR R1, R6, #-1\nADD R6, R6, #-1\nSTR R2, R6, #-1\nADD R6, R6, #-1\n");
    }

     else if (strcmp("drop", token_word) == 0) {
        theToken -> type = DROP;
        fprintf(theFile, "%s", "ADD R6, R6, #1\n");
    }

    else if (token_word[0] == 'a' && token_word[1] == 'r' && token_word[2] == 'g') {
        int final;

        if (strlen(token_word) == 5) {

            theToken -> type = ARG;
            char number_storer[10]; 
            number_storer[0] = token_word[3];
            number_storer[1] = token_word[4];
            number_storer[2] = '\0';
            final = atoi(number_storer) + 2;
            theToken -> arg_no = final - 2;

        } else {
 
            theToken -> type = ARG;
            char number_storer[10]; 
            number_storer[0] = token_word[3];
            number_storer[1] = '\0';
            final = atoi(number_storer) + 2;
            theToken -> arg_no = final - 2;
        }
        fprintf(theFile, "LDR R3 R5 #%d\nSTR R3 R6 #-1\nADD R6 R6 #-1\n", final);
    }

    else if (token_word[0] == '0' && token_word[1] == 'x') {
        theToken -> type = LITERAL;
        int actual_number;
        sscanf(token_word, "%X", &actual_number);
        theToken -> literal_value = actual_number;

        if(actual_number <= 255 && actual_number >= -256) {
           fprintf(theFile, "CONST R3, #%d\nSTR R3, R6, #-1\nADD R6, R6, #-1\n", actual_number);
       } else {
            int top;
            int bottom;

            bottom = actual_number & 255; 
            top = (actual_number >> 8) & 255;

            fprintf(theFile, "CONST R3, #%d\nHICONST R3, #%d\nSTR R3, R6, #-1\nADD R6, R6, #-1\n", bottom, top);
       }
    }


    else if (isdigit(token_word[0]) != 0 || (isdigit(token_word[1]) != 0 )) {
        theToken -> type = LITERAL;
        theToken -> literal_value = atoi(token_word);

        if(atoi(token_word) <= 255 && atoi(token_word) >= -256) {
           fprintf(theFile, "CONST R3, #%d\nSTR R3, R6, #-1\nADD R6, R6, #-1\n", atoi(token_word));
       } else {
            int top;
            int bottom;

            bottom = atoi(token_word) & 255; 
            top = (atoi(token_word) >> 8) & 255;

            fprintf(theFile, "CONST R3, #%d\nHICONST R3, #%d\nSTR R3, R6, #-1\nADD R6, R6, #-1\n", bottom, top);

       }
    }


    else if (defining == 1){


        defining = 0;
        theToken -> type = IDENT;

        fprintf(theFile, ".CODE\n.FALIGN\n\n%s\n;;Prologue\nADD R6, R6, #-3\nSTR R7, R6, #1\nSTR R5, R6, #0\nADD R5, R6, #0\n;;Function Body\n", token_word);

    }

    else if (defining == 0) {
        theToken -> type = IDENT;

        fprintf(theFile, "JSR %s\nADD R6, R6, #-1\n", token_word);
    }


    return 0;
}