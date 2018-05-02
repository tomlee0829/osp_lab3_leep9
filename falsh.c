//Description: This lab program written in C will be implemented
//as a shell named falsh
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFERSIZE 100

int main(int argc, char *argv[])
{
	//Declare functions that will be needed for this lab
	//based on analyzing the functionality and
	//understanding what needs to be implemented
	char *readCommand(char *buffer); //reads keyboard command from user
	char **commandName(const char *input); //writes back every command
	int redirection(char *input); //Allowfor redirection between programs

	//Declare variables
	char *path;
       	char *buffer;
	char *allCommand;
	           
	if((buffer=(char *)malloc(BUFFERSIZE*(sizeof(char))))==0)
	{
		printf("error! can't malloc enough space for buffer\n");					            
		return 0;
	}
	
	while(1)
	{
		path=getcwd(NULL,0);  //Get the current directory
		printf("%s$ ", path);
		allCommand=readCommand(buffer); //Read user inputs
	}
}

//This function reads keyboard commands that
//user enters, first step of the shell
char *readCommand(char *buffer)
{
	char character;
	char *userInput;
	int inputLength = 0;

	character = getchar();
	while(character != '\n' && inputLength < BUFFERSIZE) //check input condition
	{
		buffer[inputLength] = character;
		character = getchar();
		inputLength++;
	}

	if(inputLength >= BUFFERSIZE) //Handlingh errors
	{
		fprintf(stderr, "Command is too long, please re enter!\n");
		inputLength = 0;
		return NULL;
	}

	return userInput;
}

//Built-in Commands
//Handling whitespace
//Other Commands

