all : emit

emit : emit.o emitlib.o
	gcc -o emit emit.o emitlib.o -lwiringPi

emit.o : emit.c
	gcc -c emit.c

emitlib.o : emitlib.c emitlib.h
	gcc -c emitlib.c
