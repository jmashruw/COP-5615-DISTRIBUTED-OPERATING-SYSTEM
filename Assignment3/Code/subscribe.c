#include <xinu.h>

#define TOPIC_SIZE 256

extern void (*myhandler[TOPIC_SIZE][NPROC]) (topic16, uint32);
extern pid32  subsTopic[TOPIC_SIZE][NPROC];
extern topic16 subsGroup[NPROC];

syscall subscribe(topic16 topic, void (*handler)(topic16, uint32)){

    intmask	mask;			/* Saved interrupt mask		*/
    mask = disable();

    myhandler[topic & 255][currpid] = handler;  //store process's handler
    subsTopic[topic & 255][currpid] = currpid; //store process's id
    subsGroup[currpid] = topic>>8;	//store process's group

    restore(mask);		/* Restore interrupts */
    return OK;

}

