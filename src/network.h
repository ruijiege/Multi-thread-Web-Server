/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: network.h
*/
#ifndef _NETWORK_H_
#define _NETWORK_H_

extern int debugMode;
extern int portNumber;
extern int numberOfThreads;
extern int schedAlgo;
extern int timeOfWait;
extern char *logFilename;
extern char *directoryName;
extern int isLog;

// variables for socket connections
unsigned int serverSocketDescr;
struct sockaddr_in address;
unsigned int clientSocketDescr;
struct sockaddr_in addressOfClient;
socklen_t lengthOfAddress;

void network();

#endif
