#include "headers.h"

void my_pinfo(char **args, int shellpid,char* root)
{
    int noofargs = 0;
    for (int i = 0; args[i] != NULL; i++)
    {
        noofargs++;
    }

    char pid_path[1024];
    char stat_path[1024];

    if (noofargs == 1)
    {
        sprintf(pid_path,"/proc/%d/exe",shellpid);
        sprintf(stat_path, "/proc/%d/stat", shellpid);
    }
    else if (noofargs == 2)
    {
        if(getpgid(atoi(args[1])) < 0)              // Surefire way to check if process exists or not
        {
            printf(RED "Error : Process does not exist\n" RESET);
            return;
        }   
        sprintf(pid_path,"/proc/%s/exe",args[1]); 
        sprintf(stat_path, "/proc/%s/stat", args[1]);
    }
    else
    {
        return; // ERROR
    }

    FILE *f = fopen(stat_path,"r");
    char *temp = NULL;
    size_t size_of_input = 1024; // Assuming

    int len = getline(&temp, &size_of_input, f);

    if (len < 0)
    {
        printf(RED "\n Error.\n" RESET);
        exit(0);
    }

    int no_of_fields = 0;

    temp[len - 1] = '\0' ;
    char *tempcomm[100]; // can handle 100 subinstructions
    tempcomm[0] = NULL;  // initialize it initially as NULL

    char *token = strtok(temp, " "); // All semicolon seperated instructions

    for (int i = 0; token != NULL; i++)
    {
        tempcomm[i] = token;
        tempcomm[i + 1] = NULL;    // for safety purposes
        token = strtok(NULL, " "); // Use same string
        no_of_fields++;
    }

    char pid[10];
    sprintf(pid,"%s",tempcomm[0]);
    char process_status[10];
    sprintf(process_status,"%s",tempcomm[2]);
    int tpgid_fgp = atoi(tempcomm[7]);
    int grp_pid = atoi(tempcomm[4]);
    if(tpgid_fgp == grp_pid)
    {
        strcat(process_status,"+");
    }
    char memory[20];
    sprintf(memory,"%s",tempcomm[22]);
    fclose(f);
    char exec_path[1024];
    int plen = readlink(pid_path,exec_path,1024);       //Reads value of symbolic link  
    if(plen < 0)
    {
        perror(RED "Error :" RESET);
        return;
    }
    else
    {
        exec_path[plen] = '\0';
        if(strncmp(root,exec_path,strlen(root)) == 0){
            char tempexec[1000];
            tempexec[0]='~';
            tempexec[1]='/';
            strcpy(&tempexec[2],&exec_path[strlen(root)+1]);
            strcpy(exec_path,tempexec);
        }
    }

    printf("pid : %s\n",pid);
    printf("process status : %s\n",process_status);
    printf("memory : %s\n",memory);
    printf("executable path : %s\n",exec_path);


}


int my_pinfo_jobs(int pid)
{
    char stat_path[1024];
    sprintf(stat_path,"/proc/%d/stat",pid);
    FILE *f = fopen(stat_path,"r");
    if(f == NULL)
    {
        return 3;       
        //neoither stopped nor running
    }
    char *temp = NULL;
    size_t size_of_input = 1024; // Assuming

    int len = getline(&temp, &size_of_input, f);

    if (len < 0)
    {
        printf(RED "\n Error.\n" RESET);
        exit(0);
    }

    int no_of_fields = 0;

    temp[len - 1] = '\0' ;
    char *tempcomm[100]; // can handle 100 subinstructions
    tempcomm[0] = NULL;  // initialize it initially as NULL

    char *token = strtok(temp, " "); // All semicolon seperated instructions

    for (int i = 0; token != NULL; i++)
    {
        tempcomm[i] = token;
        tempcomm[i + 1] = NULL;    // for safety purposes
        token = strtok(NULL, " "); // Use same string
        no_of_fields++;
    }

    char process_status[10];
    sprintf(process_status,"%s",tempcomm[2]);

    // printf("pid : %s\n",pid);
    // printf("process status : %s\n",process_status);
    // printf("memory : %s\n",memory);
    // printf("executable path : %s\n",exec_path);

    if(strcmp(process_status,"T") == 0)
    {
        return 1;///its stopped
    }
    else if(strcmp(process_status,"R") == 0 || strcmp(process_status,"S")==0)
    {
        return 2;//its running
    }


}