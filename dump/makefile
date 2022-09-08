output.txt: lexer ass3_20CS10072_20CS10002_test.c
			./lexer < ass3_20CS10072_20CS10002_test.c > output.txt

lexer: lex.yy.c 
		cc lex.yy.c -lfl -o lexer

lex.yy.c: ass3_20CS10072_20CS10002.l
		flex ass3_20CS10072_20CS10002.l ass3_20CS10072_20CS10002.c

clean:
		rm lex.yy.c lexer output.txt 
