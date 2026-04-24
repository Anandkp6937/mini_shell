#include "main.h"
extern char *argv[30];
void execute_external_commands(char*input_string){
execvp(argv[0],argv);

}