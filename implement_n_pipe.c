#include "main.h"

void implement_n_pipe(char **argv)
{
    int cmd_start[20];
    int cmd_count = 0;

    
    cmd_start[cmd_count++] = 0;

    
    for (int i = 0; argv[i] != NULL; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            argv[i] = NULL;                 // split command
            cmd_start[cmd_count++] = i + 1; 
        }
    }

    int prev_read = 0;
    int fd[2];

    for (int i = 0; i < cmd_count; i++)
    {
        // create pipe except for last command
        if (i < cmd_count - 1)
        {
            pipe(fd);
        }

        pid_t pid = fork();

        if (pid == 0)  // CHILD
        {
         
            if (prev_read != 0)
            {
                dup2(prev_read, STDIN_FILENO);
                close(prev_read);
            }

          
            if (i < cmd_count - 1)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                close(fd[0]);
            }

            execvp(argv[cmd_start[i]], &argv[cmd_start[i]]);
            perror("execvp");
            exit(1);
        }
        else         
        {
            if (prev_read != 0)
                close(prev_read);

            if (i < cmd_count - 1)
            {
                close(fd[1]);
                prev_read = fd[0];
            }
        }
    }


    for (int i = 0; i < cmd_count; i++)
        wait(NULL);
}
