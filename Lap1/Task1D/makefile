all: encoder
encoder: ./encoder.o
	gcc -Wall -m32 -g -o encoder ./encoder.o
	
encoder.o: ./encoder.c
	gcc -Wall -m32 -c -g -o encoder.o ./encoder.c
.PHONY: clean
clean: rm -f *.o encoder

