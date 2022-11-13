output.txt: MyParser asgn4_20CS10072_20CS10002_test.c
		./MyParser < asgn4_20CS10072_20CS10002_test.c > output.txt

MyParser: lexer y.tab.o main.o
		gcc lexer y.tab.o main.o -lfl -o MyParser

main.o: asgn4_20CS10072_20CS10002.c
		gcc -c asgn4_20CS10072_20CS10002.c -o main.o

lexer: lex.yy.c
		gcc -c lex.yy.c -o lexer

y.tab.o: y.tab.c
		gcc -c y.tab.c -o y.tab.o

lex.yy.c: asgn4_20CS10072_20CS10002.l y.tab.h
		flex asgn4_20CS10072_20CS10002.l

y.tab.c y.tab.h: asgn4_20CS10072_20CS10002.y
		yacc -dty --report=all asgn4_20CS10072_20CS10002.y 

clean:
	rm output.txt MyParser main.o lex.yy.c lexer y.tab.c y.tab.h y.tab.o y.output