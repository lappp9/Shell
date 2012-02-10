#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h> 


//gcc -lreadline -lhistory -o myshell myshell.c

int main(int argc, char **argv, char *envp[]){
	//just add the pid to the children array each time one is made
	int children[1000];
	int childCount = 0;
	int i, status;
	while(1){
		
		//reap zombies before accepting more input
		for(i=0; i<childCount; i++){
			if(children[i] != 0){
				if(waitpid(children[i],&status,WNOHANG) == 0){	
					continue;
				}
			}
		}
		char *line = readline("$ ");
		if(line != NULL){
			add_history (line);		
		}
		
		
		//try to split it into tokens and put into arg array
		char whitechars[] = " \n\t\r";  
		char* argArray[100];
		char *tok = NULL;
		tok = strtok(line, whitechars);
		
		int i = 0;
		//parse for command
		while (tok != NULL) {
			//add tok to the array of args
			argArray[i] = tok;
			
			//figure out if there is redirection intended for project 3
			/*if(strcmp(argArray[i],'<')==0){
				int redirectCharPos = i;
				char* direction = "left";
				printf("left");
			}	
			else if (strcmp(argArray[i],'>')==0){
				int redirectCharPos = i;
				char* direction = "right";
				printf("right");
			}*/
			//printf ("next argument is: %s\n", argArray[i]);
			tok = strtok(NULL, whitechars);
			i++;
		}
		argArray[i] = NULL;
		
		//check for no input
		if(argArray[0] == NULL){
			continue;
		}
		//check for exit command
		if(strcmp(argArray[0],"exit") == 0 ){
			printf("Exiting...\n");
			exit(0);
		}	
		//check for kill command
		if(strcmp(argArray[0], "kill") == 0){
			kill(argArray[1],0);
		}

		//check for changing directory
		if(strcmp(argArray[0], "cd" ) == 0){
			
			if(argArray[1] == NULL){
				chdir(getenv("HOME"));
			}
			else if(chdir(argArray[1])==-1){
				printf(" cd: %s: No such file or directory\n",argArray[1]);
			}

			continue;
		}
		
		//check to see if the child will be a background process
		int background = 0;
		if(strcmp(argArray[i-1],"&")==0){
			background = 1;
			argArray[(i-1)] = NULL;
		}
		
		//now argArray has all the arguments in it
		//fork into two processes
		if(strcmp(argArray[0],"jobs") == 0){
			int j;
			char* alive;
			for(j = 0; j<childCount; j++){
				if(kill(children[j],0) == 0){
					alive = "Running";
				}
				else{
					alive = "Done";
				}
				printf("[%d]    %s        %d\n", (childCount-j), alive,children[j]);
			}
			
		}
		//come on
		int pid = fork();
		int status = 0;
		if (pid == 0) {
 	 		//this process is the child and will call exec to replace itself
			//to be able to redirect the output to a file instead of stdout you close
			//file descriptor 1 and then immediately open a file which will fill the spot
			//you just closed
			//ex close(1)/
			//	 FILE* f = fopen('whateverNameTheyGave');
			fclose(stdin);
			fopen("/dev/null", "r");
			execvp(argArray[0], argArray);	
	   		// this should never be reached, unless there is an error
			fprintf (stderr, "unknown command: %s\n", argArray[0]);
			exit(0);
		} 
		else 
		{
	   		// the parent will
			// wait for child to finish unless it is in the background
			if(background == 1){
				children[childCount] = pid;
				childCount++;
				printf("[%d] %d\n", childCount, pid);
			}
			else{
				waitpid(pid, &status, 0);
			}
		
			//if there is an error show it otherwise the loop will replay
			if (status != 0) {
	      		fprintf  (stderr, "error: %s exited with status code %d\n", argArray[0], status);
	      		break;
	   		} 
		}
	}
}
