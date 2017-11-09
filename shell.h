#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Global variable that stores the number of compinents/arguements supplied at command prompt in shell
//E.g. cd name is 2 components
int noOfComponments;
//Global variable that stores the number of Search PATHs in the environment variable PATH
int totalPaths;

void shell();
char **readShell();
void forkAndExecute(char **arguments);
int checkCommandType(char **arguments);
int searchProgram(char *programName, char *storedPath);
char **splitPath(char *storedPath);