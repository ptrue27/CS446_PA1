/*
    CS 466 Project 1 Part 2
    Parker True and Curtis Burchfield
    2/25/22
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define STR_MAX 128 // using this because <limits.h> is not allowed
#define TOKEN_MAX 32

void promptUser(bool isBatch);
void printError();
int parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits);
char getLetter(char *str, int index);
bool exitProgram(char *tokens[], int numTokens);
void launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void changeDirectories(char *tokens[], int numTokens);

void printHelp(char *tokens[], int numTokens);

int main(int argc, char *argv[])
{
    bool batchmode, isRedirect, isExit = false;

    FILE* inputFile = NULL;
    FILE* outFile = NULL;
    FILE* redirectFile = NULL;

    char cmd[STR_MAX];
    char* cmdTokens[TOKEN_MAX];
    int numTokens;

    // Determine if a batch file was inputed with command line arguements
    if(argc > 2){
        printError();
        return 1;
    } 
    else if(argc == 1){
        batchmode = false;
        inputFile = stdin;
    } 
    else{
        if(inputFile = fopen(argv[1],"r")){
            batchmode = true;
        } 
        else{
            printError();
            return 1;
        }
    }
    
    // Shell loop
    while(!isExit)
    {
        promptUser(batchmode);

        // Get cmd string from input, exit program if end of batchfile
        if(fgets(cmd, STR_MAX, inputFile) == NULL){
            // exitProgram();
        }
        if(batchmode){
            puts(cmd);
        }

        // Parse cmd string into tokens
        numTokens = parseInput(cmd, cmdTokens);
        
        // Execute command
        // char outFileName[STR_MAX] = executeCommand(cmd, &isRedirect, char* tokens[], char* outputTokens[],  &isExit); need to figure out tokens vs. outputTokens
            
    
    }

    return 0;
}

/*   MOVED runShell() FUNCTIONALITY TO main() but kept these here if you were working on it
//These two don'w work 100% right now, if you enter two arguements (help test)
            //it will break them. I am gonna get it working, just had to take a break
            exitShell = exitProgram(splitWords,splitWordsIndex);
            printHelp(splitWords,splitWordsIndex);
*/

//-------------------------------------------------------------------------
void promptUser(bool batchmode)
{
    if(!batchmode){
        // print user
        printf("%s", getenv("USER"));
        // print machine
        char hostname[STR_MAX];
        gethostname(hostname, sizeof(hostname));
        printf("@%s", hostname);
        // print cwd
        char cwd[STR_MAX];
        getcwd(cwd, sizeof(cwd));
        printf(":%s$ ", cwd);
    }
}

//-------------------------------------------------------------------------
void printError()
{
    printf("Shell Program Error Encountered");
}

//-------------------------------------------------------------------------
//This confused me, all it does is chop up the input string into different words
int parseInput(char *input, char *splitWords[]){
    int wordInd = 0;
    splitWords[0] = strtok(input, " ");
    while(splitWords[wordInd] != NULL)
    {
        // printf(" %s\n",splitWords[wordInd]);//This is it displaying each word, kind of confusing
        splitWords[++wordInd] = strtok(NULL, " ");
    }
    return wordInd;
}

//-------------------------------------------------------------------------
void printHelp(char *tokens[], int numTokens)
{
    int x=0;
    while (x <numTokens)
    {
        printf("%s",tokens[x]);
        x++;
    }   
    if (strcmp(tokens[0],"help\n")==0 || strcmp(tokens[0],"Help\n")==0 || strcmp(tokens[0],"HELP\n")==0)
    {
        printf("\nhelp detected\n");
        // if (numTokens > 1)
        // {
        //     printError();
        //     printf("\nHelp command with other error\n");
        // }
        // else
        // {
            printf("Helpful info:\n");
            printf("***Type 'exit' to exit\n");
            printf("***Type 'ls' to see all available files and folders in current directory\n");
            printf("Other\n");
        // }
    }
}

//-------------------------------------------------------------------------
bool exitProgram(char *tokens[], int numTokens)
{
    if (strcmp(tokens[0],"exit\n")==0 || strcmp(tokens[0],"Exit\n")==0 || strcmp(tokens[0],"EXIT\n")==0)
    {
        printf("\nexit detected\n");
        if (numTokens > 1)
        {
            printError();
            printf("\nExit command with other error\n");
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

// should be working but havent tested yet
void changeDirectories(char *tokens[], int numTokens){
    if(strcmp(tokens[0], "cd") == 0){
        if(numTokens != 2){
            printError();
        } 
        else{
            chdir(tokens[1]);
        }
    }
}


// not finished
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits){
    char *command = strdup(cmd);
    strcat(command, "\n");
    char outFileName[STR_MAX] = "";

    // Check if command is a redirect
    if(strchr(command, '>') != NULL){
        *isRedirect = true;
        // return redirectCommand();
    }
    *isRedirect = false;

}