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

void promptUser(bool isBatch);
void printError();
int parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits);
char getLetter(char *str, int index);
bool exitProgram(char *tokens[], int numTokens);
void launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void changeDirectories(char *tokens[], int numTokens);

int main(int argc, char *argv[]){
    bool batchmode = 0;
    promptUser(batchmode);

    return 0;
}

void promptUser(bool batchmode){
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