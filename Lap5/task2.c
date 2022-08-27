#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

//struct process
typedef struct process{
	cmdLine* cmd; /* the parsed command line*/
	pid_t pid; /* the process id that is
	running the command*/
	int status; /* status of the process:
	RUNNING/SUSPENDED/TERMINATED */
	struct process *next; /* next process in chain */
} process;


//functions
void execute(cmdLine* cLine);
void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
char* getStringStatus(int status);
void updateStatus(process** pro,int pid, int status);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void freeProList(process** proList);

int main(int argc, char **argv){ 
    char buf[2048];//to get the the files name
	char buf1[2048];//get the current 
	process** pro_List=malloc(sizeof(process));
	int ch;

	while(1==1){
		getcwd(buf,PATH_MAX);//open the currnt file
		fprintf(stderr,"%s ", buf);
		fgets(buf1,2048,stdin);//get it
		cmdLine* cLine=parseCmdLines(buf1);
		if(strncmp(buf1,"quit",4)==0){//if it quit
			freeCmdLines(cLine);
			break;
		}
		else if(strncmp(cLine->arguments[0],"cd",2)==0){
			if(chdir(cLine->arguments[1])==-1){
				printf("ERROR");
			}
		}
		else if(strncmp(cLine->arguments[0],"showprocs")==0){
			printProcessList(pro_List);
		}
		else if(strncmp(cLine->arguments[0],"nap")==0){
			int pid=fork();
			if(pid==0){
				kill(atoi(cLine->arguments[2]),SIGTSTP);
				updateStatus(pro_List,atoi(cLine->arguments[2]),SUSPENDED);
				sleep(atoi(cLine->arguments[1]));
				kill(atoi(cLine->arguments[2]),SIGCONT);
				updateStatus(pro_List,atoi(cLine->arguments[2]),RUNNING);
				exit(0);
			}
		}
		else if(strncmp(cLine->arguments[0],"stop")==0){
			int pid=fork();
			if(pid==0){
				kill(atoi(cLine->arguments[1]),SIGINT);
				updateStatus(pro_List,atoi(cLine->arguments[2]),TERMINATED);
				exit(0);
			}
			
		}
		else{
			int pid = fork();
			int status;
			if (pid==0) {//Children Process
				execute(cLine);
			} 
			else {
				addProcess(pro_List,cLine,pid);
				if(cLine->blocking == 1){
					pid_t t = waitpid(pid,&status, 0);
				}
			}
		}
	}
	freeProList(pro_List);
	return 0;
}


void execute(cmdLine* cLine){
	if (execvp(cLine->arguments[0], cLine->arguments) == -1) {
       	perror("ERROR");
    	_exit(18);
    }
}


void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
	process* add_p=malloc(sizeof(process));
	process* temp=process_list[0];
	add_p->cmd=cmd;
	add_p->pid=pid;
	add_p->status=RUNNING;
	add_p->next=NULL;
	if(temp==NULL)
		process_list[0]=add_p;
	else{
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next=add_p;
	}
}


void printProcessList(process** pro_list){
	updateProcessList(pro_list);
	process* pr=*pro_list;//pr for print
	process* t=pr;//temp
	if(pr!=NULL){
	printf("id\t  com   \t  rocess\n");
	while(pr!=NULL){
		printf("%d  \t%s \t %s\t",pr->pid,pr->cmd->arguments[0],getStringStatus(pr->status));
		if(pr->status==TERMINATED){
			t->next=pr->next;
			t=pr->next;
			if(pr == pro_list[0])
                pro_list[0] = t;
			free(pr);
		}
		if(pr==NULL){
			pr=t;
		}
		else{
			t=pr;
			pr=pr->next;
		}
		printf("\n");
	}
	}
}


char* getStringStatus(int status){
	switch (status){
		case -1:{
			return "TERMINATED";
			break;
		}
		case 1:{
			return "RUNNING";
			break;
		}
		case 0:{
			return "SUSPENDED";
			break; 
		}
	}
}


void updateStatus(process** pro,int pid1, int status1){
	process* temp=pro[0];
	while(temp->pid!=pid1){
		temp=temp->next;
	}
	temp->status=status1;
}


void freeProcessList(process* process_list){
	process* temp=process_list;
	while(temp!=NULL){
		process* t=temp;
		temp=temp->next;
		free(t);
	}
}


void updateProcessList(process **process_list){
	process* p=process_list[0];
	int check;
	int status;

	while(p!=NULL){
		check=waitpid(p->pid,&status,WNOHANG | WUNTRACED |WCONTINUED);
		if(check!=0){
			 if(WIFCONTINUED(status)){
                p->status = RUNNING;
            }else if(WIFEXITED(status) | WIFSIGNALED(status)){
                p->status = TERMINATED;
            }else if(WIFSTOPPED(status)){
                p->status = SUSPENDED;
            }
		}
		p=p->next;
	}
}


void freeProList(process** proList){
	freeProcessList(proList[0]);
	free(proList);
}