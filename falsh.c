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
	////returns a pointer to the allocated memory
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

//This function creates the pipe
//and allows the redirection for commands
//and any non-built-in command should be executed
//if it is in one of the directories
//specified in the userâ€™s path.
int pipeline(char * input)
{
    //Declare functions
    int redirection(char *input); //Allow for redirection between programs
    int isBack(char *command); //analize if background process
    int pipeNumber(const char *input); //count the pipe number
    
    //Declare variables
    int status;
    int i;
    int j;
    int k;
    int **fd;
    int back=0;
    int length;
    char **command;
    int *child;
    
    //Assign variables to functions
    back = isBack(input);
    length = strlen(input);
    k=pipeNumber(input);
    command=(char **)malloc((k+1)*sizeof(char *));
    
    //The malloc() function allocates size bytes of memory and
    //returns a pointer to the allocated memory. It can fail if the OS
    //runs out of memory.
    for(i = 0; i< k+1; i++)
    {
        command[i] = (char *)malloc((length+1)*sizeof(char));
    }
    
    child = (int *)malloc((k+1)*sizeof(char *));
    fd = (int **)malloc(k*sizeof(int *));
    for(i = 0;i< k; i++)
    {
        fd[i]=(int *)malloc(2*sizeof(int));
    }
    
    //Reset and split the command by |
    k = 0;
    j = 0;
    for (i = 0;i <= length; i++)
    {
        if(input[i] !='|')
        {
            command[k][j] = input[i];
            j++;
        }
        else
        {
            command[k][j] = '\0';
            k++;
            j = 0;
        }
    }
    
    //create the pipe
    for(i = 0;i < k;i++)
        if(pipe(fd[i]) == -1)
        {
            fprintf(stderr, "Open pipe error!\n");
            return 0;
        }
    
    //create the first child
    i=0;
    //fork() function creates a new process child
    if((child[i]=fork()) == 0)
    {
        close(fd[i][0]);
        //check condition
        if(fd[i][1] != STDOUT_FILENO)
        {
            //dup2() specified the new descriptor
            //and handle error here
            if(dup2(fd[i][1], STDOUT_FILENO) == -1)
            {
                fprintf(stderr, "Redirect Standard Out error !\n");
                return -1;
            }
            
            //close() function call deletes
            //a descriptor from the process
            close(fd[i][1]);
        }
        //Redirect commands if condition is satisfied
        redirection(command[i]);
        exit(1);    //child1 exit
    }
    else
    {
        //waitpid() call let the program
        //wait for child process
        waitpid(child[i], &status, 0);
        close(fd[i][1]);
    }
    
    i++;
    
    while(i < k)
    {
        if ((child[i] = fork()) == 0)
        {
            if(fd[i][0] != STDIN_FILENO)
            {
                
                //dup2() specified the new descriptor
                //and handle error here
                if(dup2(fd[i][1], STDOUT_FILENO) == -1)
                {
                    fprintf(stderr, "Redirect Standard Out error !\n");
                    return -1;
                }
                
                //close() function call deletes
                //a descriptor from the process
                close(fd[i][1]);
            }
             //Redirect commands if condition is satisfied
            redirection(command[i]);
            exit(1);
        }
        else
        {
            //waitpid() call let the program
            //wait for child process
            waitpid(child[i], &status, 0);
            close(fd[i][1]);
            i++;
        }
    }
    //create the last child
    if((child[i] = fork()) == 0)
    {
        redirection(command[i]);
        exit(1);
    }
    else if(back == 0)
    {
        waitpid(child[i], NULL, 0);
        close(fd[i-1][1]);
    }
    
    //free() function frees allocations that were created
    //via the preceding allocation functions.
    for(i = 0;i < k; i++)
    {
        free(fd[i]);
    }
    free(fd);
    
    for(i=0;i<k+1;i++)
    {
        free(command[i]);
    }
    free(command);
    free(child);
    
    return 1;
    
}

//This function counts the pipe number
int pipeNumber(const char *input)
{
    //Declare variables
    int sum = 0;
    int i;
    
    //loop through and check for |
    //strlen() function computes the length of
    //input and returns the number of
    //characters that precede the terminating
    //NUL character
    for (i = 0;i < strlen(input);i++)
    {
        if(input[i]=='|')
        {
            sum++; //increment
        }
    }
    
	return sum;
}

//This function is for cd command
void cdCommand(char *argv[])
{
    //Check condition
    if(argv[1] != NULL)
    {
        //Handling cd errors
        //and prints out appropriate error message
        if(chdir(argv[1]) < 0)
            switch(errno)
        {
            case ENOENT:
                fprintf(stderr,"DIRECTORY NOT FOUND\n");
                break;
            case ENOTDIR:
                fprintf(stderr,"NOT A DIRECTORY NAME\n");
                break;
            case EACCES:
                fprintf(stderr,"YOU DO NOT HAVE RIGHT TO ACCESS\n");
                break;
            default:
                fprintf(stderr,"SOME ERROR HAPPENED IN CHDIR\n");
        }
    }
    //If there's no arugument provided
    //change to the user's home directory
    else
    {
        //getenv() function obtains the current value of enviroment
        //here is "HOME" and the chdir() function shall cause the directory
        //named by the pathname pointed to by the path argument to
        //become the current working directory
        chdir(getenv("HOME"));
    }
}

