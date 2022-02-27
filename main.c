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

bool exitShell = false;

void promptUser(bool isBatch, FILE* batchFile);
void printError();
int parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits);
char getLetter(char *str, int index);
bool exitProgram(char *tokens[], int numTokens);
void launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void changeDirectories(char *tokens[], int numTokens);
void printHelp();

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
    while (exitShell==false)
    {
        promptUser(batchmode, baFile);
        if (batchmode==true)
        {
            //If it finds a batch file it will read it, then go to normal mode
            batchmode=false;
            printf("---Batch file finished, proceding to normal mode:---\n");
        }
    }
    return 0;
}

void promptUser(bool batchmode, FILE* batchFile)
{
    //Changed parameters so it can execute the batch mode in this function
    char str [100];
    char* splitWords[100];
    char test[]="exit\n";
    char test2[]="Exit\n";
    char test3[]="EXIT\n";
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
        
        //This will read in all the arguements, whether it be just 'ls' or something like 'ls -a'
        for (int i = 0; str[i-1]!='\n';i++)
        {
            str[i]=getchar();
        }
        
        //The two if statements maybe should go into a switch, i am unsure as to how we 
        //will decide what command the user types, and such. I think a switch would work,
        //however I am not sure if there is a better way to do it becuause we might end
        //up with a ton of switch statements. I don't understand the instructions when it says
        //"The shell should accept execvp() executable commands, such as ls (and ls -la) and clear.""
        //I go to that website and don't understand any of it at the moment.

        //this checks to see if 'exit' is typed to tell when they want to exit the program. Probably gonna need
        //some changing because I think we need to use kill().
        if (strcmp(str,test)==0 || strcmp(str,test2)==0 || strcmp(str,test3)==0)
        {
            exitShell=true;

        }
        if (strcmp(str,"help\n")==0 || strcmp(str,"Help\n")==0 || strcmp(str,"HELP\n")==0)
        {
            printHelp();
        }


    }

    //if it is a batch file
    else
    {
        char* cmd = NULL;
        size_t length;
        ssize_t reading;

        //reads in batch file line by line, in the loop need to add executing the command
        while ((reading = getline(&cmd,&length, batchFile)) != -1)
        {
            printf("%s\n",cmd);
        }
        
    }
}

void printError(){
    printf("Shell Program Error Encountered");
}

int parseInput(char *input, char *splitWords[]){
    int wordInd = 0;
    splitWords[0] = strtok(input, " ");
    while(splitWords[wordInd] != NULL){
        splitWords[++wordInd] = strtok(NULL, " ");
    }

    return wordInd;
}

//start to help command, nothing really
void printHelp()
{
    printf("Helpful info:\n");
    printf("***Type 'exit' to exit\n");
    printf("***Type 'ls' to see all available files and folders in current directory\n");
    printf("Other\n");
}