//Description: This lab program written in C will be implemented
//as a shell named falsh
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	//Declare functions that will be needed for this lab
	//based on analyzing the functionality and
	//understanding what needs to be implemented
	char *readCommand(char *buffer); //reads keyboard command from user
	char **commandName(const char *input); //writes back every command
	int redirection(char *input); //Allowfor redirection between programs

	return 0;
}

//Built-in Commands
//Handling whitespace
//Other Commands

