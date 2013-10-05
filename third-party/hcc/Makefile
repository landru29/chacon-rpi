all: radioEmission

radioEmission: radioEmission.o
	gcc -o $@ $^ -lwiringPi

%.o: %.cpp
	gcc -c -o $@ $<
