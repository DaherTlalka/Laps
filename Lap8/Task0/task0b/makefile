all: ELF

ELF: ELF.o
	gcc -m32 -fno-pie -g -Wall -o ELF ELF.o 
	rm *.o

ELF.o: ELF.c
	gcc -m32 -fno-pie -g -Wall -c -o ELF.o ELF.c

.PHONY: clean
clean:
	rm -f *.o ELF