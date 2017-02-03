#Makefile

all: clock

clock: clock.c
	g++ -Wall -o clock clock.c -lwiringPi

clean:
	rm clock

