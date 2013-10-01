all : test

test : test.o buffer.o home_easy.o utils.o
	gcc -o $@ $^ -lwiringPi

test.o : test.c
	gcc -c $< -o $@

%.o : ../%.c ../%.h
	gcc -c $< -o $@

clean:
	rm -f test *.o
