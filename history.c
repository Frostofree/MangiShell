#include "headers.h"

/*if fopen returns NULL, then the file is inaccessible for some reason, not because the file doesn't exist
    The file will be created if not exists*/

int readhistory(char *root, char **commands)
{
    char path_name[1024];
    size_t size_of_input = 1024;
    int no_of_commands = 0;

    commands[0] = NULL; // resetting
    sprintf(path_name, "%s/.history", root);
    FILE *f = fopen(path_name, "r+");

    if (f != NULL)
    {
        for (int i = 0; i < 20; i++)
        {
            commands[i + 1] = NULL;
            if (getline(&commands[i], &size_of_input, f) > 0)
            {
                int len = strlen(commands[i]);
                commands[i][len-1] = '\0';
                no_of_commands++; // Reached EOF
            }
            else
            {
                break;
            }
        }
        fclose(f);
    }

    return no_of_commands;
}

void addhistory(char *root, char *input)
{
    int lim = 20;

    // Have to check if input string contains anything other than spaces and tabs, since these shouldn't be added
    // In order to do so, i will check for the first instance of a command which isn't ;," "."\t",&
    int flag_v = 0;
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] != ' ' && input[i] != '\t' && input[i] != ';' && input[i] != '&')
        {
            flag_v = 1;
            break;
        }
    }

    if (flag_v == 0)
    {
        return; // Do nothing
    }

    char **commands;
    commands = (char **)malloc(21 * sizeof(char *));

    for (int i = 0; i < 21; i++)
    {
        commands[i] = (char *)malloc(100 * sizeof(char));
        commands[i] = NULL;
    }
    int noofCommands = readhistory(root, commands);

    if (noofCommands > 0 && (strcmp(commands[noofCommands - 1], input) == 0))
    {
        return; // don't add if duplicate
    }

    char path_name[1024];
    sprintf(path_name, "%s/.history", root);
    FILE *f = fopen(path_name, "a");

    if (f)
    {
        if (noofCommands == 0)
        {
            fprintf(f, "%s\n", input);
            fclose(f);
        }
        else
        {
            if (noofCommands < lim)
            {
                fclose(f);
                FILE *f = fopen(path_name, "a");
                fprintf(f, "%s\n", input);
                fclose(f);
            }
            else
            {
                fclose(f);
                FILE *f = fopen(path_name,"w");
                for (int i = 1; i < lim; i++)
                {
                    fprintf(f, "%s\n", commands[i]);
                }
                fprintf(f, "%s\n", input);
                fclose(f);
            }
        }
    }

    for (int i = 0; i < 21; i++)
    {
        free(commands[i]);
    }
    free(commands);
}

void my_history(char *root)
{
    int lim = 10;
    char **commands;
    commands = (char **)malloc(21 * sizeof(char *));

    for (int i = 0; i < 21; i++)
    {
        commands[i] = (char *)malloc(100 * sizeof(char));
        commands[i] = NULL;
    }

    int noofCommands = readhistory(root, commands); // history file will contain 0 - 20 instructions, will create one function that gets number of instructions

    if (lim > noofCommands)
    {
        for (int i = 0; i < noofCommands; i++)
        {
            printf("%s\n", commands[i]);
        }
    }
    else
    {
        for (int i = noofCommands - lim; i < noofCommands; i++)
        {
            printf("%s\n", commands[i]);
        }
    }

    for (int i = 0; i < 21; i++)
    {
        free(commands[i]);
    }
    free(commands);
}
