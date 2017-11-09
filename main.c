#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "shell.h"
#include <stdbool.h>

int main(){
	//The main function first reads the file 'profile'
	//It checks the contents of file to see if a PATH and HOME variable is defined
	//If it is defined, then the HOME variable and Search PATHs are set
	//If the HOME, PATH or both enviroment variables are not assigned, then an error is outputted. The shell does not begin and the program is terminated
	//If the assignments to these variables is blank/empty, then an error is outputted, The shell does not begin and the program is terminated
	//If the contents in the file are defined correctly, then shell begins

	FILE *file;

	file = fopen("profile", "r"); 
	if (file == NULL){
		perror("Error: Could not open File!");
		return(-1);
	}

	char stringLine[1000];
	int counter = 0;
	int countPathVar = 0;
	int countHomeVar = 0;

	while (fgets(stringLine, 1000, file) != NULL){
		int checker = checkProfileFile(stringLine);

		if (checker == 1){
			setHomeDirectory(stringLine);
			countHomeVar++;
		}
		else if (checker == 2){
			setSearchPath(stringLine);
			countPathVar++;
		}
		counter++;
	}
	

	if (countHomeVar == 0 || countPathVar==0){
		//The HOME or PATH variable has not been defined in the profile file
		if (countHomeVar == 0 && countPathVar == 0){
			printf("Error: The variable HOME and PATH has not been defined in file PROFILE\n");
		}
		else if (countPathVar == 0){
			printf("Error: The variable PATH has not been defined in file PROFILE\n");

		}
		else if (countHomeVar == 0){
			printf("Error: The variable HOME has not been defined in file PROFILE\n");
		}
	}
	else if (*homeVariable == '\0' || *paths == '\0'){
		//The HOME or PATH variable assigment is empty and has not been defined in the profile file
		if (*homeVariable == '\0' && *paths == '\0'){
			printf("Error: The variable assignment of HOME and PATH has no value assigned in file PROFILE (NULL Value)\n");
		}
		else if (*paths == '\0'){
			printf("Error: The variable assignment of PATH has no value assigned in file PROFILE (NULL Value)\n");

		}
		else if (*homeVariable == '\0'){
			printf("Error: The variable assignment of HOME has no value assigned in file PROFILE (NULL Value)\n");
		}
	}
	else{
		//Start Shell
		shell();
	}
	
	if (file != NULL){
		//Close file if it is not empty
		//An empty file gives segmentaton fault
		fclose(file);
	}	
	
	return 0;
}


void setHomeDirectory(char *stringLine){
	//This function sets the HOME variable
	//It assigns all the text after HOME= to a global variable called homeVariable

	stringLine = stringLine + 5; //Skips to text after HOME=
	homeVariable = (char*)malloc(1000*sizeof(char)); 
	
	int i=0;
	while (stringLine[i] != '\n'){
		homeVariable[i] = stringLine[i];
		i++;
	}
}

void setSearchPath(char *stringLine){
	//This function sets the Search PATHS
	//It assigns all the text after PATH= to a global variable called paths

	stringLine = stringLine + 5; //Skips to text after PATH=	
	paths = (char*)malloc(1000*sizeof(char)); 
	int i=0;
	
	while (stringLine[i] != '\n'){
		paths[i] = stringLine[i];
		i++;
	}
}


int checkProfileFile(char *stringLine){
	//This function checks the contents of the file 
	//It checks if the line read from the file is either a PATH variable or HOME variable or neither
	//It returns 1 if line read is a HOME Variable
	//It returns 2 if line read is a PATH Variable
	//It returns 0 if neither of the above

	int lengthOfVariable = 5; //Length of text "PATH=" and "HOME="
	char pathVar[] = "PATH=";
	char homeVar[] = "HOME=";
	int variableType = 0; //Variable Type HOME or PATH, 1 for HOME and 2 for PATH
	int counter = 0;
	int i;

	//String Comparison
	for (i = 0; i < lengthOfVariable; i++){
		if (stringLine[0] == 'P'){
			if (stringLine[i] == pathVar[counter]){			
				counter++;
				variableType = 2;
			}
			else{
				return 0;
			}
		}
		else if (stringLine[0] == 'H'){
			if (stringLine[i] == homeVar[counter]){
				counter++;
				variableType = 1;
			}
			else{
				return 0;
			}
		}
		else{
			return 0;
		}
	} 
	return variableType;
}
