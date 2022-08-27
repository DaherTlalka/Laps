all: menu_map
menu_map: ./menu_map.o
	gcc -Wall -m32 -g -o menu_map ./menu_map.o
	
menu_map.o: ./menu_map.c
	gcc -Wall -m32 -c -g -o menu_map.o ./menu_map.c
.PHONY: clean
clean: rm -f *.o menu_map
