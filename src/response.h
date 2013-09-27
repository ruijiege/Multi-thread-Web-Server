/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: response.h
*/
#ifndef _RESPONSE_H_
#define _RESPONSE_H_

extern int debugMode;
extern int portNumber;
extern int numberOfThreads;
extern int schedAlgo;
extern int timeOfWait;
extern char *logFilename;
extern char *directoryName;
extern int isLog;

void* sendResponse(void*);
struct queueRequest* parseRequest(char*);

#endif
