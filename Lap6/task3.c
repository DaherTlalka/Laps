/*----------------------------------------------------------------------------
|
| Module Details: task 3 - Lab 6
|
| Name: task3.c
|
| Purpose: implement multi-pipe system.
|
| Date      	Name Ver. Comments
| --------- ------- ----- ----- -----------------------------------------
| 26-May-22 	Sohel $$1 Created  
| 28-May-22 	Fihme $$2 Edited excute function(added args)  
----------------------------------------------------------------------------*/



#include "task3.h"



int main(int argc, char **argv)
{
	char buf[BUFF_SIZE];  // to get the the files name
	char buf1[BUFF_SIZE]; // get the current
	process **pro_List = malloc(sizeof(process));
	cmdLine *cLine;dataset

	while (1 == 1)
	{
		getcwd(buf, PATH_MAX); // open the currnt file
		fprintf(stderr, "%s ", buf);
		fgets(buf1, BUFF_SIZE, stdin); // get it
		cLine = parseCmdLines(buf1);

		if (strncmp(buf1, "quit", 4) == 0)
		{ // if it quit
			freeCmdLines(cLine);
			break;
		}
		else if (cLine->next != NULL)
		{
			counter = 0;
			cmdLine *temp = cLine;
			while (temp != NULL)
			{
				counter++;
				temp = temp->next;
			}
			printCommands(cLine, counter);
		}
		else if (strncmp(cLine->arguments[0], "cd", 2) == 0)
		{
			if (chdir(cLine->arguments[1]) == -1)
			{
				printf("ERROR");
			}
		}
		else if (strncmp(cLine->arguments[0], "cat", 3) == 0)
		{
			catFun(cLine);
		}
		else if (strcmp(cLine->arguments[0], "showprocs") == 0)
		{
			printProcessList(pro_List);
		}
		else if (strcmp(cLine->arguments[0], "nap") == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				kill(atoi(cLine->arguments[2]), SIGTSTP);
				updateStatus(pro_List, atoi(cLine->arguments[2]), SUSPENDED);
				sleep(atoi(cLine->arguments[1]));
				kill(atoi(cLine->arguments[2]), SIGCONT);
				updateStatus(pro_List, atoi(cLine->arguments[2]), RUNNING);
				exit(0);
			}
		}
		else if (strncmp(cLine->arguments[0], "stop", 4) == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				kill(atoi(cLine->arguments[1]), SIGINT);
				updateStatus(pro_List, atoi(cLine->arguments[2]), TERMINATED);
				exit(0);
			}
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{ // Children Process
				execute(cLine);
			}
			else
			{
				addProcess(pro_List, cLine, pid);
				if (cLine->blocking == 1)
				{
					waitpid(pid, &status, 0);
				}
			}
		}
	}
	freeProList(pro_List);
	return 0;
}


