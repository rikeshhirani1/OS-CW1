#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Global variable that stores the HOME Variable for the environment variable HOME
char *homeVariable;  
//Global variable that stores the Search PATHs for the environment variable PATH
char *paths;

void readFile();
void setSearchPath(char *stringLine);
void setHomeDirectory(char *stringLine);
int checkProfileFile(char *stringLine);


