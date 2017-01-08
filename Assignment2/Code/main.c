/*  main.c  - main */
#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

/*Send one message to one receiver*/
pid32 send1;
pid32 rec1;

/*Send 1 message to 3 receiver */
pid32 send2;
pid32 rec2;
pid32 rec3;
pid32 rec4;


/*3 Senders will send 4 messages to 3 receivers */
pid32 send3;
pid32 send4;
pid32 send5;
pid32 rec5;
pid32 rec6;
pid32 rec7;

/*3 Senders will send 11 messages to 1 receiver */
pid32 send6;
pid32 send7;
pid32 send8;
pid32 rec8;

/*3 Senders will send 11 messages to 3 receivers */
pid32 send9;
pid32 send10;
pid32 send11;
pid32 rec9;
pid32 rec10;
pid32 rec11;

umsg32 d=1;

//Declaring semaphore variables here
sid32 mx;

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

/*Send one message to one receiver*/
process sender1(void){
	umsg32 m = 100;
	sendMsg(rec1, m);
	mutex_acquire(mx);
    printf("\n Sending msg %d to receiver %d. Number of message successfully sent is %d", m, rec1, 1);
	mutex_release(mx);
	return OK;
}

process receiver1(void){
	umsg32 m=0;
	m=receiveMsg();
	mutex_acquire(mx);
	printf("\n Receiver %d received Message: %d", currpid, m);
	mutex_release(mx);
	return OK;
}

/*Send 1 message to 3 receiver */

process sender2(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m = 101;
    pid32 p_ids[3];
    p_ids[0] = rec2;
    p_ids[1] = rec3;
    p_ids[2] = rec4;
	k=sendnMsg(3, p_ids, m);
	mutex_acquire(mx);
    for(i=0; i<k; i++){
        printf("\n Sending msg %d to receiver %d. Number of message successfully sent is %d", m, p_ids[i], 1);
    }
	mutex_release(mx);
	return OK;
}

process receiver2(void){
	umsg32 a=0;
	a=receiveMsg();
	mutex_acquire(mx);
    printf("\n Receiver %d received Message: %d", currpid, a);
	mutex_release(mx);
	return OK;
}

process receiver3(void){
	umsg32 b=0;
	b=receiveMsg();
	mutex_acquire(mx);
	printf("\n Receiver %d received Message: %d", currpid, b);
	mutex_release(mx);
	return OK;
}

process receiver4(void){
	umsg32 c=0;
	c=receiveMsg();
	mutex_acquire(mx);
	printf("\n Receiver %d received Message: %d", currpid, c);
	mutex_release(mx);
	return OK;
}

/*3 Senders will send 4 messages to 3 receivers */

process sender3(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[4] = {102, 103, 104, 105};
	k=sendMsgs(rec5, m, 4);
	mutex_acquire(mx);
    for(i=0; i<4; i++){
        printf("\n Sending msg %d to receiver %d.", m[i], rec5);
    }
    printf("\n Number of message successfully sent is %d", k);
	mutex_release(mx);
	return OK;
}

process sender4(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[4] = {106, 107, 108, 109};
	k=sendMsgs(rec6, m, 4);
	mutex_acquire(mx);
    for(i=0; i<4; i++){
        printf("\n Sending msg %d to receiver %d.", m[i], rec6);
    }
    printf("\n Number of message successfully sent is %d", k);
	mutex_release(mx);
    return OK;
}

process sender5(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[4] = {110, 111, 112, 113};
	k=sendMsgs(rec7, m, 4);
	mutex_acquire(mx);
    for(i=0; i<4; i++){
        printf("\n Sending msg %d to receiver %d.", m[i], rec7);
    }
    printf("\n Number of message successfully sent is %d", k);
	mutex_release(mx);
	return OK;
}

process receiver5(void){
	int32 i=0;
    umsg32 msges5[4];
	receiveMsgs(msges5, 4);
    mutex_acquire(mx);
	for(i=0; i<4; i++){
		printf("\n Receiver %d received Message: %d", currpid, msges5[i]);
	}
    mutex_release(mx);
	return OK;
}

