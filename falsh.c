//Description: This lab program written in C will be implemented
//as a shell named falsh
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFERSIZE 100
int main(void) 
{
    //Declare functions that will be needed for this lab
    //based on analyzing the functionality and
    //understanding what needs to be implemented
    char *readCommand(char *buffer); //reads keyboard command from user
    char **commandName(const char *input); //writes back every command
    int commandNumber(const char *input); //counts the command number
    int pipeline(char *input); //pipeline
    int pipeNumber(const char *input); //counts pipe number
    int redirection(char *input); //Allow for redirection between programs
	

    //Declare variables
    char *directory;
    char *buffer;
    char *allCommand;
    char **everyCommand;
    int i;
    int k;
    int pipe;
    int number;

    //The malloc() function allocates size bytes of memory and
    //returns a pointer to the allocated memory. It can fail if the OS
    //runs out of memory.
    if((buffer =(char *)malloc(BUFFERSIZE *(sizeof(char)))) == 0)
    {
	    printf("error, malloc failed! No enough space for buffer\n");
	    return 0;
    }
    
    //Ask user to invoke falsh and the program starts
    //until the user types the exit command
    char str[10];
    printf("Welcome to the Falcon Shell! To start please enter falsh:\n"); //Welcome message
    scanf("%s",str); //Takes user input for invoking
    //The strcmp() function compares the user input
    //string and if it equals, start Falsh
    if(strcmp("falsh", str) == 0)
    {
        while(1)
        {
            directory=getcwd(NULL,0); //Get the current directory
            printf("%s $", directory);
            
            allCommand=readCommand(buffer); //Read user inputs
            
            //Check commands and see if there's pipeline
            if(allCommand==NULL)
            {
                continue;
            }
            
            number=commandNumber(allCommand);
            if (number<0)
            {
                continue;
            }
            
            everyCommand=commandName(allCommand);
            i=0;
            while (i<number)
            {
                if(strlen(everyCommand[i])!=0)
                {
                    k=pipeNumber(everyCommand[i]);
                    if(k!=0)
                    {
                        pipeline(everyCommand[i]);
                    }
                    else
                    {
                        redirection(everyCommand[i]);
                    }
                }
                i++;
            }
		
        //free() function frees allocations that were created
        //via the preceding allocation functions.
	for(i=0;i<number;i++)
        {
		free(everyCommand[i]);
        }
		free(everyCommand);
		free(allCommand);
		free(directory);
        }
    }
    //exit if user did not invoke falsh
    else
    {
        exit (1);
    }
}

//This function reads keyboard commands that
//user enters, first step of the shell
char *readCommand(char *buffer)
{
    //Delcarations
    char character;
    char *userInput;
    int inputLength = 0;
    
    //getchar() reads the next character from
    //stdin and returns it. The while loop
    //makes it read to the end of file until
    //user hits CRTL+D to indicate EOF
    character = getchar();
    while(character != '\n' && inputLength < BUFFERSIZE) //check input condition
    {
        buffer[inputLength] = character;
        character = getchar();
        inputLength++;
    }
    
    if(inputLength >= BUFFERSIZE) //Handling errors
    {
        //frpintf() writes output to the given output stream(stderr)
        //display error message
        fprintf(stderr, "Command is too long, please re enter!\n");
        inputLength = 0; //reset input length
        return NULL;
    }
    else
    {
        buffer[inputLength] = '\0';
    }
    
    //The malloc() function allocates size bytes of memory and
    //returns a pointer to the allocated memory. It can fail if the OS
    //runs out of memory.
    if((userInput=(char *)malloc(sizeof(char)*(inputLength+1)))==0)
    {
        //frpintf() writes output to the given output stream(stderr)
        //display erro message
        fprintf(stderr,"error, malloc failed! No enough space for inpput\n");
        return NULL;
    }
    
    //strcpy() function copy the string user's input
    //to destination buffer
    strcpy(userInput,buffer);
    
    return userInput;
}

//This function writes back every command
//that user enters and runs the command
//checks condition for only space or ;
char **commandName(const char *input)
{
    int commandNumber(const char *input); //count command numbers

    //Delcare variables
    int i;
    int j;
    int k;
    int maxLength;
	char **command;
	
    //strlen() function computes the length of
    //input and returns the number of
    //characters that precede the terminating
    //NUL character
    maxLength = strlen(input);
    k = commandNumber(input); //set k to count the command numbers

    //The malloc() function allocates size bytes of memory and
    //returns a pointer to the allocated memory
	command = (char **)malloc(k*sizeof(char *));
	for(i = 0; i < k; i++)
	{
		command[i] = (char *)malloc((maxLength+1)*sizeof(char));
		command[i][0] = '\0';
	}
    
    //Reset and split the command by ;
    k = 0;
    j = 0;
    for (i = 0;i <= maxLength; i++)
    {
	    if (input[i] != ';')
	    {
		    command[k][j] = input[i];
		    j++;
	    }
	    else
	    {
		    command[k][j] = '\0';
		    k++;
		    j=0;
	    }
    }
    return command;
}

//This function counts the command number
//and use ; to seperate
int commandNumber(const char *input)
{
    //Declare variables
    int sum = 0;
    int i = 0;
    int length;
    
    //strlen() function computes the length of
    //input and returns the number of
    //characters that precede the terminating
    //NUL character
	length = strlen(input);
    
    //Check condition and whitespace
	while(i < length && (input[i] == ' '|| input[i] == '	'))
    {
		i++;
    }
	if(input[i] == ';')
	{
		fprintf(stderr," syntax error near unexpected token: ;\n");
		return -1;
	}
    //command contains only space or tab
	if (i == length)
    {
		return -1;
    }
	for (i=0;i<length;i++)
    {
        if(input[i]==';')
        {
            while(i<strlen(input)&&(input[i+1]==' '||input[i+1]=='	'))
            {
                i++;
            }
        }
		if(input[i+1]==';')
		{
			fprintf(stderr," syntax error near unexpected token: ;;\n");
			return -1;
		}
		else
	       	{
			sum++;
	       	}
    }

	sum=sum+1;
	return sum;
}

