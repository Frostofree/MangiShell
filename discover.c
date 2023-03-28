#include "headers.h"
#include "discover.h"

int pstrcmpa(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void my_discovery_recursive(int is_dir_temp, int flag_d, int flag_f, char *pop, char *dir, char *rel_path)
{
    char temp_rel[1024];
    strcpy(temp_rel, rel_path);
    char tempfile[4096];

    char *files_dirs[256];

    if (is_dir_temp == 1)
    {
        struct dirent *de; // Pointer for directory entry
        DIR *panji = opendir(pop);
        if (panji == NULL)
        {
            return;
        }
        int no_of_dirfiles = 0;
        while ((de = readdir(panji)) != NULL)
        {
            if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            {
                continue;
            }
            files_dirs[no_of_dirfiles] = (char *)malloc(sizeof(char) * 256);
            strcpy(files_dirs[no_of_dirfiles], de->d_name);
            files_dirs[no_of_dirfiles + 1] = NULL;
            no_of_dirfiles++;
        }
        // qsort(files_dirs, no_of_dirfiles, sizeof(char) * 256, pstrcmpa);
        struct stat file_stat_temp;
        char npopo[4096];

        for (int i = 0; i < no_of_dirfiles; i++)
        {
            sprintf(tempfile, "%s/%s", rel_path, files_dirs[i]);
            // printf("%s\n", tempfile);
            sprintf(npopo, "%s/%s", pop, files_dirs[i]);
            if (lstat(npopo, &file_stat_temp) < 0)
            {
                perror("Error :");
                return;
            }
            if (flag_d == 1 && flag_f == 1)
            {
                printf("%s\n", tempfile);
            }
            else if (flag_d == 1)
            {
                if (file_stat_temp.st_mode & S_IFDIR)
                {
                    printf("%s\n", tempfile);
                }
            }
            else if (flag_f == 1)
            {
                if (file_stat_temp.st_mode & S_IFDIR)
                {
                    // Do Nothing
                }
                else
                {
                    printf("%s\n", tempfile);
                }
            }
        }

        for (int i = 0; i < no_of_dirfiles; i++)
        {
            sprintf(npopo, "%s/%s", pop, files_dirs[i]);
            if (lstat(npopo, &file_stat_temp) < 0)
            {
                perror("Error :");
                return;
            }
            if (file_stat_temp.st_mode & S_IFDIR)
            {
                is_dir_temp = 1;
                strcat(rel_path, "/");
                strcat(rel_path, files_dirs[i]);
                my_discovery_recursive(is_dir_temp, flag_d, flag_f, npopo, files_dirs[i], rel_path);
            }
        }
    }
    else
    {
        if (flag_f == 1)
        {
            sprintf(tempfile, "%s/%s", rel_path, dir);
            printf("%s\n", tempfile);
        }
    }

    return;
}

void my_discovery_print(int is_dir, int flag_d, int flag_f, char *root, char *dir)
{
    char curdird[1024];
    getcwd(curdird,1024);
    int is_dir_temp = 0;
    char *path_name = (char *)malloc(2048 * sizeof(char));
    char *cur_name = (char *)malloc(1024 * sizeof(char));
    getcwd(cur_name, 1024);
    char rel_path[1024];
    strcpy(rel_path, dir);

    if (dir[0] == '~')
    {
        if (strlen(dir) == 1)
        {
            sprintf(path_name, "%s", root);
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
            chdir("..");
            getcwd(path_name, 1024);
            chdir(cur_name); // for safety
        }
        else // in path for hidden folders
        {
            sprintf(path_name, "%s/%s", cur_name, dir);
        }
    }
    else
    {
        sprintf(path_name, "%s/%s", cur_name, dir);
    }

    char tempfile[4096];

    char *files_dirs[256];

    if (is_dir == 1)
    {
        struct dirent *de; // Pointer for directory entry
        DIR *panji = opendir(path_name);
        if (panji == NULL)
        {
            // perror(RED "Error" RESET);
            free(path_name);
            free(cur_name);
            return;
        }
        int no_of_dirfiles = 0;
        while ((de = readdir(panji)) != NULL)
        {
            if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            {
                continue;
            }
            files_dirs[no_of_dirfiles] = (char *)malloc(sizeof(char) * 256);
            strcpy(files_dirs[no_of_dirfiles], de->d_name);
            files_dirs[no_of_dirfiles + 1] = NULL;
            no_of_dirfiles++;
        }
        // qsort(files_dirs, no_of_dirfiles, sizeof(char) * 256, pstrcmpa);

        struct stat file_stat_temp;
        char pop[4096];

        printf("%s\n", dir);
        for (int i = 0; i < no_of_dirfiles; i++)
        {
            sprintf(pop, "%s/%s", path_name, files_dirs[i]);
            if (lstat(pop, &file_stat_temp) < 0)
            {
                perror("Error :");
                return;
            }
            sprintf(tempfile, "%s/%s", dir, files_dirs[i]);
            if (flag_f == 1 && flag_d == 1)
            {
                printf("%s\n", tempfile);
            }
            else if (flag_f == 1)
            {
                if (file_stat_temp.st_mode & S_IFDIR)
                {
                    // Do Nothing
                }
                else
                {
                    printf("%s\n", tempfile);
                }
            }
            else if (flag_d == 1)
            {
                if (file_stat_temp.st_mode & S_IFDIR)
                {
                    printf("%s\n", tempfile);
                }
            }
        }

        for (int i = 0; i < no_of_dirfiles; i++)
        {
            sprintf(pop, "%s/%s", path_name, files_dirs[i]);
            if (lstat(pop, &file_stat_temp) < 0)
            {
                perror("Error :");
                return;
            }
            if (file_stat_temp.st_mode & S_IFDIR)
            {
                is_dir_temp = 1;
                strcat(rel_path, "/");
                strcat(rel_path, files_dirs[i]);
                my_discovery_recursive(is_dir_temp, flag_d, flag_f, pop, files_dirs[i], rel_path);
                strcpy(rel_path, dir);
            }
        }
    }
    else
    {
        if (flag_f == 1)
        {
            printf("%s\n", dir);
        }
    }
}

void my_discover(char **args, char *root, char *curdir)
{
    int flag_f = 0;
    int flag_d = 0;
    int is_dir = 0;
    int n = 0;
    int no_of_args = 0;
    int first_index = 0;
    int no_of_dirfile = 0;

    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] == '-') // Searching for the flags first
        {
            if (args[i][1] == 'd')
            {
                flag_d = 1;
                no_of_args++;
            }

            if (args[i][1] == 'f')
            {
                flag_f = 1;
                no_of_args++;
            }
        }
        else // Searching for the most recent directory to work with
        {
            first_index = i;
             // storing this to tell if it is normal ls or others
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
        }
        if (!flag_tilda && lstat(pop, &file_stat) < 0)
        {
            perror("Error :");
            return;
        }

        n = 1;

        if (file_stat.st_mode & S_IFDIR)
        {
            is_dir = 1;
        }

        // if (no_of_dirfile > 1)
        // {
        //     printf("%s:\n", args[i]);
        // }
        if (flag_d == 0 && flag_f == 0)
        {
            my_discovery_print(is_dir, 1, 1, root, args[i]);
        }
        else
        {
            my_discovery_print(is_dir, flag_d, flag_f, root, args[i]);
        }
    }

    if (!n) // contents of current file
    {
        if (no_of_dirfile == 0)
        {
            my_discovery_print(1, flag_d, flag_f, root, ".");
        }
        else
        {
            my_discovery_print(1, 1, 1, root, ".");
        }
    }
}