process receiver6(void){
	int32 i=0;
    umsg32 msges6[4];
	receiveMsgs(msges6, 4);
    mutex_acquire(mx);
	for(i=0; i<4; i++){
		printf("\n Receiver %d received Message: %d", currpid, msges6[i]);
	}
    mutex_release(mx);
	return OK;
}

process receiver7(void){
	int32 i=0;
    umsg32 msges7[4];
	receiveMsgs(msges7, 4);
    mutex_acquire(mx);
	for(i=0; i<4; i++){
		printf("\n Receiver %d received Message: %d", currpid, msges7[i]);
	}
    mutex_release(mx);
	return OK;
}

/*3 Senders will send 8, 3, 2 messages to 1 receiver */

process sender6(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[8] = {1, 2, 3, 4,5,6,7,8};
	k=sendMsgs(rec8, m, 8);
	mutex_acquire(mx);
    if(k!=SYSERR){
        for(i=0; i<8; i++){
            printf("\n Sending msg %d to receiver %d.", m[i], rec8);
        }
        printf("\n Number of message successfully sent is %d", k);
    }
    else printf("\n3 ERROR while sending to receiver %d", rec8);
	mutex_release(mx);
	return OK;
}

process sender7(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[3] = {21,22,23};
	k=sendMsgs(rec8, m, 3);
	mutex_acquire(mx);
    if(k!=SYSERR){
        for(i=0; i<3; i++){
            printf("\n Sending msg %d to receiver %d.", m[i], rec8);
        }
        printf("\n Number of message successfully sent is %d", k);
    }
    else printf("\n3 ERROR while sending to receiver %d", rec8);
	mutex_release(mx);
	return OK;
}

process sender8(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[2] = {31,32};
	k=sendMsgs(rec8, m, 2);
	mutex_acquire(mx);
    if(k!=SYSERR){
        for(i=0; i<2; i++){
            printf("\n Sending msg %d to receiver %d.", m[i], rec8);
        }
        printf("\n Number of message successfully sent is %d", k);
    }
    else printf("\n3 ERROR while sending to receiver %d", rec8);
	mutex_release(mx);
	return OK;
}

process receiver8(void){
	int32 i=0;
    umsg32 msges8[10];
	receiveMsgs(msges8, 10);
    mutex_acquire(mx);
	for(i=0; i<10; i++){
		printf("\n Receiver %d received Message: %d", currpid, msges8[i]);
	}
    mutex_release(mx);
	return OK;
}

/*3 Senders will send 11 messages to 3 receivers */

process sender9(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[11] = {1,2,3, 4,5,6,7,8,9,10,11};
	k=sendMsgs(rec9, m, 11);
	mutex_acquire(mx);
    if(k!=SYSERR){
        for(i=0; i<10; i++){
            printf("\n Sending msg %d to receiver %d.", m[i], rec9);
        }
        printf("\n Number of message successfully sent is %d", k);
    }
    else printf("\n3 ERROR while sending to receiver %d", rec9);
	mutex_release(mx);
	return OK;
}

process sender10(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[11] = {21,22,23,24,25,26,27,28,29,30,31};
	k=sendMsgs(rec10, m, 11);
	mutex_acquire(mx);
    if(k!=SYSERR){
        for(i=0; i<10; i++){
            printf("\n Sending msg %d to receiver %d.", m[i], rec10);
        }
        printf("\n Number of message successfully sent is %d", k);
    }
    else printf("\n3 ERROR while sending to receiver %d", rec10);
	mutex_release(mx);
	return OK;
}

process sender11(void){
	uint32 k=0;
    uint32 i=0;
	umsg32 m[11] = {41,42,43,44,45,46,47,48,49,50,51};
	k=sendMsgs(rec11, m, 11);
	mutex_acquire(mx);
    if(k!=SYSERR){
        for(i=0; i<10; i++){
            printf("\n Sending msg %d to receiver %d.", m[i], rec11);
        }
        printf("\n Number of message successfully sent is %d", k);
    }
    else printf("\n3 ERROR while sending to receiver %d", rec11);
	mutex_release(mx);
	return OK;
}

