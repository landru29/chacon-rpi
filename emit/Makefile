all : emit listen

listen : listen.o buffer.o home_easy.o utils.o analyze.o
	gcc -o $@ $^ -lwiringPi

emit : emit.o buffer.o home_easy.o utils.o
	gcc -o $@ $^ -lwiringPi

listen.o : listen.c
	gcc -c $< -o $@

emit.o : emit.c
	gcc -c $< -o $@

%.o : %.c %.h
	gcc -c $< -o $@

clean:
	rm -f emit *.o

install: emit
	cp -f emit /usr/local/bin/.
