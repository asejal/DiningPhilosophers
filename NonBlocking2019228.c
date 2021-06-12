/*
Aditi Sejal
2019228
*/
#include<pthread.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<unistd.h> 

//defining a structure for my_semaphore
typedef struct {
	pthread_mutex_t lock;	//mutex for locking critical sections and protecting them
	volatile int my_value;	//value of binary semaphore
} my_semaphore;

//sort of constructor to initialise all attributes of above structure and return a pointer to newly created my_semaphore object
my_semaphore *my_semaphore_init(){
	my_semaphore *s;
	s = (my_semaphore *)malloc(sizeof(my_semaphore));
	//if error (no memory avialable etc.) then return NULL
	if(s ==  NULL)
		return NULL;
	//initialize binary semaphore with 1
	s->my_value = 1;
	//initiliaze mutex 
	pthread_mutex_init(&(s->lock), NULL);
	return s;
}

//wait()
 void wait(my_semaphore *s){
	while(1){
		//acquire mutex
		pthread_mutex_lock(&(s->lock));
		//if value already 0 
    	if(s->my_value <= 0) {
    		//release mutex
    		pthread_mutex_unlock(&(s->lock));
    	}
    	//if value is non-zero
    	else{
    		//decrement semaphore value
    		s->my_value--;
    		//release mutex
    		pthread_mutex_unlock(&(s->lock));
    		break;
    	}    
	}
	return;
}

//signal()
void signal(my_semaphore *s){
	//acquire mutex
	pthread_mutex_lock(&(s->lock));
	//increment semaphore value
	s->my_value++;  
	//release mutex   
    pthread_mutex_unlock(&(s->lock));
}

//signal(printValue)
void signal_printer(my_semaphore *s){
	//print value of semaphore
	printf("%d\n",s->my_value);
}

//define a macro K
#define K n

//global array for storing philosopher's thread numbers
int *phil;

//my_semaphore for 2 rice bowls considered a single entity since both are essential to eat
my_semaphore *rice_bowls; 
//array of my_semaphore for K forks
my_semaphore **forks; 
//integer with which K is initialised by default by 5
volatile int n = 5;

//print which philosopher is eating with which forks
//takes semaphore of rice bowls as parameter and thread id of thread/philosopher
void eat(my_semaphore *s, int philo){
	//print value of rice bowl semaphore for debugging
	//UNCOMMENT TO CHECK
	// signal_printer(s);
	//print details of who is eating
	printf("Philosopher %d eats using forks %d and %d.\n", philo, philo, (philo+1)%5);
}

//main function describing actions of each philosopher
void* philospher(void* num) 
{ 
	while (1) { 
		int i = *(int*)num; 
		//think for sometime
		sleep(1);
		//to avoid deadlock 0th philosopher picks the right fork first followed by left
		if(i == 0){
			//pick right fork
			wait(forks[(i+1)%K]);
			//pick left fork
			wait(forks[i]);
			//pick rice bowls
			wait(rice_bowls);
		}
		//all other philosophers pick left fork first followed by right
		else{
			//pick left fork
			wait(forks[i]);
			//pick right fork
			wait(forks[(i+1)%K]);
			//pick rice bowls
			wait(rice_bowls);
		}
		//eat
		eat(rice_bowls, i);

		//put left fork down
		signal(forks[i]);
		//put right fork down
		signal(forks[(i+1)%K]);
		//keep rice bowls back 
		signal(rice_bowls);
	} 
} 

int main() 
{ 

	//take user input for K
	printf("Enter value of K\n");
	scanf("%d", &n);

	//declare threads
	int i; 
	pthread_t thread_id[K]; 

	//initialize arrays of size K as defined above
	phil = malloc(K*sizeof(int));
	forks = malloc(K*sizeof(forks));


	for(int j = 0; j<K; j++){
		phil[j] = j;
	}

	// initialize the semaphores 
	rice_bowls = my_semaphore_init(); 

	// initialize the semaphores 
	for (i = 0; i < K; i++) 
		forks[i] = my_semaphore_init();

	//create threads to represent philosophers
	for (i = 0; i < K; i++) 
		pthread_create(&thread_id[i], NULL, &philospher, &phil[i]); 

	//wait for all to eat 
	for (i = 0; i < K; i++) 
		pthread_join(thread_id[i], NULL); 
} 

