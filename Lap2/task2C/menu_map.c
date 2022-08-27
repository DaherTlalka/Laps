
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
