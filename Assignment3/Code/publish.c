#include <xinu.h>

#define TOPIC_SIZE 256

extern topic16 topics[TOPIC_SIZE];
extern topic16 grps[TOPIC_SIZE];
extern uint32 buffer[TOPIC_SIZE];

extern int32 brear;

extern sid32 b_insert, b_read;

syscall publish(topic16 topic, uint32 data){
    
    wait(b_insert);	
    
    intmask mask;			/* Saved interrupt mask		*/
    mask = disable();

    topics[brear] = topic & 255; //store only topic bits(lower 8 bits) 
    grps[brear] = topic >> 8; //store only group bits(higher 8 bits) 	
    buffer[brear] = data; //store data
    
    brear = (brear + 1) % TOPIC_SIZE;

    restore(mask);		/* Restore interrupts */
    
    signal(b_read);
    		
    return OK;		
}
