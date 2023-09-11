#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include<semaphore.h>

// Two semaphores are needed to track which letter is being printed
sem_t sem_upper;
sem_t sem_lower;

pthread_mutex_t mutex;

void *flush_lower(void *ptr){
	char *s;
	s = (char *) ptr;
	int l = strlen(s);

	// When srtlen(s1) < strlen(s2) sem_lower = srtlen(s2). 
	// to make sure the first printed character is an uppercase letter
	sleep(rand()%4);

	for(int i = 0; i < l; i++){
		//wait until a character is printed in flush_upper
		// wait until sem_lower>0
		sem_wait(&sem_lower);
		pthread_mutex_lock(&mutex);

		// the following 3 lines cannot be changed (content + order)
		printf("%c", s[i]);
		fflush(stdout);
		sleep(rand() % 2);

		//release the lock for flush_upper
		// Now sem_upper > 0 
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_upper);
		sleep(2);
	}
	
}

void *flush_upper(void *ptr){
	char *s;
	s = (char *) ptr;
	int l = strlen(s);
	
	for(int i = 0; i < l; i++){
		// wait until a lower character is printed in flush_lower
		// wait until sem_upper > 0
		sem_wait(&sem_upper);
		pthread_mutex_lock(&mutex);

		// the following 3 lines cannot be changed (content + order)
		printf("%c", s[i]);
		fflush(stdout);
		sleep(rand() % 2);   
		
		// release the lock for flush_lower 
		// Now sem_lower > 0
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_lower);
		sleep(2);
	}
	
}

/*
 * There are 3 possible cases:
 * Case1: srtlen(s1) == strlen(s2): sem_upper = 1, sem_lower = 0;
 * Case2: strlen(s1) > strlen(s2): all characters of lowercase word are printed
 * 					and sem_upper == strlen(s1). 
 * Case3: strlen(s1) < strlen(s2): all characters of uppercase word are printed
 * 					and sem_lower == strlen(s2).
 *					
 *	Why semaphores? 
 *	Mutex provides mutual exclusion, while counting semaphores are needed 
 *	to initialize the semaphores with the value bigger than 1 
 * */

int main()
{	
	// Initializing two semaphores
	// Uppercase must be printed first, so initially sem_upper = 1 or len(s1), sem_lower = 0 or len(s2)

	char *s1 = "abcdeeerrrrr";
	char *s2 = "ABCVFF";


	sem_init(&sem_upper, 0, (strlen(s2) > strlen(s1))? strlen(s2) : 1);	
	sem_init(&sem_lower, 0, (strlen(s2) < strlen(s1))? strlen(s1) : 0);
	
	pthread_mutex_init(&mutex, NULL);

	pthread_t t1, t2;	

	int pid1 = pthread_create(&t1, NULL, flush_lower, (void*)s1);
	int pid2 = pthread_create(&t2, NULL, flush_upper, (void*)s2);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	printf("\n");
	exit(0);
	
	return 0;
}







