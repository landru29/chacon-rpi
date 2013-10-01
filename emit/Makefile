all : emit

emit : emit.o buffer.o home_easy.o utils.o
	gcc -o $@ $^ -lwiringPi

emit.o : emit.c
	gcc -c $< -o $@

%.o : %.c %.h
	gcc -c $< -o $@

clean:
	rm -f emit *.o