//This function allows redirection
int redirection(char *input)
{
    //declare functions
    char **analize(const char *input);
    char *isFileExist(const char *command);
    void cdCommand(char *argv[]);
    int number(const char *input);
    int isBack(char *command);
    int pipeNumber(const char *input);

    //declare variables
    char *commandPath;
    char *realCommand;
    char *filenameOut;
    char **analizedCommand;
    int length;
    int status;
    int i;
    int j = 0;
    int k;
    int back = 0;
    int fdOut;
    int flagIn = 0;
    int flagOut = 0;
    pid_t pid;
	
    //Assign variables to functions
    back=isBack(input);
    length=strlen(input);
    filenameOut=(char *)malloc((length+1)*(sizeof(char)));
    realCommand=(char *)malloc((length+1)*(sizeof(char)));

    //Reads string command and store in realCommand
    for(i = 0; i < length; i++)
    {
	    if (input[i]!='>')
	    {
		realCommand[i] = input[i];
    	    }
	    else
	    {
		    if (input[i]=='>')
		    {
			flagOut = 1;
		    }		
		    break;
    	    }
    }
 
    realCommand[i] = '\0';
    i++;
    //Checking conditions below for redirection
    if(flagOut == 1 && input[i] == '>')
    {
	    flagOut = 2;
	    i++;
    }
	
    //Handling whitespace
    while ((input[i] == ' '||input[i] == '	') && i<length)
    {
	    i++;
    }
    filenameOut[0]='\0';

    //Reads redirection filename
    if(flagOut > 0)
    {	
	    while (i<=length)
	    {
		    filenameOut[j] = input[i];
    		    i++;
		    j++;
    	    }
    }
    if (i < length)
    {
    	    if(flagIn > 0 && input[i] == '>')
	    {
		    i++;
		    flagOut = 1;
           	    if(input[i] == '>')
		    {
               		 flagOut=2;
               		 i++;
		    }
		    while ((input[i]==' '||input[i]=='	')&&i<length)
		    {
			i++;
		    }
		    while (i <= length)
           	    {
               		 filenameOut[j]=input[i];
               		 i++;
               		 j++;
		    }
	    }
            //handling errors of missing filename
            else
	    {
           	 fprintf(stderr,"Error! Missing filename!\n");
           	 return -1;
	    }
    }
  
    //Assign variable to function
    k=number(realCommand);
    analizedCommand=analize(realCommand);
    
    //Built-in Commands
    //exit, pwd, cd, setpath and help
    //The strcmp() function compares the user input
    //string and if it equals, exit
    if(strcmp(analizedCommand[0], "exit") == 0)
    {
	    for(i = 0;i < k; i++)
    	    {
		    free(analizedCommand[i]);
    	    }
	    free(analizedCommand);
	    free(realCommand);
	    exit(1);
	    return 1;
    }
    
    //The strcmp() function compares the user input
    //string and if it equals, print
    //the current working directory
    if(strcmp(analizedCommand[0],"pwd")==0)
    {
        char *directory;
        directory=getcwd(NULL,0);
        printf("Current working directory is: ");
        printf(directory,"%s");
        printf("\n");
        
        for(i = 0;i < k; i++)
        {
            free(analizedCommand[i]);
        }
        free(analizedCommand);
        free(realCommand);
        return 1;
    }
	
    //The strcmp() function compares the user input
    //string and if it equals, change to the user's
    //home directory if no arugments, if dir is provided
    //change to the specified directory
    if(strcmp(analizedCommand[0],"cd")==0)
    {
	    cdCommand(analizedCommand);
		
	    for(i = 0;i < k; i++)
    	    {
		    free(analizedCommand[i]);
    	    }
	    free(analizedCommand);
	    free(realCommand);
	    return 1;
    }
    
    //The strcmp() function compares the user input
    //string and if it equals, sets the path
    //user must provide at least one argument (directory)
    //The path is where falsh will look
    //for executable programs to run
    //if(strcmp(analizedCommand[0],"setpath")== 0)
    //{
    
    //}
    
    
    //The strcmp() function compares the user input
    //string and if it equals, list all
    //built-in commands descriptions
    if(strcmp(analizedCommand[0],"help")== 0)
    {
        printf("Falsh Built-in Commands \n");
        printf("exit: Exit the program \n" );
        printf("pwd: print the current working directory \n");
        printf("cd: change directory \n");
        printf("setpath: sets the path \n");
        
        for(i = 0; i < k; i++)
            free(analizedCommand[i]);
        free(analizedCommand);
        free(realCommand);
        return 1;
    }

    commandPath=isFileExist(analizedCommand[0]);
    if(commandPath == NULL)	//can't find the command
    {
	    fprintf(stderr,"command not found\n");
		
	    for(i = 0;i < k; i++)
    	    {
		    free(analizedCommand[i]);
    	    }
	    free(analizedCommand);
	    free(realCommand);
	    return -1;
    }
    
    //fork() function creates a new process
    //for redirection commands
    if((pid = fork()) == 0) 
    {
	    if(flagOut == 1)
	    {
    		    fdOut = open(filenameOut,O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR );
	    }
    	    if(flagOut == 2)
	    {
	    	    fdOut = open(filenameOut, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR );
	    }
	    if(fdOut == -1)
    	    {
    		    printf("Open out %s error \n", filenameOut);
		    return -1;
	    }
    	    if(flagOut > 0)
		    if(dup2(fdOut, STDOUT_FILENO) == -1)
			{
				fprintf(stderr,"Redirect Standard Out Error !\n");
				exit(1);
			}
    	    execv(commandPath,analizedCommand);
	    exit(1); //exit process
	}
    else
    {
        if(back == 0)
        {
		 pid=waitpid(pid, &status, 0);
        }
    }
	
	free(filenameOut);
	free(commandPath);
	for(i = 0; i < k; i++)
     	{
		free(analizedCommand[i]);
       	}
	free(analizedCommand);
	return 1;
}

