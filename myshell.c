#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv){
	//check for arguments cause if that's the case then you should run the script
	if(argc > 1){
		FILE *fp;
		while(--argc > 0){
			//i used the basic way "The C Programming Language" explains things to read from the files
			if((fp = fopen(*++argv, "r")) == NULL){
				if(((strcmp(*argv,"-h")) != 0)  && ((strcmp(*argv,"--help")) != 0)){
					printf("myshell cannot open %s\n", *argv);
					return 1;
				}
			}
			else{
				fclose(fp);
			}
		}		
	}
	else{
		while(1){
			printf("$ ");
		
			//first i need to read in the program and arguments i want to run
			char text[100];
			fflush(stdout); // http://c-faq.com/stdio/fflush.html 
  			fgets(text, sizeof text, stdin);
		
			//try to split it into tokens and put into arg array
			char whitechars[] = " \n\t\r";
			char* argArray[100];
			char *tok = NULL;
			tok = strtok(text, whitechars);
			int i = 0;
			while (tok != NULL) {
				if(tok != NULL){
					//add tok to the array of args
					argArray[i] = tok;
					//printf ("next argument is: %s\n", argArray[i]);
					tok = strtok(NULL, whitechars);
					i++;
				}
			}
			//now argArray has all the arguments in it
			//fork into two processes
			int pid = fork();
			int status = 0;
	
			if (pid == 0) {
 		  		//this process is the child and will call exec to replace itself
				execvp(argArray[0], argArray);				

	   			// this should never be reached, unless there is an error
				fprintf (stderr, "unknown command: %s\n", argArray[0]);
				exit(0);
			} 
			else {
	   			// the parent will
				// wait for child to finish
				waitpid(pid, &status, 0);
				
				//if there is an error show it otherwise the loop will replay
				if (status != 0) {
	      			fprintf  (stderr, "error: %s exited with status code %d\n", argArray[0], status);
	   			} 
			}
		}
	}
}
