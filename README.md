# OS Coursework 1 - Unix Command Shell

It must be noted that in order for the shell to start, both the PATH and HOME environment variables must be defined in the profile file.
If not defined, the program running will terminate.

The files included in this Directory include:

- profile - contains the predefined PATH and HOME environment variables
- main.c - this file contains the functions of reading the file, checking the contents of the file, and interpreting the PATH and HOME variables. If the reading and interpretation of the variables is successful, then the shell starts. If the reading and interpretation of the profile file is not successful, or some contents are missing, then the program running is terminated and the shell does not run. However, an appropriate error for termination will be displayed.
- main.h - contains function declarations for main.c file and the functions are shared across other files
- shell.c - this is where the main commands/functions of the shell are executed. This includes reading text supplied in the command prompt, detecting if program name supplied exits in the defined search PATHS. If the program name does exist then the program is run with the arguements supplied in the command prompt. This file also contains functions to define a new PATH and HOME variable, and changing directory.
- shell.h - contains function declarations for shell.c file and the functions are shared across other files

To compile:
**make**

To execute:
**./shell**

To compile and execute 
**make && ./shell**

