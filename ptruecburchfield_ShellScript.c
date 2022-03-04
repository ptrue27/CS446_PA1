/*
    CS 446 Project 1 Part 2
    Program Name: ptruecburchfield_ShellScript
    Authors: Parker True and Curtis Burchfield
    Purpose: Create a basic shell capable of running select commands
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#define STR_MAX 128 // using this because <limits.h> is not allowed
#define TOKEN_MAX 16

void promptUser(bool isBatch);
void printError();
int parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExit);
void printHelp(char *tokens[], int numTokens);
bool exitProgram(char *tokens[], int numTokens);
void launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void changeDirectories(char *tokens[], int numTokens);
char *lowerString(char *str);

void fixString(char * tokens[], int numTokens);

int main(int argc, char *argv[]){
    int bCommands =0;
    char * result;
    // Error: too many command line arguments
    if (argc > 2){
        printError();
        return 1;
    }

    bool batchmode, isRedirect, isExit = false;
    FILE *inputFile = NULL, *outFile = NULL, *redirectFile = NULL;
    char *cmd, *cmdTokens[STR_MAX], *outputTokens[STR_MAX], *outFileName;
    int numTokens;

    // Initialize input stream
    if (argc == 1){
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
    
    
    // if (batchmode)
    // {
    //     printf("\nBatch\n");
    //     size_t length;
    //     ssize_t reading;
    //     //reads in batch file line by line, in the loop need to add executing the command
    //     while ((reading = getline(&cmd,&length, inputFile)) != -1)
    //     {
    //         // outFileName = executeCommand(cmd,&isRedirect,cmdTokens,cmdTokens,&isExit);
    //         // printf("%s\n",cmd);
    //         printf("%s\n",cmd);

    //     }
        


    // }
    // else
    // {
        // Shell loop
        while(!isExit)
        {
            // Print command prompt if in user mode        
            promptUser(batchmode);
            // Get cmd string from input, exit program if end of batchfile
            if(fgets(cmd, STR_MAX, inputFile) == NULL)
            {
                isExit=true;
            }
            // result = fgets(cmd, STR_MAX, inputFile);
            if(cmd[strlen(cmd)-1] == '\n'){
                cmd[strlen(cmd)-1] = '\0';
            }
            if(batchmode){
                printf(">");
                puts(cmd);
                bCommands++;
            }

            // Parse cmd string into tokens
            // Issue: this adds null characters between words in cmd so when passing to executeCommand() cmd only gives first word
            // As of right now it still works without this but according to the directions for main() its supposed to go here
            //numTokens = parseInput(cmd, cmdTokens);
            
            // Execute command stored in cmd string
            outFileName = executeCommand(cmd,&isRedirect,cmdTokens,cmdTokens,&isExit);
        }
    // }
    printf("\nNumber of batch commands: %d\n",bCommands);
    // I'm not quite sure what this is doing but I'm guessing something to do with launchProcesses() which I have not looked at
    //kill(getpid(),SIGUSR1);

    return 0;
}

// Prints command prompt to terminal
void promptUser(bool batchmode){
    if(!batchmode){
        // Print user
        printf("%s", getenv("USER"));
        // Print machine
        char hostname[STR_MAX];
        gethostname(hostname, sizeof(hostname));
        printf("@%s", hostname);
        // Print cwd
        char cwd[STR_MAX];
        getcwd(cwd, sizeof(cwd));
        printf(":%s$ ", cwd);
    }
}

// Prints error message to terminal
void printError(){
    printf("\nShell Program Error Encountered\n\n");
}

// Splits input string into tokens
int parseInput(char *input, char *splitWords[]){
    int wordInd = 0;
    splitWords[0] = strtok(input, " ");
    while (splitWords[wordInd] != NULL){
        splitWords[++wordInd] = strtok(NULL, " ");
    }
    return wordInd;
}

// Prints help screen upon 'HELP' command
void printHelp(char *tokens[], int numTokens){
    if (strcmp(lowerString(tokens[0]),"help") == 0){
        if (numTokens > 1){
            printError();
        }
        else{
            printf("\nhelp -prints this screen so you can see available shell commands.");
            printf("\ncd -changes directories to specified path;if not given, defaults to home.");
            printf("\nexit -closes the shell");
            printf("\n[input] > [output] -pipes input file into output file");
            printf("\nls -see all available files in current directory\n\n");
        }
    }
}

// Returns true upon 'EXIT' command
bool exitProgram(char *tokens[STR_MAX], int numTokens){
    if (strcmp(lowerString(tokens[0]),"exit") == 0){
        if (numTokens > 1){
            printError();
            return false;
        }
        return true;
    }
    return false;
}

// Changes current working directory upon 'CD' command
void changeDirectories(char *tokens[STR_MAX], int numTokens){
    if (strcmp(lowerString(tokens[0]), "cd") == 0){
        if (numTokens != 2){
            printError();
        }
        else{
            tokens[1][strlen(tokens[1])-1] = '\0';
            if (chdir(tokens[1]) == -1){
                printError();
            }
        }
    }
}


// Handles execution of one command, returns output file name
char *executeCommand(char *cmd, bool *isRedirect, char *tokens[STR_MAX], char *outputTokens[STR_MAX], bool *isExit){
    char *cmdCopy = strdup(cmd), *outFileName = "";
    strcat(cmdCopy, "\n");
    
    // Check for then execute redirect command
    if (strchr(cmdCopy, '>') != NULL){
        *isRedirect = true;
        return redirectCommand(">","uh",isRedirect,tokens,outputTokens);
    }
    int numTokens = parseInput(cmdCopy, outputTokens);

    // Parse command, return if no tokens created
    *isRedirect = false;

    if (numTokens == 0){
        return outFileName;
    }

    // Exit if 'EXIT' command is given
    *isExit = exitProgram(outputTokens, numTokens);
    if(*isExit){
        return outFileName;
    }

    // Change directories if 'CD' command is given
    changeDirectories(outputTokens, numTokens);

    // Print help screen if 'HELP' command is given
    printHelp(outputTokens, numTokens);

    // Executes execvp command if given
    fixString(tokens,numTokens);
    launchProcesses(tokens, numTokens, isRedirect);

    free(cmdCopy);
    return outFileName;
}

// needs redoing

void launchProcesses(char *tokens[], int numTokens, bool isRedirect)
{
    pid_t childProc;
    int childStatus;
    pid_t childReturn;
    if ((childProc = fork()) == 0)
    {
        execvp(tokens[0],tokens);
        // printError();
        exit (1);
    }
    else
    {
        if (childProc == (pid_t)(-1))
        {
            // printError();
            exit(1);
        }
        else
        {
            childReturn = wait(&childStatus);
        }
    }
}

// Lowers case of all letters and removes newline if it exists
char *lowerString(char *str){
    int i;
    for(i = 0; str[i] != '\0'; i++){
      if(str[i] >= 65 && str[i] <= 90){
         str[i] = str[i] + 32;
      }
   }
   if(str[i-1] == '\n'){
       str[i-1] = '\0';
   }
   return str;
}

void fixString(char * tokens[], int numTokens)
{
    for (int j=0;j<numTokens;j++)
    {
        for (int i=0;i<100;i++)
        {
            if (tokens[j][i]=='\n')
            {
                tokens[j][i]= '\0';
                break;
            }
        }
    }
}

char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[])
{
    
    //Super janky, but it gets both the file names from the input
    int i = 0,f = 0, j=0;
    int readWords=0;
    char inputFileName[STR_MAX];
    char outputFileName[STR_MAX];
    i=0;
    f=0;
    j=0;
    while(readWords!=2)
    {
        for(j=0; tokens[0][j] != '\0'; ++j)
        {
            if (readWords==0)
            {
                inputFileName[i]=tokens[0][j];
                i++;
            }
            if (readWords==2)
            {
                outputFileName[f]=tokens[0][j];
                f++;
            }
            if (tokens[0][j]==' ')
            {
                readWords++;
            }
            
        }
    }
    inputFileName[i-1]='\0';
    outputFileName[f-1]='\0';

    //File copying
    char CHAR;
    FILE* inputFile, *outputFile;
    inputFile = fopen(inputFileName,"r");
    if (inputFile==NULL)
    {
        printError();
        return outputFileName;
    }
    outputFile = fopen(outputFileName,"w");
    while ((CHAR = fgetc(inputFile)) != EOF)
    {
        fputc(CHAR, outputFile);
    }
    fclose (inputFile);
    fclose (outputFile);
    printf("\nSuccessfully Copied\nOutput File Name: %s\n",outputFileName);
    return outputFileName;

}