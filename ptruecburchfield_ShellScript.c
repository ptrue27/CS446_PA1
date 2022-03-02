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

int main(int argc, char *argv[]){
    // Error: too many command line arguments
    if (argc > 2){
        printError();
        return 1;
    }

    bool batchmode, isRedirect, isExit = false;
    FILE *inputFile = NULL, *outFile = NULL, *redirectFile = NULL;
    char *cmd, *cmdTokens[STR_MAX], *outFileName;
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
    
    // Shell loop
    while(!isExit)
    {
        // Print command prompt if in user mode        
        promptUser(batchmode);

        // Get cmd string from input, exit program if end of batchfile
        if(fgets(cmd, STR_MAX, inputFile) == NULL){
            isExit = true;
            break;
        }
        if(cmd[strlen(cmd)-1] == '\n'){
            cmd[strlen(cmd)-1] = '\0';
        }

        // Display command if read from batchfile
        if(batchmode){
            puts(cmd);
        }

        // Parse cmd string into tokens
        // Issue: this adds null characters between words in cmd so when passing to executeCommand() cmd only gives first word
        // As of right now it still works without this but according to the directions for main() its supposed to go here
        //numTokens = parseInput(cmd, cmdTokens);
        
        // Execute command stored in cmd string
        outFileName = executeCommand(cmd,&isRedirect,cmdTokens,cmdTokens,&isExit);

        // Handle redirect if applicable
        if(isRedirect){
            // not implemented yet
        }
    }

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
    //for(int i=0;i<wordInd;i++){printf("%s|",splitWords[i]);}
    return wordInd;
}

// Prints help screen upon 'HELP' command
void printHelp(char *tokens[], int numTokens){
    if (strcmp(lowerString(tokens[0]),"help") == 0){
        if (numTokens > 1){
            printError();
        }
        else{
            printf("\n\nhelp -prints this screen so you can see available shell commands.");
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
        // return redirectCommand();
    }

    // Parse command, return if no tokens created
    *isRedirect = false;
    int numTokens = parseInput(cmdCopy, outputTokens);
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
    //launchProcesses(tokens, numTokens, isRedirect);

    free(cmdCopy);
    return outFileName;
}

// needs redoing
/*
void launchProcesses(char *tokens[], int numTokens, bool isRedirect){
    if (!batch)
    {
        if (numTokens == 1)
        {
            tokens[0][strlen(tokens[0])-1] = '\0';
        }
        else
        {
            for (int i=1;i<numTokens;i++)
            {
                tokens[i][strlen(tokens[i])-1] = '\0';
            }
        }
    }
    else
    {
        if (numTokens==1)
        {
            // tokens[0][strlen(tokens[0])-1] = '\0';
        }
        else
        {
            for (int i=1;i<numTokens;i++)
            {
                tokens[i][strlen(tokens[i])-1] = '\0';
            }  
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
*/

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