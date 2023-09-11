#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include<semaphore.h>
#include<errno.h>


typedef struct node{
	char val;
	struct node *next;
}node_t;


void push(char data, struct node** s){
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	new_node->val = data;
	new_node->next = *s;
	*s = new_node;
}

void pop(struct node** s){
	if(*s == NULL){
		printf("The stack is empty\n");
	}
	else{
		//printf("Popped: %c\n", (*s)->val);
		struct node* temp = *s;
		*s = (*s)->next;
		temp->next = NULL;
		free(temp);
	}
}

void top(struct node* s){
	if(s != NULL){
		printf("Top: %c\n", s->val);
	}
	else{
		printf("The stack is empty\n");
	}
}

void clear(struct node** s){
	while(*s != NULL){
		pop(s);
	}
	printf("All clear\n");
}

//Function to print all all the elements
void print_node(struct node** s){
	struct node* temp = *s;
	while(temp != NULL){
		printf("%c", temp->val);
		temp = temp->next;
	}
}

//Declaration of a node
struct node* s;
// this variable play the same role as in the first task
int idx=0;


pthread_mutex_t mtx;
sem_t full;
sem_t empty;


void *producer_thread(void *ptr){
	printf("producer thread\n");
        while(1){
		// wait untill the buffer has an empty place
		sem_wait(&empty);
        	pthread_mutex_lock(&mtx);
		
		push('A', &s);
		idx++;
		
		print_node(&s);
		printf(" %d\n", idx);

        	pthread_mutex_unlock(&mtx);
		sem_post(&full);
		
		usleep((rand()%10)*100000);
	}

}


void *consumer_thread(void *ptr){
	printf("consumer thread\n");
        while(1){
		//wait untill the buffer has an element
                sem_wait(&full);
                pthread_mutex_lock(&mtx);
		
		pop(&s);
		idx--;
		
		printf("[");
		print_node(&s);
		printf("] %d\n", idx);
		
		pthread_mutex_unlock(&mtx);
	       	sem_post(&empty);
               	
		usleep((rand()%10)*100000);
        }
}


int main()
{
	// buffer size
	int n = 20;

	//Initializing full = 0, empty = n semaphores
	sem_init(&full, 0, 0);	
	sem_init(&empty, 0, n);

	pthread_mutex_init(&mtx, NULL);

	pthread_t producer, consumer;	
	
	// A consumer thread is created before a producer thread
	int pid1 = pthread_create(&consumer, NULL, consumer_thread, (void*)NULL);
	
	sleep(1);

	int pid2 = pthread_create(&producer, NULL, producer_thread, (void*)NULL);
	
	pthread_join(consumer, NULL);
	pthread_join(producer, NULL);
	
	clear(&s);
	printf("\n");

	return 0;
}







