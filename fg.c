#include "headers.h"

void my_fg(int pid)
{
    int status;
    int p = kill(pid, SIGCONT);
    if(p < 0)
    {
        perror(RED "Error" RESET);
        return;
    }
    while (wait(&status) != pid)
        ;
}