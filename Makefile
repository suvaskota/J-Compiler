jc: token.c parser.c stack.c
	clang -Wall -g -o jc token.c parser.c stack.c

clean: 
	rm -f jc