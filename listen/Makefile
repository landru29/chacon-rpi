all : listen

listen : listen.o listenlib.o
	gcc -o listen listen.o listenlib.o -lwiringPi

listen.o : listen.c
	gcc -c listen.c

listenlib.o : listenlib.c listenlib.h
	gcc -c listenlib.c
