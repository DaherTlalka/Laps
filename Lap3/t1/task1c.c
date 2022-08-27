
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//struct
typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {//function for the functions the the user call it
  char *name;
  void (*fun)();
};
//Global Variables
link* The_viruses=NULL;
char bufferSize[10000];
int readvirus=0;
char fileName[20];

//function
void readVirus(virus* vir, FILE* input);
void printVirus(virus* vir, FILE* output);
void list_print(link *virus_list, FILE* output); 
link* list_append(link* virus_list, link* to_add); 
void list_free(link *virus_list);
void detect_virus(char *buffer, unsigned int size, link *virus_list);
void Load_signatures(FILE* input);
void Print_signatures(FILE* expected);
void Detect_viruses(FILE* expected);
void Quit();

struct fun_desc menu[] = { { "Load signatures", Load_signatures },
                           { "Print signatures", Print_signatures }, 
                           { "Detect viruses", Detect_viruses },
                           {"Quit",Quit},
                           { NULL, NULL } };


int main(int argc, char** argv){
    int menuSize = sizeof(menu)/sizeof(menu[0])-1;
    char option[10];
    while(1){
        for(int i = 0; i < menuSize; i++) fprintf(stderr, "%d) %s\n", i+1, menu[i].name);//print all the option
        fprintf(stderr, "Option: ");
        char function[20];//to know what function the user call
        scanf("%s",function);
        if(function[0]=='1'){
            char inputFile[20];
            scanf("%s",inputFile);
            FILE* file_name = fopen(inputFile,"r");
            if(file_name==NULL){
                fprintf(stderr,"the file is not existed\n");
                exit(0);
            }
            menu[0].fun(file_name);
            fclose(file_name);
        }
        else if(function[0]=='2'){
            menu[1].fun("signatures"); 
        }
        else if(function[0]=='3'){
            char inputFile[20];
            scanf("%s",inputFile);
            FILE* file_name = fopen(inputFile,"r");
            if(file_name==NULL){
                fprintf(stderr,"the file is not existed\n");
                exit(0);
            }
            menu[2].fun(file_name);
            fclose(file_name);
        }
        else{
            Quit();
        }
    }
}

//task 1a
void readVirus(virus* vir, FILE* input){//we read the input and put on the vireses
    char sigSize[2];
    int temp=fread(sigSize,2,1,input);
    if(temp<=0) {//the sigsize of the viruse is 2 and each element is 1
        free(vir);
        fclose(input);
        exit(0);
    }
    vir->SigSize=sigSize[0]+sigSize[1]*16*16;//the size 
    vir->sig=(unsigned char*) malloc(sizeof(unsigned char*) * (vir->SigSize));
    temp=fread(vir->sig, vir->SigSize, 1, input);
    if(temp <= 0){
        free(vir->sig);
        free(vir);
        fclose(input);
        exit(0);
    }
    temp=fread(vir->virusName,16,1,input);
    if(temp<=0){
        free(vir->sig);
        free(vir);
        fclose(input);
        exit(0);
    }
}

void printVirus(virus* vir, FILE* output){//in this function we read the viruses and printed on the screen
    fprintf(output, "Virus Name: %s\nVirus Size: %d\nSignature: \n", vir->virusName, vir->SigSize);
    int count=0;
    for (int i = 0; i < vir->SigSize; i++){
        fprintf(output, "%02x ", vir->sig[i]);
        count++;
        if(count==22){//if the virus has alot then evry 22 in aline
            printf("\n");
            count=0;
        }
    }
    fprintf(output, "\n\n");
}

//task1b
void list_print(link *virus_list, FILE* output){
    while(virus_list!= NULL){
        printVirus(virus_list->vir,output);
        virus_list=virus_list->nextVirus;
    }
}

link* list_append(link* virus_list, link* to_add){//add a new virus in the list
    if(virus_list==NULL){
        return to_add;
    }
    else{
        link* temp=virus_list;
        while(temp->nextVirus!=NULL){
            temp=temp->nextVirus;
        }
        temp->nextVirus=to_add;
    }
    return virus_list;
} 

void list_free(link *virus_list){//delete the list
    link* temp = virus_list;//get a temp list to delete every time the local list
    while(temp != NULL){
        virus_list = temp;
        temp = temp->nextVirus;
        free(virus_list->vir->sig);
        free(virus_list->vir);
        free(virus_list);
    }
}
//task1c
void detect_virus(char *buffer, unsigned int size, link *virus_list){
    while(virus_list!=NULL){//to print all the viruss
         for(int i=0;i<size-virus_list->vir->SigSize;i++){
             if(memcmp(buffer+i,virus_list->vir->sig,virus_list->vir->SigSize)==0){
                fprintf(stderr, "Virus detected!!\nStarting byte: %d\nVirus name: %s\nSignature size: %d\n", i, virus_list->vir->virusName, virus_list->vir->SigSize);
                printf("---------------------------\n");
             }
        }
        virus_list=virus_list->nextVirus;
    }
}
void Load_signatures(FILE* input){
    while(!feof(input) && readvirus<11) {
        virus *v = malloc(sizeof(virus));
        readVirus(v, input);
        link* toadd = malloc(sizeof(link));
        toadd->nextVirus = NULL;
        toadd->vir = v;
        The_viruses=list_append(The_viruses,toadd);
        readvirus++;
    }
    readvirus=0;
}

void Print_signatures(FILE* expected){
     list_print(The_viruses, stdout);
}

void Detect_viruses(FILE* expected){
    int size;
    fseek(expected, 0, SEEK_END);
    size = ftell(expected);
    fseek(expected, 0, SEEK_SET);
    fread(bufferSize, size, 1, expected);
    detect_virus(bufferSize, size,The_viruses);
}

void Quit(){
    list_free(The_viruses);
    exit(0);
}
