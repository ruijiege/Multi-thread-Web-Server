/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: threadpool.h
*/
#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

extern int debugMode;
extern int portNumber;
extern int numberOfThreads;
extern int schedAlgo;
extern int timeOfWait;
extern char *logFilename;
extern char *directoryName;
extern int isLog;


pthread_cond_t condVarOfQueue; //for initialization threads
pthread_mutex_t mutexOfQueue;
pthread_mutex_t mutexOfReadyQ; //for request ready queue
sem_t cnt, waitingThreading, *wThreads;

// Queuing thread struct 
struct queueRequest
{
	int reqNumber;
	int typeOfRequest;
	char* locationOfRequest;
	int len;
	time_t timeOfRequest;
	time_t timeOfExec;

};

// Working thread struct 
struct readyQueueRequest
{
	pthread_t thread;
	struct queueRequest* queueRequestptr;
	int isSched;
};

struct readyQueueRequest* readyQThreads;
struct queueRequest *queueOfRequests[REQ];

void initializeRequestQueue();
void mallocThreadpool();
void initializeMutex();
void createThreadpool();
void initializeSchedulingThread();
void* scheduleRequestUsingFCFS(void*);
void* scheduleRequestUsingSJF(void*);
void* sendResponse(void*);
struct queueRequest* parseRequest(char*);


#endif
