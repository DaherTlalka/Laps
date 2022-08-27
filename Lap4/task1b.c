#include "util.h"
#define STDERR 2
#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_CLOSE 6
#define SYS_OPEN 5
#define SYS_EXIT 1
#define SYS_LSEEK 19
#define STDIN 0
#define STDOUT 1
#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64
extern int system_call();
int main (int argc , char* argv[], char* envp[]) {
    int input=STDIN,output=STDOUT,firstLitter=0,chwords=0,isD=0;
    int sumWords=0;
    char lit='\n';
    int i_r=0;
    for(;i_r<argc;i_r++){
        if(strncmp(argv[i_r], "-i", 2) == 0){
            if((input = system_call(SYS_OPEN, argv[i_r]+2, O_RDONLY, 0644)) <= 0){/*if it an input*/
                system_call(SYS_WRITE, STDOUT, "The file isn't found!we will use standard input!!\n", strlen("The file isn't found!we will use standard input!!\n"));
            }
        }
        if(strncmp(argv[i_r], "-o", 2) == 0)
            output = system_call(SYS_OPEN, argv[i_r]+2, O_CREAT | O_WRONLY, 0644);/*to put in a file that call output*/

        if(strncmp(argv[i_r],"-D",2)==0||strncmp(argv[i_r],"-d",2)==0){
            isD=1;
            system_call(SYS_WRITE, STDERR, "in:", strlen("in:"));
            system_call(SYS_WRITE,STDERR,itoa(input),strlen(itoa(input)));
            system_call(SYS_WRITE, STDERR, "  out:", strlen("  out:"));
            system_call(SYS_WRITE,STDERR,itoa(output),strlen(itoa(output)));
            system_call(SYS_WRITE,STDERR,"\n",strlen("\n"));
        }
    }
    int f=0;
    while(system_call(SYS_READ, input, &lit,1) > 0){
        if(isD){
            system_call(SYS_WRITE, STDERR, "system call [arg1, arg2, arg3, arg4, ret code] = ", strlen("system call [arg1, arg2, arg3, arg4, ret code] = "));
            system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
            system_call(SYS_WRITE,STDERR,", ",strlen(", "));
            system_call(SYS_WRITE,STDERR,itoa(STDIN),strlen(itoa(STDIN)));
            system_call(SYS_WRITE,STDERR,", ",strlen(", "));
            system_call(SYS_WRITE,STDERR,itoa(lit),strlen(itoa(lit)));
            system_call(SYS_WRITE,STDERR,", 1, 1\n",strlen(", 1, 1\n"));
        }
        if(lit==' '&&firstLitter==1)
            firstLitter=0;
        else if((lit!=' '&&lit!='\n')&&firstLitter==0){/*if we acount the number then put the number */
            chwords=1;
            firstLitter=1;
            sumWords+=1;
        }
        else if(lit=='\n'&&chwords==1){     
            system_call(SYS_WRITE, output, itoa(sumWords),strlen(itoa(sumWords)));
            system_call(SYS_WRITE, output, "\n",strlen("\n"));
            if(isD==1){
                system_call(SYS_WRITE,STDERR, "system call [arg1, arg2, arg3, arg4, ret code] = ", strlen("system call [arg1, arg2, arg3, arg4, ret code] = "));
                system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
                system_call(SYS_WRITE,STDERR,", ",strlen(", "));
                system_call(SYS_WRITE,STDERR,itoa(output),strlen(itoa(output)));
                system_call(SYS_WRITE,STDERR,", ",strlen(", "));
                system_call(SYS_WRITE,STDERR,itoa(sumWords),strlen(itoa(sumWords)));
                system_call(SYS_WRITE,STDERR,", 1, 1\n",strlen(", 1, 1\n"));
                }
            sumWords=0;
            firstLitter=0;
        }
    }
    if(input != STDIN)
        system_call(SYS_CLOSE, input);
    if(output != STDOUT)
        system_call(SYS_CLOSE, output);
    return 0;
}

