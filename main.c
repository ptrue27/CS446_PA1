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
#include <limits.h>

void promptUser(bool isBatch, FILE* batchFile);
void printError();
int parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits);
char getLetter(char *str, int index);
bool exitProgram(char *tokens[], int numTokens);
void launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void changeDirectories(char *tokens[], int numTokens);

void batchRead(FILE* file);
void printHelp(char *tokens[], int numTokens);
void runShell(bool batchmode, FILE* batchFile);

int main(int argc, char *argv[])
{
    bool batchmode;
    FILE* baFile;
    if (argc > 1)
    {
        //checks to see if command line arguement was used, and if so is it a file that exists
        if (baFile = fopen(argv[1],"r"))
        {
            batchmode=1;
            printf("---Batch Found and initiating---\n");
        }
        else
        {
            batchmode=0;
            printf("-----EnteringNormal mode, no batch file found-----\n");
        }
    }
    //Determine if a batch file was inputed with command line arguements

    runShell(batchmode,baFile);
    //use kill to exit

    return 0;
}

//-------------------------------------------------------------------------
void runShell(bool batchmode, FILE* batchFile)
{
    int splitWordsIndex;
    char* splitWords[100];
    char input[100];
    bool exitShell = false;

    while (exitShell==false)
    {
        promptUser(batchmode, batchFile);
        if (batchmode==true)
        {
            //If it finds a batch file it will read it, then go to normal mode
            batchmode=false;
            printf("---Batch file finished, proceding to normal mode:---\n");
        }
        else
        {
        for (int i = 0; input[i-1]!='\n';i++)
        {
            //This will read in all the arguements, whether it be just 'ls' or something like 'ls -a'
            input[i]=getchar();
        }
            splitWordsIndex = parseInput(input,splitWords);
        
            //These two don'w work 100% right now, if you enter two arguements (help test)
            //it will break them. I am gonna get it working, just had to take a break
            exitShell = exitProgram(splitWords,splitWordsIndex);
            printHelp(splitWords,splitWordsIndex);
        }
    }

}

//-------------------------------------------------------------------------
void promptUser(bool batchmode, FILE* batchFile)
{
    //Changed parameters so it can execute the batch mode in this function
    char str [100];

    int splitWordsIndex;

    if(!batchmode){
        // print user
        printf("%s", getenv("USER"));
        // print machine
        char hostname[HOST_NAME_MAX];
        gethostname(hostname, sizeof(hostname));
        printf("@%s", hostname);
        // print cwd
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf(":%s$ ", cwd);
    }

    //if it is a batch file
    else
    {
        batchRead(batchFile);
    }
}

void batchRead(FILE* file)
{
    char* cmd = NULL;
    size_t length;
    ssize_t reading;

    //reads in batch file line by line, in the loop need to add executing the command
    while ((reading = getline(&cmd,&length, file)) != -1)
    {
        printf("%s\n",cmd);
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