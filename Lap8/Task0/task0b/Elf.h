#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
//
// mmap is to acsses data for that we send it the mapstat
// struct
typedef struct state
{
    char debug;
    char file_name[128];
    int opened;
    int fd;
    void *start_map;
    struct stat fd_stat;
    Elf32_Ehdr *header;
    int numberSectionHeaders;
} state;
//
struct fun_desc
{
    char *name;
    void (*fun)(state *st);
} fun_desc;


void toggle_debug_mode(state *st);
void Examine_ELF_File(state *st);
void Print_Section_Names(state *st);
void Print_Symbols(state *st);
void quit(state *st);
void printingElf(state *st);