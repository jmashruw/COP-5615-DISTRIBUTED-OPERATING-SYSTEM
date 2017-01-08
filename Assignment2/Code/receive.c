/* receive.c - receive, receiveMsg, receiveMsgs */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}

/*------------------------------------------------------------------------------------
 *  receiveMsg  -  Wait for a message and return the message to the caller from queue
 *------------------------------------------------------------------------------------
 */
umsg32	receiveMsg(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	umsg32	msg=0;			/* Message to return		*/

	mask = disable();
	if (count[currpid]==0) {
		proctab[currpid].prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	/* Retrieve message		*/
	if(count[currpid] > 0){
        msg = myCircularQ[currpid][front[currpid]]; //remove integer element from front position
		front[currpid] = (front[currpid] + 1) % 10;  //increment front by 1 and ensure it points to 1st loc when it reaches buffer limit      
		count[currpid] = count[currpid] - 1;
	}

	/* Reset message flag*/
	restore(mask);
	return msg;
}

/*---------------------------------------------------------------------------------------------
 *  receiveMsgs  -  Wait for group of messages and return the message to the caller  from queue
 *----------------------------------------------------------------------------------------------
 */
syscall	receiveMsgs(umsg32* msgs, uint32 msg_count)
{
	intmask	mask;			/* Saved interrupt mask		*/
	uint32	i=0;			/* Message to return		*/

	mask = disable();

	if (count[currpid]==0 || count[currpid]<msg_count) {
		proctab[currpid].prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	else{
		for(i=0;i<msg_count; i++){
			        msgs[i] = myCircularQ[currpid][front[currpid]]; //remove integer element from front position
				front[currpid] = (front[currpid] + 1) % 10;  //increment front by 1 and ensure it points to 1st loc when it reaches buffer limit      
				count[currpid] = count[currpid] - 1;
			}
		} 
	restore(mask);		/* Restore interrupts */
	return OK;
}
