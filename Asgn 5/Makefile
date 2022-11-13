ass=asgn5
roll=20CS10002_20CS10072
name=$(ass)_$(roll)

a.out: lexer $(name).tab.o $(name)_translator.o
	g++ lexer $(name).tab.o $(name)_translator.o -lfl

$(name)_translator.o: $(name)_translator.h $(name)_translator.cxx
	g++ -c $(name)_translator.h
	g++ -c $(name)_translator.cxx

lexer: lex.yy.c
	g++ -c lex.yy.c -o lexer

$(name).tab.o : $(name).tab.c
	g++ -c $(name).tab.c

lex.yy.c: $(name).l $(name).tab.h $(name)_translator.h
	flex $(name).l

$(name).tab.c $(name).tab.h: $(name).y
	bison -dtv $(name).y

test:
	./a.out <t1.c> result1.txt
	./a.out <t2.c> result2.txt
	./a.out <t3.c> result3.txt
	./a.out <t4.c> result4.txt
	./a.out <t5.c> result5.txt

clean:
	rm -f lex* *.tab.* *.output *.o a.out *.txt *.h.gch