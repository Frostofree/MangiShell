#include "headers.h"
#include "ls.h"
#include <libgen.h>

/*HANDLED ~ case
 */

int pstrcmp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void my_ls_print(int flag_d, int flag_a, int flag_l, char *root, char *dir)
{

    char *path_name = (char *)malloc(2048 * sizeof(char));
    char *cur_name = (char *)malloc(1024 * sizeof(char));
    getcwd(cur_name, 1024);
    if (dir[0] == '~')
    {
        if (strlen(dir) == 1)
        {
            strcpy(path_name,root);
        }

        if (dir[1] == '/')
        {
            sprintf(path_name, "%s%s", root, dir + 1);
        }
    }
    else if (dir[0] == '.')
    {
        if (strcmp(dir, ".") == 0) // current folder
        {
            getcwd(path_name, 1024);
        }
        else if (strcmp(dir, "..") == 0) // previous folder, FIX FOR RELATIVE HOME DIR
        {
            sprintf(path_name, "%s", root);
            chdir("..");
            getcwd(path_name, 1024);
            chdir(cur_name); // for safety
        }
        else // in path for hidden folders
        {
            sprintf(path_name, "%s/%s", cur_name, dir);
        }
    }
    else if(strcmp(dir,"/") == 0)
    {
        sprintf(path_name,"/");
    }
    else
    {
        sprintf(path_name, "%s/%s", cur_name, dir);
    }

    char tempfile[4096];
    // -a is basically for hidden files

    if (flag_d == 1)
    {

        struct dirent *de; // Pointer for directory entry
        DIR *panji = opendir(path_name);
        if (panji == NULL)
        {
            perror(RED "Error" RESET);
            free(path_name);
            free(cur_name);
            return;
        }
        char *list[1024];
        int no_of_dirfiles = 0;
        list[no_of_dirfiles] = NULL;
        while ((de = readdir(panji)) != NULL)
        {
            if (flag_a == 0 && de->d_name[0] == '.')
            {
                continue; // skipping hidden folders
            }

            list[no_of_dirfiles] = (char *)malloc(sizeof(char) * 1024);
            strcpy(list[no_of_dirfiles], de->d_name);
            list[no_of_dirfiles + 1] = NULL;
            no_of_dirfiles++;
        }

        // for (int i = 0; i < no_of_dirfiles; i++)
        // {
        //     printf("%s\n", list[i]);
        // }

        qsort(list, no_of_dirfiles, sizeof(char *), pstrcmp);

        // for (int i = 0; i < no_of_dirfiles; i++)
        // {
        //     printf("%s\n", list[i]);
        // }

        for (int i = 0; i < no_of_dirfiles; i++)
        {
            sprintf(tempfile, "%s/%s", path_name, list[i]); // contains path of file
            // printf("%s\n", tempfile);
            struct stat file_stat; // File details
            if (lstat(tempfile, &file_stat) < 0)
            {
                perror("Error :");
                free(path_name);
                free(cur_name);
                return;
            }

            if (flag_l) // contains -l stuff, permissions, all that stuff
            {
                /*------------------PERMISSIONS------------------------*/
                printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

                printf("\t");
                /*----------------OTHER STUFF--------------------------*/
                printf("%ld\t", file_stat.st_nlink);
                printf("%s\t", getpwuid(file_stat.st_uid)->pw_name);
                printf("%s\t", getgrgid(file_stat.st_gid)->gr_name);
                printf("%ld\t", file_stat.st_size);
                struct tm *time_date = localtime(&(file_stat.st_ctim.tv_sec));
                char temptime[100];
                char month[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
                sprintf(temptime, "%s\t%d\t%d\t", month[time_date->tm_mon], time_date->tm_mday, time_date->tm_year + 1900);
                printf("%s\t", temptime);
            }

            if (file_stat.st_mode & S_IFDIR) // Directory
            {
                printf(BLU "%s" RESET, list[i]);
            }
            else if (file_stat.st_mode & S_IXUSR) // Executable
            {
                printf(GRN "%s" RESET, list[i]);
            }

            else
            {
                printf("%s", list[i]);
            }

            if (flag_l == 1)
            {
                printf("\n"); // Cuz permissions, new line stuff
            }
            else
            {
                printf(" ");
            }
        }
        free(de);
        closedir(panji);         // aLSO VERY iMPORTANT
        for(int i = 0; i < no_of_dirfiles;i++)
        {
            free(list[i]);
        }
    }
    else
    {
        // sprintf(tempfile, "%s/%s", path_name, dir); // contains path of file
        struct stat file_stat; // File details
        if (lstat(path_name, &file_stat) < 0)
        {
            perror("Error :");
            free(path_name);
            free(cur_name);
            return;
        }

        if (flag_l) // contains -l stuff, permissions, all that stuff
        {
            /*------------------PERMISSIONS------------------------*/
            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

            printf("\t");
            /*----------------OTHER STUFF--------------------------*/
            printf("%ld\t", file_stat.st_nlink);
            printf("%s\t", getpwuid(file_stat.st_uid)->pw_name);
            printf("%s\t", getgrgid(file_stat.st_gid)->gr_name);
            printf("%ld\t", file_stat.st_size);
            struct tm *time_date = localtime(&(file_stat.st_ctim.tv_sec));
            char temptime[100];
            char month[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
            sprintf(temptime, "%s\t%d\t%d\t", month[time_date->tm_mon], time_date->tm_mday, time_date->tm_year + 1900);
            printf("%s\t", temptime);
        }

        char *name;
        name = basename(path_name);
        printf("%s", name);

        if (flag_l == 1)
        {
            printf("\n"); // Cuz permissions, new line stuff
        }
        else
        {
            printf(" ");
        }
    }

    if (flag_l == 0)
    {
        printf("\n");
    }

    free(path_name);
    free(cur_name);
}

/*HAVE TO HANDLE THE SINGULAR FILES*/

void my_ls(char **args, char *root, char *curdir)
{
    int flag_a = 0;
    int flag_l = 0;
    int flag_d = 0; // flag for directories
    int n = 0;
    int no_of_args = 0;
    int first_index = 0;
    int no_of_dirfile = 0;

    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] == '-') // Searching for the flags first
        {
            if (args[i][1] == 'a' || args[i][2] == 'a')
            {
                flag_a = 1;
            }

            if (args[i][1] == 'l' || args[i][2] == 'l')
            {
                flag_l = 1;
            }
        }
        else // Searching for the most recent directory to work with
        {
            first_index = i;
            no_of_args++; // storing this to tell if it is normal ls or others
        }
    }

    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] == '-')
        {
            continue;
        }
        else
        {
            no_of_dirfile++;
        }
    }

    struct stat file_stat;
    char pop[1024];

    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] == '-')
        {
            continue;
        }

        sprintf(pop, "%s/%s", curdir, args[i]);
        int flag_tilda = 0;
        if (args[i][0] == '~')
        {
            flag_tilda = 1;
            strcpy(pop, root);
        }

        if (lstat(pop, &file_stat) < 0)
        {
            perror("Error :");
            return;
        }

        n = 1;

        if (file_stat.st_mode & S_IFDIR)
        {
            flag_d = 1;
        }

        if (no_of_dirfile > 1)
        {
            printf("%s:\n", args[i]);
        }

        my_ls_print(flag_d, flag_a, flag_l, root, args[i]);
    }

    if (!n) // contents of current file
    {
        my_ls_print(1, flag_a, flag_l, root, ".");
    }
}
