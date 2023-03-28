#include "headers.h"
#include "input.h"

#include "prompt.h"

//char* input(char ***subcommands)
char* input(char **subcommands)
{

    char *temp = NULL;
    size_t size_of_input = 1024;     // Assuming
    
    int len  = getline(&temp,&size_of_input,stdin);

    if(len < 0)
    {
        printf(RED "\n Ctrl-D Detected \n" RESET);
        exit(0);
    }

    temp[len - 1] = '\0' ;   // removing newline charecter

    //printf("%s\n",temp);

    // if(len == 1) // empty string
    // {
    //     return NULL;
    // }

    char* tempcomm[100]; // can handle 100 subinstructions
    tempcomm[0] = NULL; // initialize it initially as NULL

    char* token = strtok(temp,";");   // All semicolon seperated instructions

    for(int i = 0; token != NULL;i++)
    {
        tempcomm[i] = token;
        tempcomm[i+1] = NULL; // for safety purposes
        token = strtok(NULL,";"); // Use same string
    }

    subcommands[0] = NULL;

    for(int i = 0; tempcomm[i] != NULL;i++)
    {
        subcommands[i+1] = NULL;
        //subcommands[i] = (char*)malloc(100*sizeof(char));
        subcommands[i] = tempcomm[i];
    }    

    // for(int i = 0; tempcomm[i] != NULL;i++)
    // {
    //     subcommands[i+1] = NULL;
    //     subcommands[i] = (char**)malloc(100*sizeof(char*));
    //     subcommands[i][0] = NULL; // it's a 3d array now , break this argument down to smaller parts

    //     char* token_l = strtok(tempcomm[i],"&"); // tokenise on this

    //     for(int j = 0; token_l != NULL;j++)
    //     {
    //         subcommands[i][j+1] = NULL;
    //         subcommands[i][j] = token_l;
    //         token_l = strtok(NULL,"&");
    //     } 
    // }

    return temp;
}