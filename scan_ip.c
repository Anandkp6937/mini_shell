#include "main.h"
char*external_commands[200]={"\0"};
   char *argv[30];
   int pid;
   int status;
   lList*head=NULL;

   lList*createLlist(pid_t pid,char*cmd){
     lList*new=(lList*)malloc(sizeof(lList));
          new->link=NULL;
          strcpy(new->command,cmd);
          new->pid=pid;
          if(head==NULL){
          head=new;
          }
          else{
          new->link=head;
          head=new;     
          }
        return new;
 }
lList *deleteAtHead(lList *head) {
    if (head == NULL)
        return NULL;

    lList *temp = head;
    head = head->link;
    free(temp);

    return head;
}
void*printList(lList*head){
lList*temp=head;
if(head==NULL){
     printf("LIST IS EMPTY : \n");
}
while(temp!=NULL){
     printf("%s %d \n",temp->command,temp->pid);
     temp=temp->link;
}

}

   
void scan_input(char *prompt, char *input_string){
     signal(SIGINT,signal_handler);
     signal(SIGTSTP,signal_handler);
     extract_external_commands(external_commands);

while(1){
printf("%s : ",prompt);
scanf(" %[^\n]",input_string);

//checl argument that is whether it is PS1= or not
if(strncmp(input_string,"PS1=",4)==0){
     if(input_string[4]!=' '){
          strcpy(prompt,(input_string+4));
     }
     else{
     printf("Not validated ! \n");
     }
}
else {

char*cmd=get_command(input_string);
int res=check_command_type(cmd);
// printf("%d is the command type \n",res);
if(res==BUILTIN){
     execute_internal_commands(input_string);
}
else if(res==EXTERNAL){
int pipe_flag=0;
   
char*token;
char*str=input_string;
int i=0,j=0;
token=strtok(str," \t\n");
while(token){
argv[i++]=token;
token=strtok(NULL," \t\n");
}
argv[i]=NULL;
for(int i=0;argv[i]!=NULL;i++){
     if(strcmp(argv[i],"|")==0){
          pipe_flag=1;
          break;
     }
}
     pid=fork();
    if(pid==0){
     signal(SIGINT,SIG_DFL);
     signal(SIGTSTP,SIG_DFL);
   
     if(pipe_flag==1){
          implement_n_pipe(argv);
     }
     else{
   
       execute_external_commands(input_string);
     }
    }
    else if(pid>0){
     // parent process
     waitpid(pid,&status,WUNTRACED);
     pid=0;

    }
}
else if (strcmp(input_string, "jobs") == 0)
{
    if (head == NULL)
    {
        printf("No jobs\n");
    }
    else
    {
        printList(head);
    }
}

else if (strcmp(input_string, "bg") == 0)
{
    if (head == NULL)
    {
        printf("bg: no stopped process\n");
    }
    else
    {
        kill(head->pid, SIGCONT);
        printf("[%d] running in background\n", head->pid);
    }
}

else if (strcmp(input_string, "fg") == 0)
{
    if (head == NULL)
    {
        printf("fg: no jobs\n");
    }
    else
    {
        pid = head->pid;
        kill(pid, SIGCONT);

        waitpid(pid, &status, WUNTRACED);

        head = deleteAtHead(head);
        pid = 0;
    }
}


}

}

}



//check built in or not
int check_command_type(char *command){
     // printf("The command is %s \n",command);
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source","exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};


for(int i=0;builtins[i]!=NULL;i++){
     if(strcmp(builtins[i],command)==0){
          return BUILTIN;
     }
}

for(int i=0;external_commands[i]!=NULL;i++){
     if(strcmp(command,external_commands[i])==0){
          return EXTERNAL;
     }
}

return NO_COMMAND;

}

//fetching commands from file to array for useage
void extract_external_commands(char **external_commands){
     char buff[50]={'\0'};
     char ch;
     int j=0,i=0;
     int fd=open("ext_key.txt",O_RDONLY);
    if(fd==-1){
     perror("open: ");
     exit(1);
    }

    while(read(fd,&ch,sizeof(ch))>0){
     if(ch!='\n'){
          buff[i++]=ch;
     }
     else{
          external_commands[j]=calloc((strlen(buff)+1),sizeof(char));
          strcpy(external_commands[j++],buff);
          external_commands[j]=NULL;
          memset(buff,'\0',sizeof(buff));
          i=0;
     }

    }


}
