#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include<semaphore.h>
#include<errno.h>

// n buffers
# define n 20
char arr[n];
int idx=0;



pthread_mutex_t mtx;
sem_t full;
sem_t empty;

void *producer_thread(void *ptr){
	printf("producer thread\n");
        while(1){
		//wait untill the buffer has an element
		sem_wait(&empty);
        	pthread_mutex_lock(&mtx);
		
		arr[idx] = 'A';
        	idx++;
		printf("%s %d\n", arr, idx);

        	pthread_mutex_unlock(&mtx);
        	sem_post(&full);

		usleep((rand()%10)*100000);
	}

}

void *consumer_thread(void *ptr){
        printf("consumer thread\n");
        while(1){
		// A consumer thread is created before a producer thread
                sem_wait(&full);
                pthread_mutex_lock(&mtx);
		
		idx--;
                arr[idx] = '\0';
                printf("[%s] %d\n", arr, idx);

		pthread_mutex_unlock(&mtx); 
		sem_post(&empty);
		
                usleep((rand()%10)*100000);
        }
}

int main()
{	
	// Fill arr[i] with null character.
	// producer function produces only 'A' character
	for(int i=0; i<n; i++){
		arr[i] = '\0';
	}

	//Initializing full = 0, empty = n semaphores
	sem_init(&full, 0, 0);	
	sem_init(&empty, 0, n);

	pthread_mutex_init(&mtx, NULL);

	pthread_t producer, consumer;	

	// A consumer thread is created before a producer thread
	int pid1 = pthread_create(&consumer, NULL, consumer_thread, (void*)NULL);
	
	usleep(1000);

	int pid2 = pthread_create(&producer, NULL, producer_thread, (void*)NULL);
	
	pthread_join(consumer, NULL);
	pthread_join(producer, NULL);
	
	printf("\n");
	exit(0);
	
	return 0;
}







