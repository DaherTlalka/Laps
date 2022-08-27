#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <errno.h>
#include <signal.h>
#include "stdlib.h"
#include <sys/wait.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1

//functions
void execute(cmdLine *cLine);


int main(int argc, char **argv){ 
    char buf[PATH_MAX];//to get the the files name
	char buf1[2048];//get the current 
	cmdLine* cLine;
	int ch;
	while(1){
		getcwd(buf,PATH_MAX);//open the currnt file
		fprintf(stderr,"%s ", buf);
		fgets(buf1,2048,stdin);//get it
		cLine=parseCmdLines(buf1);
		if(strncmp(buf1,"quit",4)==0){//if it quit
			freeCmdLines(cLine);
			break;
		}
		else if(strncmp(cLine->arguments[0],"cd",2)==0){
			if(chdir(cLine->arguments[1])==-1){
				printf("ERROR");
			}
		}
		else{
			ch=fork();
			if(ch==0){
				if(cLine!=NULL){
					execute(cLine);
				}
			}
			else if(cLine->blocking==1){
				waitpid(ch,0,0);
			}
		}
		freeCmdLines(cLine);
	}
	return 0;
}
void execute(cmdLine* cLine){
	
	if(cLine->inputRedirect){
		FILE* rediInput = fopen(cLine->inputRedirect,"r");
		int fdSTI = STDIN;
		int fdST=fileno(rediInput);
		dup2(fdST,fdSTI);
		fclose(rediInput);
	}
	if(cLine->outputRedirect){
		FILE* rediOutput = fopen(cLine->outputRedirect,"w");
		int fdSTO = STDOUT;
		int fdST = fileno(rediOutput);
		dup2(fdST,fdSTO);
		fclose(rediOutput);
	}
	if(execvp(cLine->arguments[0],cLine->arguments)==-1){
		perror("EROOR");
		_exit(0);
	}
}