process receiver9(void){
	int32 i=0;
    umsg32 msges9[10];
	receiveMsgs(msges9, 10);
    mutex_acquire(mx);
	for(i=0; i<10; i++){
        printf("\n Receiver %d received Message: %d", currpid, msges9[i]);
	}
    mutex_release(mx);
	return OK;
}

process receiver10(void){
	int32 i=0;
    umsg32 msges10[10];
	receiveMsgs(msges10, 10);
    mutex_acquire(mx);
	for(i=0; i<10; i++){
		printf("\n Receiver %d received Message: %d", currpid, msges10[i]);
	}
    mutex_release(mx);
	return OK;
}

process receiver11(void){
	int32 i=0;
    umsg32 msges11[10];
	receiveMsgs(msges11, 10);
    mutex_acquire(mx);
	for(i=0; i<10; i++){
		printf("\n Receiver %d received Message: %d", currpid, msges11[i]);
	}
    mutex_release(mx);
	return OK;
}

process	main(void)
{
	recvclr();
	mx = semcreate(1);

	/*Send one message to one receiver*/
	send1 = create(sender1, 4096, 50, "sender1", 0);
	rec1 = create(receiver1, 4096, 50, "receiver1", 0);

	/*Send 1 message to 3 receiver */
	send2 = create(sender2, 4096, 50, "sender2", 0);
	rec2 = create(receiver2, 4096, 50, "receiver2", 0);
	rec3 = create(receiver3, 4096, 50, "receiver3", 0);
	rec4 = create(receiver4, 4096, 50, "receiver4", 0);
    
	/*3 Senders will send 4 messages to 3 receivers */
	send3 = create(sender3, 4096, 50, "sender3", 0);
	send4 = create(sender4, 4096, 50, "sender4", 0);
	send5 = create(sender5, 4096, 50, "sender5", 0);
	rec5 = create(receiver5, 4096, 50, "receiver5", 0);
	rec6 = create(receiver6, 4096, 50, "receiver6", 0);
	rec7 = create(receiver7, 4096, 50, "receiver7", 0);

	/*3 Senders will send 11 messages to 1 receiver */
	send6 = create(sender6, 4096, 50, "sender6", 0);
	send7 = create(sender7, 4096, 50, "sender7", 0);
	send8 = create(sender8, 4096, 50, "sender8", 0);
	rec8 = create(receiver8, 4096, 50, "receiver8", 0);

	/*3 Senders will send 11 messages to 3 receivers */	
	send9 = create(sender9, 4096, 50, "sender10", 0);
	send10 = create(sender10, 4096, 50, "sender9", 0);
	send11 = create(sender11, 4096, 50, "sender11", 0);
	rec9 = create(receiver9, 4096, 50, "receiver9", 0);
	rec10 = create(receiver10, 4096, 50, "receiver10", 0);
	rec11 = create(receiver11, 4096, 50, "receiver11", 0);

	resched_cntl(DEFER_START);
    /*Send one message to one receiver*/
	resume(send1);
	resume(rec1);
    /*Send 1 message to 3 receiver */
	resume(send2);
	resume(rec2);
	resume(rec3);
	resume(rec4);
    /*3 Senders will send 4 messages to 3 receivers */
	resume(send3);
	resume(send4);
	resume(send5);
	resume(rec5);
	resume(rec6);
	resume(rec7);
    /*3 Senders will send 11 messages to 1 receiver */
	resume(send6);
	resume(send7);
	resume(send8);
	resume(rec8);
    /*3 Senders will send 11 messages to 3 receivers */
	resume(send9);
	resume(send10);
	resume(send11);
	resume(rec9);
	resume(rec10);
	resume(rec11);
	resched_cntl(DEFER_STOP);

	return OK;
}
