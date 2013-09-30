all : test

test : test.o emitlib.o
	gcc -o test test.o emitlib.o

test.o : test.c
	gcc -c test.c -o test.o

emitlib.o : emitlib.c emitlib.h
	gcc -c ../emitlib.c -o emitlib.o

clean:
	rm -f test *.o
