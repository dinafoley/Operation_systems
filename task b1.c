#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include<semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char* argv[])
{	
	//check the number of arguments	
	if(argc < 3){
		fprintf(stderr, "like the usage: ./executionFile <input c file> <output file>\n");
		exit(1);	
	}

	// two children 
	int id_ch2;
	int id_ch1 = fork();
	
	if(id_ch1 == 0){	
		//printf("-------In child 1--------\n"); 	

		execlp("gcc", "gcc", argv[1], "-o", argv[2], "-pthread",  (char*)NULL);
		
		// if execlp fails
		fprintf(stderr, "ERROR in id_ch1: %s\n", strerror(errno));
		exit(1);
	}
	else{	
		//Waiting for id_ch1 to finish
		wait(NULL);

		id_ch2 = fork();
		if(id_ch2 < 0){
			fprintf(stderr, "Forking id_ch2 failed\n");
			exit(1);
		}
	}
	if(id_ch2 == 0){
		int n = strlen(argv[2])+2;
		char* temp = malloc(n);
		
		// temp = ./argv[2]
		strcpy(temp, "./");
		strcat(temp, argv[2]);
       			
		execlp(temp, temp, (char*)NULL);	

		//if execlp fails
		fprintf(stderr, "ERROR in id_ch2: %s\n", strerror(errno)); 
		exit(1);	
	}
	else{
		//wait the second child
		wait(NULL);
		fprintf(stdout, "main (the parent) called the exit(0)\n");
		exit(0);
	}
	
	return 0;
}
