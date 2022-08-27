/*----------------------------------------------------------------------------
|
| Module Details: task 1 - Lab 8
|
| Name: task1.c
|
| Purpose: Print the readelf.
|
| Date      	Name Ver. Comments
| --------- ------- ----- ----- -----------------------------------------
|
----------------------------------------------------------------------------*/
#include "task1.h"
//
//
struct fun_desc menu[] = {{"1)Toggle Debug Mode", toggle_debug_mode}, {"2)Examine ELF File", Examine_ELF_File}, {"3)Print Section Names", Print_Section_Names}, {"4)Print Symbols", Print_Symbols}, {"5)Quit", quit}, {NULL, NULL}};
//
int main(int argc, char **argv)
{
    state *st = calloc(1, sizeof(state));
    int op;
    st->opened = 0;
    while (1)
    {
        fprintf(stderr, "Choose action:\n");
        for (int i = 0; i < 5; i++)
        {
            printf("%s\n", menu[i].name);
        }
        scanf("%d", &op);
        if(op==5){
            menu[op-1].fun(st);
        }
        else if (op > 3 || op < 0)
            fprintf(stderr, "not implemented yet\n");
        else
        {
            menu[op-1].fun(st);
        }
    }
    return 0;
}

/*---------------------------------------------------------------------------*\
| Function: toggle_debug_mode::ELF.h
| Purpose: check the debug and change it 
| 
| Input: 
|         state *st: state of the file 
|           
| Output: change the debug flag. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/

void toggle_debug_mode(state *st)
{
      if (debu == 0)
    {
        fprintf(stderr, "Debug flag now on\n");
        debu=1;
    }
    else
    {
        fprintf(stderr, "Debug flag now off\n");
        debu = 0;
    }
}

/*---------------------------------------------------------------------------*\
| Function: Examine_ELF_File::ELF.h
| Purpose: printing an information like readelf
| 
| Input: 
|         state *st: state of the file 
|           
| Output: printing informtions of the file. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void Examine_ELF_File(state *st)
{
    printf("Enter You'r File pleas:");
    scanf("%s", st->file_name);
    st->opened = 1;
    if ((st->fd = open(st->file_name, O_RDWR)) < 0)
    {
        printf("there is an error of open this file\n");
        exit(0);
        return; // think of break
    }
    if (fstat(st->fd, &st->fd_stat) != 0)
    {
        perror("stat failed");
        exit(-1);
        return;
    }
    if ((st->start_map = mmap(0, st->fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, st->fd, 0)) == MAP_FAILED)
    {
        perror("there is problem in mmap \n");
        exit(-4);
        return;
    }
    st->header = (Elf32_Ehdr *)st->start_map;
    if (strncmp((char *)st->header->e_ident + 1, "ELF", 3) != 0)
    {
        perror("THis file is'n a ELF file\n\n");
        close(st->fd);
        st->fd = -1;
        munmap(st->start_map, st->fd_stat.st_size); // terminate  all the mapping state
        return;
    }
    // first printing the magic
    printf("%c %c %c\n", st->header->e_ident[1], st->header->e_ident[2], st->header->e_ident[3]); // 1
    printingElf(st);
}

/*---------------------------------------------------------------------------*\
| Function:  Print_Section_Names::ELF.h
| Purpose: printing the section headers
| 
| Input: 
|         state *st: state of the file 
|           
| Output: printing the section headers and the print look like the readelf
| Return: VOID. 
\*---------------------------------------------------------------------------*/

void Print_Section_Names(state *st)
{
   if(st->opened==0){
       printf("current fd is invalid,pleac try agine\n");
       return;
   }
    printf("Section Headers:\n");
    printf("[Nr] Name                Type             Addr     Off    Size");
    if(debu==1)
        printf("     OffsetName");
    printf("\n");
   st->start_map=mmap(0,st->fd_stat.st_size,PROT_READ,MAP_PRIVATE,st->fd,0);
   Elf32_Ehdr *ehd = (Elf32_Ehdr *)st->start_map;//header
   Elf32_Shdr *shd = (Elf32_Shdr *)(st->start_map+ehd->e_shoff);//Section headers descriptors
   Elf32_Shdr *sh_strtab=&shd[ehd->e_shstrndx/*index of string table*/];//string table
   const char *const strtabP=st->start_map+sh_strtab->sh_offset;
   int shN=ehd->e_shnum;

   for(int i=0;i<shN;i++){
       printf("[%2d] %-20s %-8x        %08x %06x %06x",i,shd[i].sh_name+strtabP, shd[i].sh_type,shd[i].sh_addr,shd[i].sh_offset,shd[i].sh_size);
       if(debu==1){
           printf("   %06x",shd[i].sh_name);
       }
       printf("\n");
   }
   printf("\n");
}


void Print_Symbols(state *st)
{
    printf("hi");
}

/*---------------------------------------------------------------------------*\
| Function: quit::ELF.h
| Purpose: end the programe
| 
| Input: 
|         state *st: state of the file 
|           
| Output: close the file from all his sections. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/

void quit(state *st)
{
    if (debu == 1)
        printf("%s", "Quit");
    if (st->start_map)
    {
        munmap(st->start_map, st->fd_stat.st_size);
    }
    free(st);
    exit(3);
}

/*---------------------------------------------------------------------------*\
| Function: printingElf::ELF.h
| Purpose: printing the informatin about the file
| 
| Input: 
|         state *st: state of the file 
|           
| Output: an information about the file and printing it like readelf. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/


void printingElf(state *st)
{
    printf("version:\t%i\t\t\n", st->header->e_version);
    printf("Entry point: %x\n", st->header->e_entry);                 // 3
    printf("Section header table: %d\n", st->header->e_shoff);        // 4
    printf("Number of section header: %d\n", st->header->e_shnum);    // 5
    printf("Size of section header : %d\n", st->header->e_shentsize); // 6
    printf("Size of header: %d\n", st->header->e_phoff);              // 7
    printf("Number of program header: %d\n", st->header->e_phnum);      // 8
    printf("Size of program headers: %d\n\n", st->header->e_phentsize); // 9
};