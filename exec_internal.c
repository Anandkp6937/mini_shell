#include "main.h"
void execute_internal_commands(char *input_string){

if(strcmp(input_string,"exit")==0){
     exit(0);
}
if(strcmp(input_string,"pwd")==0){
     char str[100];
     getcwd(str,100);
     printf("%s \n",str);
}
if(strncmp(input_string,"cd",2)==0){
     chdir(&input_string[3]);
}

if(strncmp(input_string,"echo",4)==0){
     if(strncmp(&input_string[5],"$SHELL",6)==0){
          printf("%s \n",getenv("SHELL"));
     }
     else if(strncmp(&input_string[5],"$?",2)==0){
          //later
     }
     else if(strncmp(&input_string[5],"$$",2)==0){
          printf("%d \n",getpid());
     }
}
}