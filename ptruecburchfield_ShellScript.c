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
#include <sys/wait.h>

#define STR_MAX 128 // using this because <limits.h> is not allowed
#define TOKEN_MAX 32

void promptUser(bool isBatch);
void printError();
int parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits,int numTokens);
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
            printf("\nDOes htis execute\n");
        }
        if(batchmode){
            puts(cmd);
        }

        // Parse cmd string into tokens
        numTokens = parseInput(cmd, cmdTokens);
        
        printHelp(cmdTokens,numTokens);
        isExit=exitProgram(cmdTokens,numTokens);
        executeCommand(cmd,&isRedirect,cmdTokens,cmdTokens,&isExit,numTokens);
    }

    return 0;
}
            // exitShell = exitProgram(splitWords,splitWordsIndex);
            // printHelp(splitWords,splitWordsIndex);

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
    printf("\nShell Program Error Encountered\n");
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
    // for (int i=0;i<wordInd;i++)
    // {
    //     printf("%s",splitWords[i]);
    // }
    return wordInd;
}

//-------------------------------------------------------------------------
void printHelp(char *tokens[], int numTokens)
{
    if (strcmp(tokens[0],"help\n")==0 || strcmp(tokens[0],"Help\n")==0 || strcmp(tokens[0],"HELP\n")==0 || strcmp(tokens[0],"HELP")==0 || strcmp(tokens[0],"Help")==0 || strcmp(tokens[0],"help")==0)
    {
        if (numTokens > 1)
        {
            printError();
            printf("\nHelp command with other arguement\n\n");
        }
        else
        {
            printf("\n\nhelp -prints this screen so you can see available shell commands.");
            printf("\ncd -changes directories to specified path;if not given, defaults to home.");
            printf("\nexit -closes the shell");
            printf("\n[input] > [output] -pipes input file into output file");
            printf("\nls -see all available files in current directory");
        }
    }
}

//-------------------------------------------------------------------------
bool exitProgram(char *tokens[], int numTokens)
{
    if (strcmp(tokens[0],"exit\n")==0 || strcmp(tokens[0],"Exit\n")==0 || strcmp(tokens[0],"EXIT\n")==0 || strcmp(tokens[0],"exit")==0 || strcmp(tokens[0],"Exit")==0 || strcmp(tokens[0],"EXIT")==0)
    {
        if (numTokens > 1)
        {
            printError();
            printf("\nExit command with other arguement\n\n");
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

// Working, test on TestDir
void changeDirectories(char *tokens[], int numTokens){
    tokens[1][strlen(tokens[1])-1] = '\0';
        if(numTokens != 2){
            printError();
        } 
        else{
            chdir(tokens[1]);
        }
}


// not finished
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits,int numTokens){
    char *command = strdup(cmd);
    char outFileName[STR_MAX] = "";
    strcat(command, "\n");
    // Check if command is a redirect
    if(strchr(command, '>') != NULL){
        *isRedirect = true;
        // return redirectCommand();
    }
    else
    {
        *isRedirect = false;
    }

    if (strcmp(tokens[0],"cd\n")==0 || strcmp(tokens[0],"cd")==0)
    {
        changeDirectories(tokens,numTokens);
    }
    else
    {
        launchProcesses(tokens,numTokens,isRedirect);
    }
    return "outputFile.txt";
}


void launchProcesses(char *tokens[], int numTokens, bool isRedirect)
{
    if (numTokens == 1)
    {
        tokens[0][strlen(tokens[0])-1] = '\0';
    }
    else
    {
        for (int i=1;i<numTokens;i++)
        {
            tokens[i][strlen(tokens[0])] = '\0';
        }
    }
    char* args[3];
    args[0]="ls";
    args[1]="-l";
    args[2]=NULL;
    
    // tokens[0][strlen(tokens[0])-1] = '\0';
    pid_t childProc;
    int childStatus;
    pid_t childReturn;
    if ((childProc = fork()) == 0)
    {
        execvp(tokens[0],tokens);
        exit (1);
    }
    else
    {
        if (childProc == (pid_t)(-1))
        {
            exit(1);
        }
        else
        {
            childReturn = wait(&childStatus);
        }
    }
}