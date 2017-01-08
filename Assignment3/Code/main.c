#include <xinu.h>

#define TOPIC_SIZE 256

//Declaring processes here
pid32 pub1, pub2;
pid32 sub1, sub2, sub3, sub4;

//Declaring semaphore variables here
extern sid32 mx;

void mutex_acquire(sid32 mutex){
	wait(mutex);
}

void mutex_release(sid32 mutex){
	signal(mutex);
}

void foo(topic16 t, uint32 m){
	mutex_acquire(mx);
	kprintf("\nFunction foo is called with topic16 %d and data %d",t,m);
	mutex_release(mx);
}

void pick(topic16 t, uint32 m){
	mutex_acquire(mx);
	kprintf("\nFunction pick is called with topic16 %d and data %d",t,m);
	mutex_release(mx);
}

void bar(topic16 t, uint32 m){
    mutex_acquire(mx);
    kprintf("\nFunction bar is called with topic16 %d and data %d",t,m);
    mutex_release(mx);
}

void drop(topic16 t, uint32 m){
    mutex_acquire(mx);
    kprintf("\nFunction drop is called with topic16 %d and data %d",t,m);
    mutex_release(mx);
}

process publisher1(void){
	
    uint32 m = 100;
    publish(512, m);
	mutex_acquire(mx);
    kprintf("\nProcess %d publishes message %d to topic 512",currpid, m);
	mutex_release(mx);
    
    uint32 p = 200;
    publish(256, p);
    mutex_acquire(mx);
    kprintf("\nProcess %d publishes message %d to topic 256",currpid, p);
    mutex_release(mx);
    
    uint32 q = 300;
    publish(0, q);
    mutex_acquire(mx);
    kprintf("\nProcess %d publishes message %d to topic 0",currpid, q);
    mutex_release(mx);
    
    return OK;
}

process publisher2(void){
	
    uint32 n = 400;
    publish(512, n);
    mutex_acquire(mx);
    kprintf("\nProcess %d publishes message %d to topic 512",currpid, n);
    mutex_release(mx);
    
    uint32 r = 500;
    publish(1, r);
    mutex_acquire(mx);
    kprintf("\nProcess %d publishes message %d to topic 1",currpid, r);
    mutex_release(mx);
   
    uint32 s = 600;
    publish(0, s);
    mutex_acquire(mx);
    kprintf("\nProcess %d publishes message %d to topic 0",currpid, s);
    mutex_release(mx);
    
    return OK;
}

process subscriber1(void){
    subscribe(512, foo);
	mutex_acquire(mx);
    kprintf("\nProcess %d subscribes to topic 512 with handler foo",currpid);
	mutex_release(mx);
    sleep(5);
	return OK;
}

process subscriber2(void){
    subscribe(256, pick);
    mutex_acquire(mx);
    kprintf("\nProcess %d subscribes to topic 256 with handler pick",currpid);
    mutex_release(mx);
    sleep(5);
    return OK;
}

process subscriber3(void){
    subscribe(0, bar);
    mutex_acquire(mx);
    kprintf("\nProcess %d subscribes to topic 0 with handler bar",currpid);
    mutex_release(mx);
    sleep(5);
    return OK;
}

process subscriber4(void){
    subscribe(1, drop);
    mutex_acquire(mx);
    kprintf("\nProcess %d subscribes to topic 1 with handler drop",currpid);
    mutex_release(mx);
    sleep(5);
    return OK;
}

process	main(void)
{
	recvclr();

	pub1 = create(publisher1, 4096, 50, "publisher1", 0);
	pub2 = create(publisher2, 4096, 50, "publisher2", 0);
	sub1 = create(subscriber1, 4096, 50, "subscriber1", 0);
	sub2 = create(subscriber2, 4096, 50, "subscriber2", 0);
	sub3 = create(subscriber3, 4096, 50, "subscriber3", 0);
	sub4 = create(subscriber4, 4096, 50, "subscriber4", 0);
    
	resched_cntl(DEFER_START);
	resume(pub1);
	resume(pub2);
	resume(sub1);
    resume(sub2);
    resume(sub3);
    resume(sub4);
	resched_cntl(DEFER_STOP);

	return OK;
}
