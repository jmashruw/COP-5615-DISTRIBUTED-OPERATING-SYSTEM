initialize.c - to allocate memory for all the global variables needed to implement system calls sendMsg, sendMsgs, sendnMsgs, receiveMsg, receiveMsgs and main.c

main.c - contains implementation to test and execute extended message passing system calls sendMsg, sendMsgs, sendnMsgs, receiveMsg, receiveMsgs. Have used counting semaphores and mutex to control synchronization and access to I/O respectively

output.txt - contains sample output on executing main.c

prototypes.h - adds new system calls (sendMsg, sendMsgs, sendnMsgs, receiveMsg, receiveMsgs) in Xinu

send.c  - contains implementation of system calls sendMsg, sendMsgs, sendnMsgs

receive.c - contains implementation of system calls receiveMsg, receiveMsgs
