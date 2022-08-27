all: task1

task1: task1.o
	gcc -m32 -fno-pie -g -Wall -o task1 task1.o 
	rm *.o

task1.o: task1.c
	gcc -m32 -fno-pie -g -Wall -c -o task1.o task1.c

.PHONY: clean
clean:
	rm -f *.o task1