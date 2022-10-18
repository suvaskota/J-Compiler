/*
 * parser.c
 */

#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    
	if (argc < 2) {

        printf("Invalid Arguments\n");
        return 1;
    }

    int i; 
    FILE *out_file;
    FILE *in_file; 
	char outputFile[100]; 
    char inputFile[100];
    char row[300];
    char *strings;


 
    strcpy(inputFile, argv[1]);
    i = strlen(argv[1]);


    strncpy(outputFile, argv[1], i-1);
    outputFile[i-1] = '\0';
    strcat(outputFile, "asm");


    in_file = fopen(inputFile, "r");
    out_file = fopen(outputFile, "w");


    while(!feof(in_file)) {

    	fgets(row, 300, in_file);

    	strings = strtok(row, " \t");
    	while(strings) {

    		if (strcmp(";", strings) == 0 || strcmp(";;", strings) == 0 || strcmp(";;\n", strings) == 0 || strcmp("\n", strings) == 0 || strcmp(";\n", strings) == 0) {
    			break;
    		}

            char *letter = strchr(strings, '\n');
            if (letter) {
                *letter = '\0';
            }

            token someToken; 
            token *theToken = &someToken; 
    		make_token_write(theToken, out_file, strings);
    		strings = strtok(NULL, " \t");
    	}
    }

    fclose(in_file);
    fclose(out_file);
}