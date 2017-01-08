#include <xinu.h>

#define TOPIC_SIZE 256

extern void (*myhandler[TOPIC_SIZE][NPROC]) (topic16, void*,uint32);
extern pid32  subsTopic[TOPIC_SIZE][NPROC];

syscall unsubscribe(topic16 topic){
    
    intmask	mask;			/* Saved interrupt mask		*/
    mask = disable();
    
    myhandler[topic & 255][currpid] = NULL;
    subsTopic[topic & 255][currpid] = 0;
    
    restore(mask);		/* Restore interrupts */
    return OK;
    
}