//This function analize if it is background process
//and remove &
int isBack(char *command)
{
	int length = strlen(command);
	if(command[length] == '&')
	{
		command[length]='\0';
		return 1;
	}
	else
       	{
       		return 0;
       	}
}

//This function checks if the file exist
char *isFileExist(const char *command)
{
    char *path;
    char *p;
    char *buffer;
    int i = 0;
    int maxLength;
	
    //getenv() function obtains the current value of enviroment
    //here is "PATH"
    path = getenv("PATH");
    p = path;
    maxLength = strlen(path)+strlen(command)+2;
    if((buffer = (char *)malloc(maxLength*(sizeof(char)))) == 0)
    {
	    fprintf(stderr,"error, malloc failed! No enough space for buffer\n");
    	    return NULL;
    }
    while(*p != '\0') 
    {
    	    //Use : to seperate patH
	    if(*p != ':')
	    {
    		    buffer[i++] = *p;
	    }
    	    else 
	    {
	            buffer[i++] = '/';
        	    buffer[i] = '\0';
            
            	//strcat() function append a copy of command
           	 //to the end of buffer and gets pathname
           	 strcat(buffer,command);
           	 if(access(buffer,F_OK) == 0)  //file is found
           	 {
               		 return buffer;
          	 }
           	 //continue to look for other path
           	 else
		 {
			 i = 0;
	       	 }

	 	 p++;
    	    }
	//If all path is searched and not found, return NULL
	return NULL;
    }
}

//This function is to analize command and parameter number
int number(const char *input)
{
    //Declare
    int i=0;
    int k=0;
    int inputLength = strlen(input);
    k=0; //use k to mark command and parameter number
    int flag = 0;

    for (i=0;i<inputLength;i++)
    {
	    if(input[i] == ' '||input[i] == '>'||input[i] == '	')
	    {
		    flag = 0;
		    continue;
	    }
	    else 
	    {
		    if(flag == 0)
		    {
			    flag = 1;
			    k++;
		    }
	    }
    }
    return k;
}

//This function is to analize the user input command
//and store it in arg
char **analize(const char *input)
{
    //Declarations
    int number(const char *input);
    int i;
    int j;
    int k; //use k to mark command and parameter number
    int inputLength;
    int isBack = 0;

    char *buffer;
    char **arg; //store command and its parameter

    inputLength = strlen(input);

    if((buffer = (char *)malloc((inputLength+1)*(sizeof(char)))) == 0)
    {
	    fprintf(stderr,"error, malloc failed! No enough space for buffer\n");
	    return NULL;
    }

    //Assign, analize command and parameter number
    k = number(input);
    if((arg = (char **)malloc((k+1)*sizeof(char *))) == 0)
    {
	    fprintf(stderr,"error, malloc failed! No enough space for arg\n");
	    return NULL;
    }	

    //Seperation
    for (i  =0,j = 0,k = 0; i <= inputLength; i++)
    {
	    if(input[i] == ' '||input[i] == '>'||input[i] == '	'||input[i] == '\0')
	    {
		    if(j == 0)
		    {
			continue;
		    }
    		    else 
		    {
			    buffer[j] = '\0';
			    j++;
			    arg[k] = (char *)malloc(sizeof(char)*j);
	    
			    //strcpy() function copies the command into arG
			    strcpy(arg[k], buffer);
		    	    j=0;
	    		    k++;
		    }
    	    }
	    else 
	    {
		    if(input[i]== '&' && input[i+1]=='\0') 
    		    {
			    isBack = 1;
			    continue;
		    }
		    buffer[j]=input[i];
		    j++;
	    }
    }

    //When use exec, last parameter needs to be NULL.
    arg[k]=NULL;

    //free() function frees allocations that were created
    //via the preceding allocation functions.
    free(buffer);
    return arg;
}