/*---------------------------------------------------------------------------*\
| Function: execute::task3.h
| Purpose: excute a cmdline 
| 
| Input: 
|         cmdLine *cLine: cmd line argument  
|           
| Output: execution of the cmd line arg. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void execute(cmdLine *cLine)
{
	if (execvp(cLine->arguments[0], cLine->arguments) == -1)
	{
		perror("ERROR");
		_exit(18);
	}
}


/*---------------------------------------------------------------------------*\
| Function: addProcess::task3.h
| Purpose: add a process to a process list
| 
| Input: 
|         process **process_list: list of precesses
|         cmdLine *cLine: cmdline 
|         pid_t pid: precess id
| Output: add cmdline to the precess list. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void addProcess(process **process_list, cmdLine *cLine, pid_t pid)
{
	process *add_p = malloc(sizeof(process));
	process *temp = process_list[0];
	add_p->cLine = cLine;
	add_p->pid = pid;
	add_p->status = RUNNING;
	add_p->next = NULL;

	if (temp == NULL)
	{
		process_list[0] = add_p;
	}
	else
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = add_p;
	}
}

/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void printProcessList(process **pro_list)
{
	updateProcessList(pro_list);
	process *pr = *pro_list; // pr for print
	process *t = pr;		 // temp

	if (pr != NULL)
	{
		printf("id\t  com   \t  rocess\n");
		while (pr != NULL)
		{
			printf("%d  \t%s \t %s\t", pr->pid, pr->cLine->arguments[0], getStringStatus(pr->status));
			if (pr->status == TERMINATED)
			{
				t->next = pr->next;
				t = pr->next;
				if (pr == pro_list[0])
					pro_list[0] = t;
				free(pr);
			}
			if (pr == NULL)
			{
				pr = t;
			}
			else
			{
				t = pr;
				pr = pr->next;
			}
			printf("\n");
		}
	}
}
/*---------------------------------------------------------------------------*\
| Function: getStringStatus::task3.h
| Purpose: get the string of the status
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/

char *getStringStatus(int status)
{
	// status == -1 ? return "TERMINATED" : status == 1 ? return "RUNNING"
	// 												 : return "SUSPENDED";
	switch (status)
	{
	case -1:
	{
		return "TERMINATED";
		break;
	}
	case 1:
	{
		return "RUNNING";
		break;
	}
	case 0:
	{
		return "SUSPENDED";
		break;
	}
	}
	return "";
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/

void updateStatus(process **pro, int pid1, int status)
{
	process *temp = pro[0];
	while (temp->pid != pid1)
	{
		temp = temp->next;
	}
	temp->status = status;
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void freeProcessList(process *process_list)
{
	process *temp = process_list;
	while (temp != NULL)
	{
		process *t = temp;
		temp = temp->next;
		free(t);
	}
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void updateProcessList(process **process_list)
{
	process *p = process_list[0];
	int check;
	int status;

	while (p != NULL)
	{
		check = waitpid(p->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
		if (check != 0)
		{
			if (WIFCONTINUED(status))
			{
				p->status = RUNNING;
			}
			else if (WIFEXITED(status) | WIFSIGNALED(status))
			{
				p->status = TERMINATED;
			}
			else if (WIFSTOPPED(status))
			{
				p->status = SUSPENDED;
			}
		}
		p = p->next;
	}
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void freeProList(process **proList)
{
	freeProcessList(proList[0]);
	free(proList);
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void catFun(cmdLine *cline)
{
	int p = fork();

	if (p == 0)
	{ // child
		if (cline->inputRedirect != NULL)
		{
			close(0); // standert input
			open(cline->inputRedirect, O_RDONLY);
		}
		if (cline->outputRedirect != NULL)
		{
			close(1);
			open(cline->outputRedirect, O_WRONLY | O_CREAT);
		}
		execute(cline);
	}
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
int **createPipes(int nPipes)
{
	int **pipes;
	pipes = (int **)calloc(nPipes, sizeof(int));

	for (int i = 0; i < nPipes; i++)
	{
		pipes[i] = (int *)calloc(2, sizeof(int));
		pipe(pipes[i]);
	}
	return pipes;
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void releasePipes(int **pipes, int nPipes)
{
	for (int i = 0; i < nPipes; i++)
	{
		free(pipes[i]);
	}
	free(pipes);
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
int *leftPipe(int **pipes, cmdLine *pCmdLine)
{
	if (pCmdLine->idx == 0)
		return NULL;
	return pipes[pCmdLine->idx - 1];
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
int *rightPipe(int **pipes, cmdLine *pCmdLine)
{
	if (pCmdLine->next == NULL)
		return NULL;
	return pipes[pCmdLine->idx];
}
/*---------------------------------------------------------------------------*\
| Function: printProcessList::task3.h
| Purpose: print the process list on the output stream
| 
| Input: 
|         process **process_list: list of precesses
| Output: print the process List. 
| Return: VOID. 
\*---------------------------------------------------------------------------*/
void printCommands(cmdLine *cLine, int counter)
{
	int closeIn = 0;
	int closeOut = 1;
	int pid;
	int length = 0;
	int **pipe = createPipes(counter - 1);
	int *p;
	while (cLine != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
			if (closeIn)
			{
				close(0);
				p = leftPipe(pipe, cLine);
				dup(p[0]);
				close(p[0]);
			}
			if (closeOut)
			{
				close(1);
				p = rightPipe(pipe, cLine);
				dup(p[1]);
				close(p[1]);
			}
			if (strncmp(cLine->arguments[0], "cd", 2) == 0)
			{
				if (chdir(cLine->arguments[1]) == -1)
				{
					fprintf(stderr, "Error!\n");
				}
			}
			else if (strncmp(cLine->arguments[0], "cat", 3) == 0)
			{
				if (cLine->inputRedirect != NULL)
				{
					close(0);
					open(cLine->inputRedirect, O_WRONLY, 0644);
				}
				if (cLine->outputRedirect != NULL)
				{
					close(1);
					open(cLine->outputRedirect, O_WRONLY | O_CREAT, 0644);
				}
				execute(cLine);
			}
			else
			{
				execute(cLine);
			}
		}
		else
		{
			if (cLine->next != NULL)
			{
				p = rightPipe(pipe, cLine);
				close(p[1]);
			}
			int st;
			waitpid(pid, &st, 0);
		}
		closeIn = 1;
		closeOut = 1;
		cLine = cLine->next;
		length++;
		if (length == counter - 1)
		{
			closeOut = 0;
		}
	}
	releasePipes(pipe, counter - 1);
	
}