#include "headers.h"
#include "prompt.h"
#include "input.h"
#include "echo.h"
#include "pwd.h"
#include "cd.h"
#include "ls.h"
#include "history.h"
#include "pinfo.h"
#include "discover.h"
#include "sig.h"
#include "fg.h"
#include "bg.h"

char *user;
size_t username_size = 1024;
char root[1024];
char curdir[1024]; // arbitrary size of array
char prvdir[1024]; // For cd -
char *sysname = NULL;
int no_of_bg = 0;
int cur = 1;

int bg_index = 0;

int fg_prompt = 0;

int cur_fg = 0;
char scur_fg[1024];

char *bg_names[1024];
int bg_ids[1024];

char *test;

size_t test_size = 32;

int fixstdin;
int fixstdout;
int output_fd = 0;
int input_fd = 0;
int redmode = -1; // redirection mode in which the shell must operate
int redflag = 0;

// char **subcommands[100];
char *subcommands[100];
char *pipedcommands[100];

void my_jobs(char **args, int noofargs)
{
    if (no_of_bg == 0)
    {
        printf("No Background Processes\n");
        return;
    }

    char temp[1024];
    if (noofargs > 1)
    {
        if (strcmp(args[1], "-r") == 0) // For running processes
        {
            for (int i = 0; i < no_of_bg; i++)
            {

                int shellpid = getpid();
                int ret = my_pinfo_jobs(bg_ids[i]);
                if (ret == 2)
                {
                    printf("[%d] Running %s [%d]\n", i + 1, bg_names[i], bg_ids[i]);
                }
            }
        }
        else if (strcmp(args[1], "-s") == 0) // For stopped
        {
            for (int i = 0; i < no_of_bg; i++)
            {
                int shellpid = getpid();
                int ret = my_pinfo_jobs(bg_ids[i]);
                if (ret == 1)
                {
                    printf("[%d] Stopped %s [%d]\n", i + 1, bg_names[i], bg_ids[i]);
                }
            }
        }
        else
        {
            printf(RED "Incorrect flag\n" RESET);
        }
    }
    else
    {
        for (int i = 0; i < no_of_bg; i++)
        {
            int ret = my_pinfo_jobs(bg_ids[i]);
            if (ret == 1)
            {
                printf("[%d] Stopped %s [%d]\n", i + 1, bg_names[i], bg_ids[i]);
            }
            else if (ret == 2)
            {
                printf("[%d] Running %s [%d]\n", i + 1, bg_names[i], bg_ids[i]);
            }
        }
    }
}

void childHandler() /* Executed if the child dies before the parent */
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        if (WIFSTOPPED(status))
        {
            return;
        }

        int index = -1;
        for (int i = 0; i < 1024; i++)
        {
            if (pid == bg_ids[i])
            {
                index = i;
                break;
            }
        }

        if (index < 0)
        {
            printf("There has been an Error\n");
            return;
        }
        /*
            WIFEXITED(status) = 0 // Normal
            WIFEXITED(status) = 1 // abnormal
        */
        fprintf(stderr, "\n%s with PID %d exited %s\n", bg_names[index], pid, WIFEXITED(status) ? "normally" : "abnormally"); // Checking the exit status
        fflush(stderr);
        PrintPrompt(0, user, root, curdir, sysname);
    }
}

void CtrlCHandler()
{
    if (cur_fg == 0) // No forgeround process
    {
        printf("\n");
        PrintPrompt(0, user, root, curdir, sysname);
    }
    else
    {
        // kill(cur_fg, SIGINT);
        cur_fg = 0;
        printf("\n");
    }
}

void CtrlZHandler()
{
    if (cur_fg == 0) // No forgeround process
    {
        printf("\n");
        PrintPrompt(0, user, root, curdir, sysname);
    }
    else
    {
        // kill(cur_fg,SIGCHLD);
        strcpy(bg_names[bg_index], scur_fg);
        bg_ids[bg_index] = cur_fg;
        bg_index++;
        no_of_bg++;
        cur_fg = 0;
        printf("\n");
    }
}

