#include "headers.h"

void my_bg(int pid)
{
    int p = kill(pid,SIGCONT);
    if( p < 0)
    {
        perror(RED "Error" RESET);
        return;
    }
}