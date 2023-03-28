#include "headers.h"

void my_sig(int pid, int signal)
{
    int p = kill(pid,signal);
    if(p < 0)
    {
        perror(RED "Error" RESET);
        return;
    }
}