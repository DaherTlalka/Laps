all: execute

execute: LineParser.o task1.o task2.o task3.o
	gcc -m32 -Wall -g LineParser.o task1.o -o task1
	gcc -m32 -Wall -g LineParser.o task2.o -o task2
	gcc -m32 -Wall -g LineParser.o task3.o -o task3
./LineParser.o:
	gcc -m32 -c -Wall -g LineParser.c -o LineParser.o
./task1.o:
	gcc -m32 -c -Wall -g task1.c -o task1.o
./task2.o:
	gcc -m32 -c -Wall -g task2.c -o task2.o
./task3.o:
	gcc -m32 -c -Wall -g task3.c -o task3.o

.PHONY: clean
clean:
	rm -f *.o execute

