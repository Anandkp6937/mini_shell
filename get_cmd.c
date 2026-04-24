#include "main.h"
char ch;
char cmd[50];


char *get_command(char *input_string){
     int i=0,j=0;
     while(input_string[i]!='\n'){
          if(input_string[i] !=' '){
               ch=input_string[i];
               cmd[j++]=ch;
          }
          else{
               break;
          }
          i++;
     }
     cmd[i]='\0';
     return cmd;
}