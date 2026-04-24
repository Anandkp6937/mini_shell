#include "main.h"
extern char prompt[20];
extern char input_string[50];
extern int pid;

void signal_handler(int sig_num)
{
    if (sig_num == SIGINT)
    {
        // Ctrl+C pressed
        if (pid == 0)
        {
            // Shell is idle → just print prompt
            printf("\n%s ", prompt);
            fflush(stdout);
        }
        // If pid > 0 → child will terminate automatically
    }
    else if (sig_num == SIGTSTP)
    {
        // Ctrl+Z pressed
        if (pid == 0)
        {
            // Shell is idle → just print prompt
            printf("\n%s ", prompt);
            fflush(stdout);
        }
        else
        {
            // Stop the running child and save in job list
            kill(pid, SIGTSTP);              // stop child process
            createLlist(pid, input_string);   // store stopped job
            printf("\n[%d] Stopped %s\n", pid, input_string);
        }
    }

    else if (sig_num == SIGCHLD){
    // just clean up any finished child processes
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ; // do nothing else
}

}