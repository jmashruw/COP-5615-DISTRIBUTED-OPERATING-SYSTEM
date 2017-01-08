/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

//Declaring constant values for use throughout your code
#define ARR_SIZE 250
#define RESTRICT 1700


pid32 producer_id;
pid32 consumer_id;
pid32 timer_id;

int32 consumed_count = 0;
const int32 CONSUMED_MAX = 100;

//Declaring circular buffer structure here
int32 myQueue[ARR_SIZE];
int32 rear=0, front=0, data=-1, d=1;

//Declaring semaphore variables here
sid32 pr;
sid32 cr;
sid32 m;

//Defining process to insert integer element in queue
void insertIntoQ(int32 n)
{
	 myQueue[rear] = n; //insert integer element at rear position
     rear = (rear + 1) % ARR_SIZE; //increment rear by 1 
     							   //Ensuring it points to 1st loc when it reaches buffer limit
}

//Defining process to remove integer element in queue
int32  removeFromQ()
{
      data = myQueue[front]; //remove integer element from front position
      front = (front + 1) % ARR_SIZE; //increment front by 1 
     							   //Ensuring it points to 1st loc when it reaches buffer limit
      return data;
}

/* Code for entering a critical section */
void mutex_acquire(sid32 mutex)
{
	wait(mutex); //decrement semaphore count by 1
}

/* Code for leaving a critical section */
void mutex_release(sid32 mutex)
{
	signal(mutex); //increment semaphore count by 1
}

/* Code for the buffer producer*/
process producer(void)
{
	while(d<=RESTRICT) { //Continue producing till certain limit
			mutex_acquire(pr);	//acquire the producer semaphore
			insertIntoQ(d);  //insert element
			mutex_acquire(m); //acquire the mutex
			printf("Produced: %d \n",d);  //print to console
			mutex_release(m); //release the mutex
			d += 1; // increment 'd'
			mutex_release(cr); //signal the consumer semaphore
		}		
	return OK;
	}

/* Place the code for the buffer consumer here */
process consumer(void)
{
	 int32 j=1;
	 int32 x,y;
	 while(j<=RESTRICT){ //Continue consumimg till certain limit
		 mutex_acquire(cr); //acquire the consumer semaphore
		 x = removeFromQ();  //delete element
		 y=front; // print loc of element dequeued
       	 mutex_acquire(m); //acquire the mutex
       	 printf("Is %d power of 2 at buffer location %d : %s \n",x,y-1,(x && (!(x&(x-1))))?"Yes":"No"); //print to console
		 mutex_release(m); //release the mutex
		 j += 1;	// increment 'j'
		 consumed_count += 1; // increment 'consumed_count'
		 mutex_release(pr); //signal the producer semaphore		
	 }
	return OK;
}

/* Timing utility function - please ignore */
process time_and_end(void)
{
	int32 times[5];
	int32 i;

	for (i = 0; i < 5; ++i)
	{
		times[i] = clktime_ms;
		yield();

		consumed_count = 0;
		while (consumed_count < CONSUMED_MAX * (i+1))
		{
			yield();
		}

		times[i] = clktime_ms - times[i];
	}

	kill(producer_id);
	kill(consumer_id);

	for (i = 0; i < 5; ++i)
	{		
		kprintf("TIME ELAPSED (%d): %d\n", (i+1) * CONSUMED_MAX, times[i]);
	}
}

process	main(void)
{
	recvclr();

	printf("\n\n This is semaphore execution!! \n\n");
	
	/* Define semaphores */
	pr = semcreate (ARR_SIZE);
	cr = semcreate (0);
	m = semcreate(1);

	producer_id = create(producer, 4096, 50, "producer", 0);
	consumer_id = create(consumer, 4096, 50, "consumer", 0);
	timer_id = create(time_and_end, 4096, 50, "timer", 0);

	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	resume(timer_id);
	resched_cntl(DEFER_STOP);

	return OK;
}

