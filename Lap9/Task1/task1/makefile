
all: Task1

Task1: skeleton.o
	ld -m elf_i386 skeleton.o -o Task1

skeleton.o: skeleton.s
	nasm -f elf skeleton.s -o skeleton.o

clean:
	rm *.o Task1