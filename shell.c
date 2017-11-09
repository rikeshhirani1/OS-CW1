#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "main.h"
#include <stdbool.h>
#include <sys/wait.h>
#include <dirent.h> 


void shell(){	
	//This function is the main command prompt and is repeated, after a command has been processed
	while (true){
		char *workingDirectory = (char*)malloc(1000*sizeof(char));
		//Get current working directory and print in shell
		getcwd(workingDirectory, 1000);
		printf("%s>", workingDirectory);
		char **arguments = malloc(1000*sizeof(char*));
		arguments = readShell();
		
		if (noOfComponments != 0){
			//Interpret type of command supplied 
			int commandType = checkCommandType(arguments);
			
			if (commandType == 0){
				//If command type can be processed using exec, then execute
				int checkProgramExists = searchProgram(arguments[0], paths);

				if (checkProgramExists == 1){
					forkAndExecute(arguments);
				}
			}
		}
				
		//Clean Memory up for pointers
		free(arguments);
		arguments = NULL;
		free(workingDirectory);
		workingDirectory = NULL;
	}
}


char **splitPath(char *storedPath){
	//This function splits the stored PATH into individual search PATHs
	//E.g. /bin:/usr is split into /bin and /usr
	char **dividedPaths = malloc(1000*sizeof(char*));	
	int counter = 0;

	int length = 0;
	//Gets the length of the stored PATH
	while (storedPath[length] != '\0'){
        length++; 
	}

	char *copyPath = (char*) malloc((length+1)*sizeof(char));
	copyPath[length]= '\0';

	storedPath = &storedPath[0];

	int index = 0;
	//Creates a duplicate string of the current PATH stored
	while(storedPath[index] != '\0'){
		copyPath[index] = storedPath[index];
		index++;
	}

	char *splitString = (char*)malloc(1000*sizeof(char));
	splitString = strtok(copyPath,":"); //Splits strings where there is a : (colon)
	
	while (splitString != NULL){
		dividedPaths[counter] = malloc(100*sizeof(char));
 		dividedPaths[counter] = splitString;
	    splitString = strtok(NULL, ":");
	    
	    counter++;
  	}

  	totalPaths = counter;
  	return dividedPaths;
}

int searchProgram(char *programName, char *storedPath){
	//This function checks if the Program name supplied exists in the stored Search PATHs
	//It searches the Program file name in each directory supplied in the search Paths
	//If Program file name exists, then it returns 2, else 0 
	char **searchPaths = malloc(1000*sizeof(char*));
	searchPaths = splitPath(storedPath);

  	int i;
  	
	for (i=0; i<totalPaths; i++){
		DIR *directoryStream;
		struct dirent *entries; 
		directoryStream = opendir(searchPaths[i]);
		
		if (directoryStream){
			while((entries = readdir(directoryStream)) != NULL){

				int compareFileName = strcmp(programName,entries->d_name);
				if (compareFileName == 0){
					//printf("The Program File Exists in: %s\n", searchPaths[i]);
					return 1;
				}

			}
			closedir(directoryStream);
		}
	}
	
	printf("Error: The Program File does not Exists in the defined PATHs\n");
	return 0;

}

char **readShell(){
	//This function reads the input line supplied in the shell
	//It splits the input into individual arguements and components 
	//E.g. mkdir new is split into mkdir and new
	char input[1000];
	fgets(input, 1000, stdin);
	
	input[strlen(input)-1]= 0;

	char **arguments = malloc(1000*sizeof(char*));
	if (input[0] == 0){
		//Do nothing if empty line
		arguments[0] = "";
		noOfComponments = 0;
		return 0;
	}
	else {
		int totalStrings = 0;
		char *splitString = (char*)malloc(1000*sizeof(char));
		splitString = strtok(input," "); //Splits strings where there is a space

		if (splitString == NULL){
			arguments[0] = "";
			totalStrings = 0;
		}

		while (splitString != NULL){
			arguments[totalStrings] = malloc(100*sizeof(char));
	 		arguments[totalStrings] = splitString;	 		
		    splitString = strtok (NULL, " ");
		    totalStrings++;
	  	}

	  	arguments[totalStrings] = NULL; 
	  	noOfComponments = totalStrings;  	
	  	free(splitString);
		splitString = NULL;
	}
	
	return arguments;
}


int checkCommandType(char **arguments){
	//This function checks the first command supplied 
	//It checks if the first command is a change directory command, or new $PATH or $HOME variable definition
	//If change directory, then the directory is changed
	//If new $PATH or $HOME, then the new variables are set
	//If it is none of the above, then 0 is returned which means the command will be executed using exec to run the program
	if ((strcmp(arguments[0], "cd") == 0) && ((noOfComponments == 1) || (strcmp(arguments[1], "~") == 0))) {
	  	int check = chdir(homeVariable);

	  	if (check == -1){
	  		printf("Error: Your Home Directory specified in PROFILE or in Memory cannot be found\n");
  		}
	  	return 1;
	}
	else if ((strcmp(arguments[0], "cd") == 0) && (noOfComponments > 1)){	  		
  		int check = chdir(arguments[1]); 

	  	if (check == -1){
	  		printf("Error: The Directory you specified doesn't exist\n");
	  	}
	  	return 2;
	}
	else{
		//Set new PATH or HOME if $PATH or $HOME supplied as first command
		char pathHomeVar[5];
		strncpy(pathHomeVar, arguments[0], 6);
		if(strcmp(pathHomeVar, "$PATH=") == 0){
			memset(paths,0, 1000);
			arguments[0] = arguments[0] + 6;
			strcpy(paths, arguments[0]);
			/*free(paths);  Doesn't work on CS Server, but works on ubuntu. Above alternative solution used
		        paths = NULL;
			arguments[0] = arguments[0] + 1;
			setSearchPath(arguments[0]); 
			*/
			return 3;
		}
		else if(strcmp(pathHomeVar, "$HOME=") == 0){
			memset(homeVariable,0, 1000);
			arguments[0] = arguments[0] + 6;
			strcpy(homeVariable, arguments[0]);
			/*free(homeVariable);	Doesn't work on CS Server, but works on ubuntu. Above alternative solution used
			homeVariable = NULL;
			arguments[0] = arguments[0] + 1;
			setHomeDirectory(arguments[0]);
			*/
			return 4;
		}
	}
	return 0;
}

void forkAndExecute(char **arguments){
	//Forks a new process for the command supplied
	//And execute the process 
	pid_t childProcessID = fork();
	
	if (childProcessID == 0){
		int checkExecution = execvp(arguments[0], arguments);
		if (checkExecution == -1){
			perror("Error:");
		}
		exit(checkExecution);
	}
	else if (childProcessID>0){
		waitpid(childProcessID, NULL, 0);
	}
	else{
		perror("Error: The Fork() Failed");
		exit(EXIT_FAILURE); //Terminate the calling process
	}
}

