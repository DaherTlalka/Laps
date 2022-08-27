all: task2

task1: task2.o
	gcc -m32 -fno-pie -g -Wall -o task2 task2.o 
	rm *.o

task1.o: task2.c
	gcc -m32 -fno-pie -g -Wall -c -o task2.o task2.c

.PHONY: clean
clean:
	rm -f *.o task2