void execute(char **args, int noofargs)
{

    if (noofargs == 0)
    {
        return;
    }

    if (strcmp(args[0], "echo") == 0)
    {
        my_echo(args, noofargs);
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        my_pwd(curdir);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (noofargs > 2)
        {
            printf(RED "Error : Invalid number of arguments\n" RESET);
            return;
        }
        else if (noofargs == 1)
        {
            strcpy(prvdir, curdir);
            chdir(root); // cd to root
        }
        else
        {
            my_cdf(args, root, curdir, prvdir);
        }
    }
    else if (strcmp(args[0], "ls") == 0)
    {
        my_ls(args, root, curdir);
    }
    else if (strcmp(args[0], "pinfo") == 0)
    {
        if (noofargs > 2)
        {
            printf(RED "Too many arguments\n" RESET);
        }
        pid_t shell_pid = getpid();
        my_pinfo(args, shell_pid, root);
    }
    else if (strcmp(args[0], "history") == 0)
    {
        if (noofargs > 2)
        {
            printf(RED "Too many arguments\n" RESET);
            return;
        }
        my_history(root);
    }
    // else if (strcmp(args[0], "quit") == 0)
    // {
    //     if (noofargs > 2)
    //     {
    //         printf(RED "Too many arguments\n" RESET);
    //     }
    //     exit(0);
    // }
    else if (strcmp(args[0], "discover") == 0)
    {
        my_discover(args, root, curdir);
    }
    else if (strcmp(args[0], "jobs") == 0)
    {
        if (noofargs > 2)
        {
            printf(RED "Too many arguments\n" RESET);
            return;
        }
        my_jobs(args, noofargs);
    }
    else if (strcmp(args[0], "sig") == 0)
    {
        if (noofargs > 3)
        {
            printf(RED "Too many arguments\n" RESET);
            return;
        }

        int job_index = atoi(args[1]) - 1;
        int signal = atoi(args[2]);
        if (bg_ids[job_index] != -1)
        {
            my_sig(bg_ids[job_index], signal);
        }
        else
        {
            printf(RED "The job index does not exist\n" RESET);
        }
        // my_sig();
    }
    else if (strcmp(args[0], "fg") == 0)
    {
        if (noofargs > 2)
        {
            printf(RED "Too many arguments\n" RESET);
            return;
        }

        int job_index = atoi(args[1]) - 1;
        if (bg_ids[job_index] != -1)
        {
            my_fg(bg_ids[job_index]);
        }
        else
        {
            printf(RED "The job index does not exist\n" RESET);
        }
    }
    else if (strcmp(args[0], "bg") == 0)
    {
        if (noofargs > 2)
        {
            printf(RED "Too many arguments\n" RESET);
            return;
        }

        int job_index = atoi(args[1]) - 1;
        if (bg_ids[job_index] != -1)
        {
            my_bg(bg_ids[job_index]);
        }
        else
        {
            printf(RED "The job index does not exist\n" RESET);
        }
    }
    else
    {
        /*NEED to handle all bg calls of the form
            1. a b &
            2. a & b & c &

            Here, we have already filtered out the bg process in main, and have sent a certain number of arguments to work with
        */
        char *tempaa[256];
        tempaa[0] = NULL;
        for (int i = 0; i < noofargs; i++)
        {
            tempaa[i] = (char *)malloc(sizeof(char) * 256);
            strcpy(tempaa[i], args[i]);
            tempaa[i + 1] = NULL;
        }
        if (tempaa[noofargs - 1][strlen(tempaa[noofargs - 1]) - 1] == '&') // Background
        {
            tempaa[noofargs - 1][strlen(tempaa[noofargs - 1]) - 1] = '\0';

            if (strlen(tempaa[noofargs - 1]) == 0)
            {
                noofargs--;
            }

            tempaa[noofargs] = NULL;
            pid_t pid;
            int status;

            pid = fork();

            if (pid < 0)
            {
                perror("Error");
                return;
            }
            else if (pid == 0) // Child
            {
                int er = execvp(tempaa[0], tempaa);
                if (er < 0)
                {
                    perror("Error : ");
                    return;
                }
            }
            else // Parent
            {
                strcpy(bg_names[bg_index], args[0]);
                bg_ids[bg_index] = pid;
                bg_index++;
                printf("[%d] %d\n", cur, pid);
                cur++; // Return to parent, main stuff
                pid_t child_pid;
                while ((child_pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
                {
                }
            }
        }
        else // Foreground
        {
            pid_t pid;
            int status;

            time_t ti;
            time_t tf;

            pid = fork();

            cur_fg = pid;
            strcpy(scur_fg, args[0]);

            time(&ti);
            if (pid < 0) /*Fokring a child process,invalid*/
            {
                perror("Error :");
                cur_fg = 0;
                return;
            }
            else if (pid == 0) /*For child process, we will execute command in this*>*/
            {

                int er = execvp(tempaa[0], tempaa);
                if (er < 0)
                {
                    perror("Error : ");
                    return;
                }
            }
            else /*Make parent wait*/
            {
                while (waitpid(pid, &status, WUNTRACED) != pid)
                    ;
                time(&tf);
            }

            cur_fg = 0;

            // waitpid()
            if (redflag == 0)
            {
                PrintPrompt(tf - ti, user, root, curdir, sysname);
                fg_prompt = 1;
            }
        }

        for (int i = 0; i < noofargs; i++)
        {
            free(tempaa[i]);
        }
        // free(tempaa);
        // printf(RED "Invalid Command : '%s' does not exist\n" RESET, args[0]);
    }
}

int main()
{
    int noofargs;
    char *test1;
    char *args[256];

    char *token;

    signal(SIGCHLD, childHandler);
    signal(SIGINT, CtrlCHandler);  // Just interrupt
    signal(SIGTSTP, CtrlZHandler); // Stop typed at terminal

    // root = getenv("PWD");        // Present working directory
    // user = getenv("USER");       // Gives name of the user
    struct utsname unameData;   // structure describing system and machine
    register struct passwd *pw; // Record in user database
    register uid_t uid;
    int c;

    uid = geteuid();
    pw = getpwuid(uid);
    user = pw->pw_name;
    test1 = getcwd(root, 1024); // for root
    if (test1 == NULL)
    {
        printf(RED "Error : Buffer size insufficient\n" RESET);
    }
    // user = unameData.;

    if (uname(&unameData) < 0)
    {
        perror("uname");
        exit(1);
    }

    sysname = unameData.sysname;

    /*Initialising Global array*/
    // bg_names[0] = NULL;
    for (int i = 0; i < 1022; i++)
    {
        bg_names[i] = (char *)malloc(sizeof(char) * 1024);
        // bg_names[i+1] = NULL;
    }

    for (int i = 0; i < 1024; i++)
    {
        bg_ids[i] = -1;
    }

    printf(RED "\t\tInitiating MangiShell\t\t\n" RESET);
    while (1) // The shell Starts
    {
        int PFd[2][2];

        getcwd(curdir, 1024); // we need to get current working directory everytime

        if (!fg_prompt)
            PrintPrompt(0, user, root, curdir, sysname);

        fg_prompt = 0;
        char *input_line = input(subcommands);

        addhistory(root, input_line);

        /*RESETTING GLOBAL VARIABLES*/
        // for (int i = 0; i < 1024; i++)
        // {
        //     bg_ids[i] = -1;
        // }
        // for (int i = 0; bg_names[i] != NULL; i++)
        // {
        //     bg_names[i] = NULL;
        // }

        for (int i = 0; subcommands[i] != NULL; i++)
        {
            int is_bg = 0;
            for (int i = 0; i < 100; i++)
            {
                args[i] = NULL;
            }
            noofargs = 0; // resetting args
            // int subsubcommands = 0;
            // while (subcommands[i][subsubcommands] != NULL)
            // {
            //     subsubcommands++;
            // }

            // for (int j = 0; j < subsubcommands; j++)
            // {
            char *pipetoken;
            pipetoken = strtok(subcommands[i], "|");
            int pipeargs = 0;
            while (pipetoken != NULL)
            {
                pipedcommands[pipeargs] = pipetoken;
                pipeargs++;
                pipedcommands[pipeargs] = NULL;
                pipetoken = strtok(NULL, "|");
            }

            for (int j = 0; pipedcommands[j] != NULL; j++)
            {

                for (int l = 0; l < 100; l++)   // The actual place where ags are meant to be resetted
                {
                    args[l] = NULL;
                }
                noofargs = 0; // resetting args

                output_fd = 0;
                input_fd = 0;
                redmode = -1; // redirection mode in which the shell must operate

                // redmode = 0 // "<" symbol, only reading, also after reading, we get input fd from next argument
                // redmode = 1 // ">" symbol, write mode, not append. get output fd from next argument
                // redmode = 2 // ">>" symbol, append mode, get output fd from next argument

                /*
                Piping essentially makes use of virtual file descriptors to redirect input output
                For reference1 : https://www.youtube.com/watch?v=Mqb2dVRe0uo
                For reference2 : https://pubs.opengroup.org/onlinepubs/009604599/functions/pipe.html
                each pipe has two modes, read (fd[0]) and write (fd[1])
                when a pipe fd is opened, the default values of fd[0] and fd[1] is 0(stdin) and 1(stdout)
                In a piping command a | b | c , b only takes output of a as input, c only takes output of b as input
                when we're on the c command, i can just close the pipe of a, and reuse it as pipe for c
                I can be done with the pipes in just one 2D pipe array PFd[2][2]
                I can use the indexing of the pipedargs to sort of alternate between the first pipe and second one
                */
                if (pipeargs > 1 && j < pipeargs - 1) // For last pipearg c, don't open a new pipe
                {
                    if (pipe(PFd[j % 2]) < 0)
                    {
                        perror(RED "Error" RESET);
                        break; // There has been a pipe opening error, we will be skipping this ';' seperated command
                    }
                    output_fd = PFd[j % 2][1]; // for this process, output_fd will be out
                }

                if (pipeargs > 1 && j > 0)
                {
                    input_fd = PFd[1 - j % 2][0]; // the preceding pipe's output is this current pipe's input
                }

                token = strtok(pipedcommands[j], " \t");
                while (token != NULL)
                {
                    if (redmode > -1)
                    {
                        if (redmode == 0) // "read"
                        {
                            input_fd = open(token, O_RDONLY); // only open read only file
                        }
                        else if (redmode == 1) // write, also truncate
                        {
                            output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        }
                        else if (redmode == 2)
                        {
                            output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0644);
                        }

                        redmode = -1; // important, as we need to take more io operations
                    }
                    else if (strcmp(token, "<") == 0) // readmode
                    {
                        redmode = 0;
                    }
                    else if (strcmp(token, ">") == 0) // truncate mode
                    {
                        redmode = 1;
                        redflag = 1;
                    }
                    else if (strcmp(token, ">>") == 0) // append mode
                    {
                        redmode = 2;
                        redflag = 1;
                    }
                    else
                    {
                        args[noofargs] = token;
                        noofargs++;
                        args[noofargs] = NULL;
                    }
                    token = strtok(NULL, " \t");
                }

                // for(int p = 0; p < noofargs ; p++)
                // {
                //     printf("%s\n",args[p]);
                // }

                if (input_fd < 0 || output_fd < 0)
                {
                    perror(RED "Error " RESET);
                    continue; // fd does not exist or some shit, continue
                }

                if (input_fd != 0)
                {
                    fixstdin = dup(0);
                    dup2(input_fd, 0);
                }

                if (output_fd != 0)
                {
                    fixstdout = dup(1);
                    dup2(output_fd, 1); // all stdout into output_fd
                }

                int f_comm = 0;
                // no_of_bg = 0;
                for (int z = 0; z < noofargs; z++)
                {
                    if (args[z][strlen(args[z]) - 1] == '&')
                    {
                        execute(&args[f_comm], z - f_comm + 1);
                        f_comm = z + 1;
                        no_of_bg++;
                    }
                }
                if (f_comm < noofargs)
                {
                    execute(&args[f_comm], noofargs - f_comm);
                }

                // bg_index = 0; // resetting

                // token = strtok(subcommands[i][j], " \t");
                //}

                if (input_fd != 0)
                {
                    close(input_fd);
                    dup2(fixstdin, 0);
                    close(fixstdin);
                }
                if (output_fd != 0)
                {
                    close(output_fd);
                    dup2(fixstdout, 1);
                    close(fixstdout);
                }

                redflag = 0;

                /*Closing pipes:
                  Closing pipes is an important step, because the preceding pipes need to be open when current process is executing
                  We close current pipe as we finish it
                */
                 if(pipeargs > 1 && j < pipeargs -1 )         // close outfds except the last one
                {
                   close(PFd[j%2][1]);
                }
                if (pipeargs > 1 && j > 0)
                {
                    close(PFd[1 - j % 2][0]); // closing preceding pipe's input
                }
            }

            subcommands[i] = NULL; // resetting
        }
    }
}
