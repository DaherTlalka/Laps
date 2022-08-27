
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct fun_desc {
  char *name;
  char (*fun)(char);
};
//Struct is a block of memory

int cheak(char c);
char censor(char c);
char* map(char *array, int array_length, char (*f) (char));
char encrypt(char c);
char decrypt(char c);
char dprt(char c);
char cprt(char c);
char my_get(char c);
char quit(char c);

struct fun_desc menu[] ={{"Censor",censor},
                         {"Encrypt",encrypt},
                         {"Decrypt",decrypt},
                         {"Print dec",dprt},
                         {"Print string",cprt},
                         {"Get string",my_get},
                         {"Quit",quit},
                         { NULL, NULL }};


const size_t MENU_SIZE = sizeof(menu)/sizeof(menu[0]);
const size_t ARRAY_SIZE = 5;

int main(int argc, char **argv){
  char* carray = malloc(ARRAY_SIZE); 
  carray[0] = '\0';
  char receive[1];
  char ch;
  int i, j, theFunction;
  char * temp;

  while(1){
    puts("Please choose a function:");
    for(i = 0 ; i < MENU_SIZE - 1 ; i++){
      printf("%d) %s\n",i,menu[i].name);
    }
    printf("Option: ");
    j=0;
    while((ch = fgetc(stdin)) != '\n'){
      receive[j] = ch;
      j++;
    }
    theFunction= atoi(receive);

    if(theFunction >= 0 && theFunction <= MENU_SIZE -2)
      puts("Within bounds");
    else{
      puts("Not within bounds");
      break;
    }

    temp = carray; // temp stores the old carray
    carray = map(carray,ARRAY_SIZE,menu[theFunction].fun);
    puts("DONE.\n");
  }
  free(temp);
  free(carray); 
  return 0;
}


int cheak(char c){//cheak if it a litter
  return c>='A'&&c<='z';
}



char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}



char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i=0;i<array_length;i++){
    mapped_array[i]=f(array[i]);
  }
  /* TODO: Complete during task 2.a */
  return mapped_array;
}



char encrypt(char c){//chack if the C is a litter if it get the litter that after 2 from him else return him
  if(cheak(c)) return c+2;
  return c;
}



char decrypt(char c){//chack if the C is a litter if it get the litter that before 2 from him else return him
  if(cheak(c)) return c-2;
  return c;
}


char dprt(char c){//print the latter
  printf("%d\n",c);
  return c;
}


char cprt(char c){//chack if the C is a litter if it print the latter else print the *
  if(cheak(c)){
    printf("%c\n",c);
  }
  else{
    printf("*\n");
  }
  return c;
}


char my_get(char c){//take a input from the user
  return fgetc(stdin);
}


char quit(char c){//if it a quit exit from the programe
  if(c=='q'){
    exit(0);
  }
  return c;
}





