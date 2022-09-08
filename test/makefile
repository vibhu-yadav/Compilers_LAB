ass2: main.o libmyl.a
	cc main.o -L. -lmyl -o myl

main.o: main.c myl.h
	cc -Wall -c main.c

libmyl.a: myl.o
	ar -rcs libmyl.a myl.o

myl.o: myl.c myl.h
	cc -Wall -c myl.c

clean:
	rm myl main.o libmyl.a myl.o