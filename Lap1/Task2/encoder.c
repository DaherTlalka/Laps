#include<stdio.h>
#include<string.h>

void litters(int variable,char temp,FILE* output){
     while(variable>0){
        putc(temp,output);
        variable--;
        }
}
int main(int argc, char **argv) {
    int ch,arg,counter=0,debug=0,isencoder=0,mid=0,temp,thefirstlitter=0,ise=0,counter_ise=0;
    FILE* input=stdin;
    FILE* output=stdout;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-D")==0){//to know if there any debug or not
            debug=1;
        }
        char str[2];
        str[0] = argv[i][0];
        str[1] = argv[i][1];
        if((strcmp(str,"+e")==0||strcmp(str,"-e")==0)&&strlen(argv[i]) == 3){//for to know how match we have to rebbet
            isencoder=1;
            if(strcmp(str,"-e")==0){//to know if it -e
                ise=1;
            }
            if(argv[i][2]>=65&&argv[i][2]<=70){
                mid=argv[i][2]-65+10;//the litters 10 to 15
            }
            else{
                mid=argv[i][2]-48;//the numbers 1to 9
            }
        }
        else if(strncmp(argv[i],"-i",2) == 0){
            if((input = fopen(argv[i]+2, "r")) == NULL){
                fprintf(stderr, "There is no File");
            }
        }
        else if(strcmp(str,"-o") == 0 && strlen(argv[i]) > 2){
            if((output = fopen(argv[i]+2,"w"))== NULL){
				printf("The file not found!");
            }
        }
    }
    while((ch=fgetc(input))!=EOF){
        if(ise&&ch!='\n'){//if we are in the -e 
        counter_ise++;
        }
        if(thefirstlitter==0&&isencoder){//if we in the +e 
            temp=ch;
            thefirstlitter=1;
        }
        if(ch>='A'&&ch<='Z'){//if it a big litter
            arg='.';
            counter++;
        }
        else{//if if a small litter
            arg=ch;
        }
        if(debug&&ch!='\n'){//to know if we are in the debbging
            fprintf(stderr,"%d\t%d\n",ch,arg);
            fputc(arg,output);
        }
        else if(debug){
            fprintf(stderr,"\nthe number of letters: %d\n",counter);
            counter=0;
        }
        else if(isencoder){//to the encoder +/-e
            if(ch=='\n'&&ise==0){//to put the last litter in the last
                litters(mid,temp,output);
                thefirstlitter=0;
                fputc(ch,output);
            }
            else if(ise){//for -e
                if(counter_ise>mid){
                    fputc(ch,output);
                }
                if(ch=='\n'){
                    if(counter_ise<=mid){
                        printf("-NOUN-\n");
                    }
                    counter_ise=0;
                }
            }
            else{//for +e
                fputc(ch,output); 
            }
        }
        else{
            fputc(arg,output);
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}

