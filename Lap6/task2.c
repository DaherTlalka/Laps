#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h> 
#include <wait.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <fcntl.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int debug = 0;

int child(int pipefd[], int toClose, int i, cmdLine* commandLine, int debug){
    if (debug){
        fprintf(stderr, "(parent_process>forking…)\n");
    }
    int chID = fork();     // forking 
    if(chID == 0){     //child processes 
        if(commandLine->inputRedirect != NULL){    // redirect the input stream
            int fd = open(commandLine->inputRedirect, O_RDONLY);
            dup2(fd, STDIN); //INPUT redirect
            close(fd);
        }
        if(commandLine->outputRedirect != NULL){     // redirect the output stream
            int fd;
            if ((fd = open(commandLine->outputRedirect, O_CREAT | O_WRONLY, 0777)) < 0){
                fprintf(stderr, "error opening output");
            }
            dup2(fd, STDOUT); //OUTPUT redirect
            close(fd);
        }
        if (debug){
            if(i == 1){     // if the child is the first child with write end
                fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
            }
            else{     // the child with the read end
                fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");
            }
        }
        close(toClose);
        dup(pipefd[i]);     //duplicating the file descriptor
        close(pipefd[i]);   //closing the original file descriptor
        if (debug){
            fprintf(stderr, "(child>going to execute cmd: %s)\n", commandLine->arguments[0]);
        }
        execvp(commandLine->arguments[0], commandLine->arguments);  
        perror("execvp");
        _exit(0);
    }
    return chID;
}

void execute(cmdLine *exeCLine){
    if(strncmp(exeCLine->arguments[0], "cd", 2) == 0){     // changing the proccess's working directory
        chdir(exeCLine->arguments[1]);
    }
    else if (exeCLine->next != NULL) {
        int pipefd[2];
        int child1;
        int child2;
        if (pipe(pipefd) == -1){
            fprintf(stderr,"error while piping...");
            _exit(0);
        }
        child1 =  child(pipefd, STDOUT_FILENO, 1, exeCLine, debug);     //using the child function and send the parameters for child 1 for forking and redirecting if it neccessary
        if(debug){
            fprintf(stderr, "(parent_process>created process with id: %d)\n", child1);
        }
        if(debug){
            fprintf(stderr, "(parent_process>closing the write end of the pipe…)\n");
        }
        close(pipefd[1]);
        child2 = child(pipefd, STDIN_FILENO, 0, exeCLine->next, debug);     //using the child function and send the parameters for child 2 for forking and redirecting if it neccessary
        if(debug){
            fprintf(stderr, "(parent_process>created process with id: %d)\n", child2);
        }
        if(debug){
            fprintf(stderr, "(parent_process>closing the read end of the pipe…)\n");
        }
        close(pipefd[0]);
        if(debug){
            fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
        }
        waitpid(child1, NULL, 0);     //waiting for child1 to terminate
        waitpid(child2, NULL, 0);     //waitntg for child  to terminate
        if(debug){
            fprintf(stderr, "(parent_process>exiting…)\n");
        }
    } else {
        int childPID = fork();     // forking for redirect the I/O streams
        if(childPID == 0){
            if(exeCLine->inputRedirect != NULL){     //if it neccessary to redirect 
                int fd = open(exeCLine->inputRedirect, O_RDONLY);
                dup2(fd, STDIN); //INPUT redirect
                close(fd);
            }
            if(exeCLine->outputRedirect != NULL){     //if it neccessary to redirect 
                int fd;
                if ((fd = open(exeCLine->outputRedirect, O_CREAT | O_WRONLY, 0777)) < 0){     
                    fprintf(stderr, "error opening output");
                }
                dup2(fd, STDOUT); //OUTPUT redirect
                close(fd);
            }
            execvp(exeCLine->arguments[0], exeCLine->arguments);
            perror("Error");
            exit(-1);
        }
        if(exeCLine->blocking == 1)
            waitpid(childPID, NULL, 0);
    }
}

int main(int argc, char** argv){
    char path[PATH_MAX];
    char in[2048];
    getcwd(path, PATH_MAX);
    cmdLine* cLine;
        for(int i=1; i<argc; i++){     //check debug mode
            if(strcmp(argv[i], "-d") == 0){
                debug = 1;
            }
        }
    while(1){
        getcwd(path, PATH_MAX);
        fprintf(stderr, "%s: ", path);
        fgets(in, 2048, stdin);     //get the input commands
        cLine = parseCmdLines(in);     //parsing the commands
        if(strncmp(cLine->arguments[0], "quit", 4) == 0){
            exit(0);
        }
        execute(cLine);
    }
    freeCmdLines(cLine);
    return 0;
}