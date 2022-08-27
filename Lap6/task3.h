#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0
#define BUFF_SIZE 2048

// struct process
typedef struct process
{
	cmdLine *cLine;		  /* the parsed command line*/
	pid_t pid;			  /* the process id that is
			   running the command*/
	int status;			  /* status of the process:
			  RUNNING/SUSPENDED/TERMINATED */
	struct process *next; /* next process in chain */
} process;

// functions



void execute(cmdLine *cLine);
void addProcess(process **process_list, cmdLine *cLine, pid_t pid);
void printProcessList(process **process_list);
char *getStringStatus(int status);
void updateStatus(process **pro, int pid, int status);
void freeProcessList(process *process_list);
void updateProcessList(process **process_list);
void freeProList(process **proList);
void catFun(cmdLine *cline);
int **createPipes(int nPipes);
void releasePipes(int **pipes, int nPipes);
int *leftPipe(int **pipes, cmdLine *pCmdLine);
int *rightPipe(int **pipes, cmdLine *pCmdLine);
void printCommands(cmdLine *cmds, int length);
