/* userret.c - userret */

#include <xinu.h>

#define TOPIC_SIZE 256

extern void (*myhandler[TOPIC_SIZE][NPROC]) (topic16, uint32);
extern pid32  subsTopic[TOPIC_SIZE][NPROC];
extern topic16 subsGroup[NPROC];

/*------------------------------------------------------------------------
 *  userret  -  Called when a process returns from the top-level function
 *------------------------------------------------------------------------
 */
void	userret(void)
{
    int32 i;
    subsGroup[getpid()] = 0; //get topic id
    for(i = 0; i < TOPIC_SIZE; i++){
        myhandler[i][getpid()] = NULL;
        subsTopic[i][getpid()] = 0;
    }
}
