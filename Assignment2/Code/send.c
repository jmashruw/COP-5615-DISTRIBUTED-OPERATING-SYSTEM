/* send.c - send, sendMsg, sendMsgs, sendnMsg */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*------------------------------------------------------------------------
 *  sendMsg  -  Apend a message to queue and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	sendMsg(
	  pid32		pid,	/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;                /* Saved interrupt mask */			

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	if (count[pid]<10){
	       	myCircularQ[pid][rear[pid]]= msg; //insert integer element at rear position
	       	rear[pid] = (rear[pid] + 1) % 10;  //increment rear by 1 
	        count[pid] = count[pid] + 1;
		}
	else {
		restore(mask);
		return SYSERR;	
	}

	/* If recipient waiting or in timed-wait make it ready */
	if (proctab[pid].prstate == PR_RECV) {
		ready(pid);
	} 
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*-------------------------------------------------------------------------------
 *  sendMsgs  -  Apend group of messages to queue and start recipient if waiting
 *-------------------------------------------------------------------------------
 */
uint32	sendMsgs(
	  pid32		pid,	/* ID of recipient process	*/
	  umsg32*	msgs,		/* Contents of message		*/
	  uint32 msg_count  /* No. of messages */	
	)
{
	intmask	mask;                /* Saved interrupt mask */
	int32 j=0;	

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	if(count[pid] >= 10 ){
		restore(mask);
		return SYSERR;
	}
	if(msg_count > (10-count[pid])){
		msg_count = (10-count[pid]);  //update msg_count to maximum limit a buffer can accept
		if(msg_count <0) msg_count = msg_count * -1;  //make it absolute
	}
	for(j=0; j<msg_count; j++){
		    myCircularQ[pid][rear[pid]]= msgs[j]; //insert integer element at rear position
		    rear[pid] = (rear[pid] + 1) % 10;  //increment rear by 1 
		    count[pid] = count[pid] + 1;
		}

	/* If recipient waiting or in timed-wait make it ready */
	if (proctab[pid].prstate == PR_RECV) {
		ready(pid);
	} 
	restore(mask);		/* Restore interrupts */
	return msg_count ;
}

/*---------------------------------------------------------------------------------------
 *  sendnMsg  -  Broadcast a message to multiple recepient and start recipient if waiting
 *---------------------------------------------------------------------------------------
 */
uint32	sendnMsg(
	  uint32 pid_count,   /* No. of processes */	
	  pid32* pids,	      /* ID of recipient process	*/
	  umsg32 msg		  /* Contents of message		*/
	)
{
	intmask	mask;                /* Saved interrupt mask */
	uint32 i=0;			

	mask = disable();

	while(i<pid_count){
		if (isbadpid(pids[i])) {
			restore(mask);
		}
		if (count[pids[i]]<10){ 
	       	myCircularQ[pids[i]][rear[pids[i]]]= msg; //insert integer element at rear position
	       	rear[pids[i]] = (rear[pids[i]] + 1) % 10;  //increment rear by 1 
	        count[pids[i]] = count[pids[i]] + 1;
	        i=i+1;
		}
		else 
			continue;

		/* If recipient waiting or in timed-wait make it ready */
		if (proctab[pids[i]].prstate == PR_RECV) {
			ready(pids[i]);
		}
	}	 
	
	restore(mask);		/* Restore interrupts */
	if(i==0) i=SYSERR;
	return i;
}
