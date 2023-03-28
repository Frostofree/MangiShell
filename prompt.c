#include "prompt.h"

#include "headers.h"

void PrintPrompt(int flag, char *user, char *root, char *curdir, char *hostname)
{
    char path[1024] = " "; // extracting relative path from the absolute one
    // int i = 0;
    // while(1)
    // {
    //     if(root[i] == '\0')   // We are putting ~ the moment root terminates, to preserve the relative path, as home is the directory on which we start shell in
    //     {
    //         path[0] = '~';
    //         break;
    //     }
    //     else if(curdir[i] == '\0' | curdir[i] != root[i]) // if curdir goes a level above root
    //     {
    //         break;
    //     }
    //     i++;
    // }

    if (strcmp(root, curdir) <= 0)
    {
        path[0] = '~';
    }

    if (path[0] != '~') // the above home dir case
    {
        strcpy(path, curdir);
    }
    else
    {
        strcat(path, curdir + strlen(root)); // append the charecters after the root part, path already has '~'
    }

    if (flag > 0)
    {
        printf("<\x1B[32m%s@%s\x1B[0m:\x1B[36m%s took %d seconds\x1B[0m>", user, hostname, path, flag);
    }
    else
    {
        printf("<\x1B[32m%s@%s\x1B[0m:\x1B[36m%s\x1B[0m>", user, hostname, path);
    }

    fflush(stdout);
}