#include "headers.h"
#include "echo.h"
void my_echo(char **args,int noofargs)
{
    // int i = 1;
    for(int i = 1; i < noofargs;i++)
    {
        printf("%s ",args[i]);
    }

    printf("\n");
}