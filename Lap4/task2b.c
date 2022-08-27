
#include "util.h"
#define STDERR 2
#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_CLOSE 6
#define SYS_OPEN 5
#define SYS_EXIT 1
#define SYS_LSEEK 19
#define SYS_GETDENTS 141
#define STDIN 0
#define STDOUT 1
#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64
#define DT_BLK 6 /*block dev*/
#define DT_CHR 2 /*char dev*/
#define DT_DIR 4 /*directory*/
#define DT_FIFO 1/*FIFO*/
#define DT_LNK 10 /*symlink*/
#define DT_SOCK 12 /*socket*/
#define DT_REG 8 /*regular*/
#define DT_UNKNOWN 0 /*????*/

extern int system_call();

typedef struct flame{
    int d_ino;
    int offset;
    short length;
    char name[];
} flame;

void print(int argv1,char* argv2);
int main (int argc , char* argv[], char* envp[]) {
    char buffer[8192],data_type,pre='\n',*type,isD=0;
    int r,file;
    flame* flame_value;
    file=system_call(SYS_OPEN, ".", O_RDONLY, 0644);/*open the file name that before .*/
    r= system_call(SYS_GETDENTS, file, &buffer, 8192);/*read the information of the file**/
    int i_r=0 ;
    for(;i_r<argc;i_r++){
        if(strncmp(argv[i_r],"-p",2)==0){/*cheak if it -p*/
            pre=(argv[i_r]+2)[0];/*get the prefix that we will work on it*/
        }
        if(strncmp(argv[i_r],"-D",2)==0||strncmp(argv[i_r],"-d",2)==0){
            isD=1;
            system_call(SYS_WRITE, STDERR, "ID:", strlen("ID:"));
            system_call(SYS_WRITE,STDERR,itoa(SYS_GETDENTS),strlen(itoa(SYS_GETDENTS)));
            system_call(SYS_WRITE, STDERR, "  read: ", strlen("  read: "));
            system_call(SYS_WRITE,STDERR,itoa(r),strlen(itoa(r)));
            system_call(SYS_WRITE,STDERR,"\n",strlen("\n"));
        }
    }
    i_r=0;
    while(i_r<r){
        flame_value=(flame*)(buffer+i_r); /*get The next linux_dirent to work on it*/
        data_type=*(buffer+i_r+flame_value->length-1); /*the type of the linux_dirent*/
        if(isD){
            print(flame_value->length,flame_value->name);
        }
        if((pre=='\n'||pre==flame_value->name[0])){
            system_call(SYS_WRITE, STDOUT, flame_value->name, strlen(flame_value->name));/*print the flame name on the screen*/
            system_call(SYS_WRITE, STDOUT, "\t", 2);/*inter*/
        /*get the type*/
            switch (data_type){
            case DT_BLK:
                type="block dev";
                break;
            case DT_CHR:
                type="char dev";
                break;
            case DT_DIR:
                type="directory";
                break;
            case DT_FIFO:
                type="FIFO";
                break;
            case DT_LNK:
                type="symlink";
                break;
            case DT_SOCK:
                type="socket";
                break;
            case DT_REG:
                type="regular";
                break;
            case DT_UNKNOWN:
                type="????";
                break;
            }
            system_call(SYS_WRITE, STDOUT, type, strlen(type));
            system_call(SYS_WRITE, STDOUT, "\n", 1);
        }
        i_r=i_r+flame_value->length;/*go to the next one*/
    }
    system_call(SYS_CLOSE,file);
    return 0;
}
void print(int argv1,char* argv2){
    system_call(SYS_WRITE, STDERR, "length: ", strlen("length: "));
    system_call(SYS_WRITE, STDERR, itoa(argv1), strlen(itoa(argv1)));
    system_call(SYS_WRITE, STDERR, "\n\n", strlen("\n\n"));
    system_call(SYS_WRITE, STDERR, "name: ",strlen("name: "));
    system_call(SYS_WRITE, STDERR, argv2, strlen(argv2));
    system_call(SYS_WRITE, STDERR, "\n", 1);
}