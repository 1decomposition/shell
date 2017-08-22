/* Simple Shell
 *
 * After reading user input, the steps are:
 * (1) fork a child process using fork()
 * (2) the child process will invoke execvp()
 * (3) if command included &, parent will NOT invoke wait()                      
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#define MAXLINE 80 /* The maximum length command */

//reads user input
char *readLine()
{
	char *buffer;

	buffer = malloc(sizeof(char) * MAXLINE + 1);
	if(buffer == NULL)
	{
		perror("malloc error");
		exit(1);
	}
	fgets(buffer, MAXLINE+1, stdin);

	return buffer;
}

int exec_command(char **args, int i)
{
	pid_t pid;
	
	pid = fork();
	//child process
	if(strcmp(args[i-1], "&") != 0)
		wait(NULL);
	if (pid == 0)
	{
		if(strcmp(args[i-1], "&") == 0)
			args[i-1] = NULL;
		if (execvp(args[0], args) < 0)
		{
			write(1,"command not found:",18);
			printf("%s\n", args[0]);
		}
		exit(1);
	}
	//error in creating fork
	else if(pid < 0)
	{
		write(1,"Fork Error\n",11);
		exit(-1);
	}

	return (0);
}

int main(void)
{
	char *args[MAXLINE/2 + 1]; /* command line with max 40 arguments */ 
	char *buffer;
	int should_run = 1; /* flag to determine when to exit program */
	int i = 0;

       	printf("CS149 Shell from Justin Coy\n"); /* replace w/ name */ 
	while (should_run) 
	{
		printf("Justin-860>"); /* prompt- replace FirstName and L3SID */
		buffer = readLine(); //read user input
		//breaks user input into individual tokens, adds them into args
		buffer = strtok(buffer, " \n\t\r\a");
		if(strcmp(buffer, "exit") == 0)
			should_run = 0;
		else 
		{
			while(buffer != NULL)
			{
				args[i] = buffer;
				i++;
				buffer = strtok(NULL, " \n\t\r\a");
			}
			args[i] = NULL;
			exec_command(args,i);
		}

		i = 0;
		fflush(stdout);

	}
	free(buffer);
	return (0);
}
