#include "headers.h"
#include "cd.h"

/*
        1. cd ./__________
        2. cd ________
        3. cd ..
        4. cd .
        5. cd ~
        6. cd -
        7. cd ~/_______

*/

void my_cdf(char **args, char *root, char *curdir, char *prvdir)
{
    char newpath[1024];

    if (args[1][0] == '.') // cases 1,3,4 , do nothing
    {
        if (strcmp(args[1], ".") == 0) // Case 4
        {
            strcpy(prvdir, curdir); // storing curdir in prvdir
            return;
        }
        else if (strcmp(args[1], "..") == 0) // Case 3
        {
            // if (strcmp(curdir, root) <= 0)
            // {
            //     strcpy(curdir, root);
            //     chdir(root);
            //     printf("%s\n",root);
            //     return;
            // }
            strcpy(newpath, prvdir);
            strcpy(prvdir, curdir);
            int l = chdir("..");
            if (l < 0)
            {
                strcpy(curdir, prvdir);  // restore curdir
                strcpy(prvdir, newpath); // restore the prvdir
                return;
            }
            getcwd(curdir, 1024); // updating cwd
            // if (strcmp(curdir, root) <= 0)
            // {
            //     strcpy(curdir, root);
            //     chdir(root);
            //     return;
            // }
        }
        else // Case where directory path has . as first char
        {
            // strcpy(newpath, curdir);      // put curdir
            // strcat(newpath, args[1] + 1); // concatenating part after . onto curdir

            sprintf(newpath, "%s/%s", curdir, args[1]);

            char safety[1024];
            strcpy(safety, prvdir); // copying prvdir into safety, in case chdir fails
            strcpy(prvdir, curdir); // storing prvdir
            int l = chdir(newpath);
            if (l < 0)
            {
                strcpy(prvdir, safety);
                printf(RED "Error : Directory Does not exist\n" RESET);
                return;
            }
            getcwd(curdir, 1024); // updating cwd
            // if (strcmp(curdir, root) <= 0)
            // {
            //     strcpy(curdir, root);
            //     chdir(root);
            //     return;
            // }
        }
    }
    else if (strcmp(args[1], "-") == 0)
    {
        strcpy(newpath, curdir);
        strcpy(curdir, prvdir);  // changing curdir to prvdir
        strcpy(prvdir, newpath); // making prv dir curdir

        int l = chdir(curdir);
        if (l < 0)
        {
            printf(RED "Error : Directory does not exist\n" RESET);
            // restoring previous contents
            strcpy(newpath, curdir);
            strcpy(curdir, prvdir);  // changing curdir to prvdir
            strcpy(prvdir, newpath); // making prv dir curdir
            return;
        }
        else
        {
            printf("%s\n", curdir); // print new path
        }
    }
    else if (args[1][0] == '~')
    {
        if (strcmp(args[1], "~") == 0) // case 5
        {
            strcpy(newpath, prvdir); // in case chdir fails
            strcpy(prvdir, curdir);
            int l = chdir(root);

            if (l < 0)
            {
                strcpy(prvdir, newpath); // restore prv paths
                printf(RED "Error : Directory does not exist\n" RESET);
                return;
            }
            getcwd(curdir, 1024);
        }
        else // for case 7
        {
            strcpy(newpath, root);
            strcat(newpath, args[1] + 1); // skipping the ~ part in argument

            char safety[1024];
            strcpy(safety, prvdir); // copying prvdir into safety, in case chdir fails
            strcpy(prvdir, curdir); // storing prvdir
            int l = chdir(newpath);
            if (l < 0)
            {
                strcpy(prvdir, safety);
                printf(RED "Error : Directory Does not exist\n" RESET);
                return;
            }
            getcwd(curdir, 1024); // updating cwd
            // if (strcmp(curdir, root) <= 0)
            // {
            //     strcpy(curdir, root);
            //     chdir(root);
            //     return;
            // }
        }
    }
    else if (args[1][0] == '/')
    {
        char safety[1024];
        strcpy(safety, prvdir);
        strcpy(prvdir, curdir);
        strcpy(newpath,args[1]);
        int l = chdir(newpath);
        if (l < 0)
        {
            strcpy(prvdir, safety);
            printf(RED "Error : Directory Does not exist\n" RESET);
            return;
        }
        getcwd(curdir, 1024);
    }
    else // Only chars dir
    {
        sprintf(newpath, "%s/%s", curdir, args[1]); // made newpath

        char safety[1024];
        strcpy(safety, prvdir); // copying prvdir into safety, in case chdir fails
        strcpy(prvdir, curdir); // storing prvdir
        int l = chdir(newpath);
        if (l < 0)
        {
            strcpy(prvdir, safety);
            printf(RED "Error : Directory Does not exist\n" RESET);
            return;
        }
        getcwd(curdir, 1024); // updating cwd
        // if (strcmp(curdir, root) <= 0)
        // {
        //     strcpy(curdir, root);
        //     return;
        // }
    }
}