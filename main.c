#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
  
#define MAXCOM 1000
#define MAXLIST 100
  
#define clear() printf("\033[H\033[J")

void init_shell()
{
    clear();
    printf("\033[0;32m");
    printf("\tBear-Shell (C Port)\n");
    printf("\tMichael S.");
    printf("\033[0m");
    char* username = getenv("USER");
    printf("\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}
  
int takeInput(char* str)
{
    char* buf;
    
    printf("\033[0;34m");
    buf = readline("\n└─[Bear-Shell]#:-> \033[0;94m");
    printf("\033[0;93m");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

int debug()
{
    char path[100]="PATH=";
    char* folder = "/home/michaelbear/";
    putenv(strcat(path, folder));
    char* pPath;
    pPath = getenv("PATH");
    printf("%s", pPath);
    return 1;
}

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\033[0;92m");
    printf("\n┌─[Directory] -> \033[0;32m%s", cwd);
    printf("\033[0m");
}
  
void execArgs(char** parsed)
{
    /*
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char* PATH = getenv("PATH");
    char* SHELL = getenv("SHELL");
    char* HOME = getenv("HOME");
    char* TERM = getenv("TERM");
    char* PWD = getenv("PWD");
    char** env = {
        "PATH",
        PATH,
        "SHELL",
        SHELL,
        "HOME",
        HOME,
        "TERM",
        TERM,
        "PWD",
        PWD
    };
    */

    pid_t pid = fork(); 
  
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        wait(NULL); 
        return;
    }
}
  
void execArgsPiped(char** parsed, char** parsedpipe)
{
    /*
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char* PATH = getenv("PATH");
    char* SHELL = getenv("SHELL");
    char* HOME = getenv("HOME");
    char* TERM = getenv("TERM");
    char* PWD = getenv("PWD");
    char** env = {
        "PATH",
        PATH,
        "SHELL",
        SHELL,
        "HOME",
        HOME,
        "TERM",
        TERM,
        "PWD",
        PWD
    };
    */

    int pipefd[2]; 
    pid_t p1, p2;
  
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }
  
    if (p1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
  
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        p2 = fork();
  
        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }
  
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            wait(NULL);
            wait(NULL);
        }
    }
}

void openHelp()
{
    puts("\nBear-Shell (C Port)"
        "\nList of Commands supported:"
        "\ncd"
        "\nls"
        "\nexit"
        "\n! all other general commands available in UNIX shell"
        "\n- pipe handling"
        "\n- improper space handling");
  
    return;
}
  
void customCommands()
{
     puts("\nBear-Shell (C Port)"
        "\nCustom Bear-Shell Commands"
        "\n(COMMANDS MUST CREATED IN C)"
        "\n(DO THIS AT YOUR OWN RISK!)"
        "\n(In Progress)");

    return;
}

int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 8, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;
    char* github = "https://github.com/BizzyPythonBear\0";

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "github";
    ListOfOwnCmds[5] = "custom-commands";
    ListOfOwnCmds[6] = "debugger-thing";
    ListOfOwnCmds[7] = "shell";
  
    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }
  
    switch (switchOwnArg) {
    case 1:
        printf("\nGoodbye\n");
		clear();
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nWelcome to "
            "Bear-Shell (C Port)",
            username);
        return 1;
    case 5:
        printf("\bMichael (Creator): %s", github);
        return 1;
    case 6:
        customCommands();
        return 1;
    case 7:
        debug();
        return 1;
    default:
        break;
    }
  
    return 0;
}
  
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
  
    if (strpiped[1] == NULL)
        return 0;
    else {
        return 1;
    }
}
  
void parseSpace(char* str, char** parsed)
{
    int i;
  
    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
  
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
  
int processString(char* str, char** parsed, char** parsedpipe)
{
  
    char* strpiped[2];
    int piped = 0;
  
    piped = parsePipe(str, strpiped);
  
    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
  
    } else {
  
        parseSpace(str, parsed);
    }
  
    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}
  
int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();
  
    while (1) {
        printDir();
        if (takeInput(inputString))
            continue;
        execFlag = processString(inputString,
        parsedArgs, parsedArgsPiped);
  
        if (execFlag == 1)
            execArgs(parsedArgs);
  
        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}