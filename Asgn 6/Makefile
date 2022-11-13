ass=asgn6
roll=20CS10002_20CS10072
name=$(ass)_$(roll)

tinyC: lex.yy.o $(name).tab.o $(name)_translator.o $(name)_target_translator.o
	g++ lex.yy.o $(name).tab.o $(name)_translator.o $(name)_target_translator.o -lfl -o tinyC

$(name)_target_translator.o: $(name)_target_translator.cxx
	g++ -c $(name)_target_translator.cxx

$(name)_translator.o: $(name)_translator.h $(name)_translator.cxx
	g++ -c $(name)_translator.h
	g++ -c $(name)_translator.cxx

lex.yy.o: lex.yy.c
	g++ -c lex.yy.c

$(name).tab.o: $(name).tab.c
	g++ -c $(name).tab.c

lex.yy.c: $(name).l $(name).tab.h $(name)_translator.h
	flex $(name).l

$(name).tab.h $(name).tab.c: $(name).y
	bison -dtv $(name).y

libSelfIO.a: $(name).o
	ar -rcs libSelfIO.a $(name).o

$(name).o: $(name).c myl.h
	gcc -c $(name).c

$(name).c:
	touch $(name).c

test: tinyC libSelfIO.a
	
	@mkdir -p Quads
	@mkdir -p Assembly
	@mkdir -p Object
	@mkdir -p Executable

	@for i in 1 2 3 4 5; \
		do echo "Compiling Testfile$$i"; \
		./tinyC $$i < Testing/Test$$i.c > Quads/Quad$$i.txt; \
		mv asm_$$i.s Assembly/asm_$$i.s; \
		gcc -c Assembly/asm_$$i.s -o Object/Obj_$$i.o; \
		gcc Object/Obj_$$i.o -o Executable/Test$$i -L. -lSelfIO -no-pie; \
	done;

run:
	@for i in 1 2 3 4 5; \
	do echo "\n\nTestCase $$i"; \
	./Executable/Test$$i; \
	done;

clean:
	rm -f lex.yy.c *.tab.c *.tab.h *.output *.o *.s *.a *.out *.gch tinyC Quads/* Assembly/* Object/* Executable/* 
	rmdir Quads/ Assembly/ Object/ Executable/ 
