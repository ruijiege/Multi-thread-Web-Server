/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: threadpool.c
*/
#include "main.h"
#include "response.h"
#include "options.h"
#include "network.h"
#include "threadpool.h"

// initialization of request queue
void initializeRequestQueue()
{
	int i;
	for (i = 0; i < REQ; i++)
    	{
		queueOfRequests[i] = (struct queueRequest*) malloc(sizeof(struct queueRequest));
	}
}

// assign the memory
void mallocThreadpool()
{
	readyQThreads = (struct readyQueueRequest*) malloc(numberOfThreads * sizeof(struct readyQueueRequest));
	wThreads = (sem_t *) malloc(numberOfThreads * sizeof(sem_t));
}

// intialization of mutex and semaphores
void initializeMutex()
{
	int k;	
	pthread_mutex_init(&mutexOfQueue, NULL);
	pthread_cond_init(&condVarOfQueue, NULL);
	pthread_mutex_init(&mutexOfReadyQ, NULL);
	sem_init(&cnt, 0, 0);
	sem_init(&waitingThreading, 0, numberOfThreads);
	
	for (k = 0; k < numberOfThreads; k++)
	{
		sem_init(&wThreads[k], 0, 0);
	}
}

// create a threadpool of n number of threads
void createThreadpool()
{
	int j = 0;
	for (j = 0; j < numberOfThreads; j++)
	{
		pthread_mutex_lock(&mutexOfQueue);
		pthread_create(&((readyQThreads + j)->thread), NULL, sendResponse, &j);
		(readyQThreads + j)->isSched = 1;
		pthread_cond_wait(&condVarOfQueue, &mutexOfQueue);
		pthread_mutex_unlock(&mutexOfQueue);
	}
}

// initialization of scheduling thread
void initializeSchedulingThread()
{
	pthread_t schthreads;
	if (schedAlgo == 0)
	{
		pthread_create(&schthreads, NULL, scheduleRequestUsingFCFS, NULL);
	} 
	else
	{
		pthread_create(&schthreads, NULL, scheduleRequestUsingSJF, NULL);
	}
}

// Scheduling using FCFS algorithm
void* scheduleRequestUsingFCFS(void* ptr) 
{
	sleep(timeOfWait);
	while (1) 
	{
		int i, j;
		sem_wait(&cnt);
		// lock to fetch the request from ready queue
		pthread_mutex_lock(&mutexOfReadyQ);
		struct queueRequest* userRequest = queueOfRequests[0];
		
		for (j = 0; j < REQ; j++) 
		{
			queueOfRequests[j] = queueOfRequests[j + 1];
			if (queueOfRequests[j + 1]->reqNumber == 0) 
			{
				break;
			}
		}
		// unlock the ready queue
		pthread_mutex_unlock(&mutexOfReadyQ); 
		// request is assigned to particular thread		
		sem_wait(&waitingThreading);
		pthread_mutex_lock(&mutexOfQueue);
		
		time_t assignedTime;
		for (i = 0; i < numberOfThreads; i++) 
		{
			if ((readyQThreads + i)->isSched == 1) 
			{
				(readyQThreads + i)->queueRequestptr = userRequest;
				(readyQThreads + i)->isSched = 0;
				printf("file %sl is assigned to thread %d\n", (readyQThreads + i)->queueRequestptr->locationOfRequest, i + 1);
				time(&assignedTime);
				(readyQThreads + i)->queueRequestptr->timeOfExec = assignedTime;
				sem_post(&wThreads[i]);
				break;
			}
		}
		pthread_mutex_unlock(&mutexOfQueue);
	}
}

// Scheduling using SJF algorithm
void* scheduleRequestUsingSJF(void* ptr) 
{
	sleep(timeOfWait);
	while (1) 
	{
		sem_wait(&cnt);
		// lock and fetch the request from the queue
		pthread_mutex_lock(&mutexOfReadyQ); 
		struct queueRequest* userRequest = queueOfRequests[0];
		int i, j = 0;
		for (i = 1; i < REQ; i++) 
		{
			if (queueOfRequests[i]->reqNumber == 0)
				break;
			if ((queueOfRequests[i]->len) < (userRequest->len)) 
			{
				userRequest = queueOfRequests[i];
				j = i;
			}
		}
		// unlock
		pthread_mutex_unlock(&mutexOfReadyQ);

		// assign the request to some thread
		sem_wait(&waitingThreading);
		pthread_mutex_lock(&mutexOfQueue);
		time_t assignedTime;		
		int a;
		for (a = 0; a < numberOfThreads; a++) 
		{
			if ((readyQThreads + a)->isSched == 1) 
			{
				(readyQThreads + a)->queueRequestptr = userRequest;
				(readyQThreads + a)->isSched = 0;
				printf("file %s is assigned to thread %d\n", (readyQThreads + a)->queueRequestptr->locationOfRequest, a + 1);
				time(&assignedTime);
				(readyQThreads + a)->queueRequestptr->timeOfExec = assignedTime;
				sem_post(&wThreads[a]);
				break;
			}
		}
		pthread_mutex_unlock(&mutexOfQueue);

		pthread_mutex_lock(&mutexOfReadyQ);
		int m;
		for (m = j; m < REQ; m++) 
		{
			queueOfRequests[m] = queueOfRequests[m + 1];
			if (queueOfRequests[m + 1]->reqNumber == 0) 
			{
				break;
			}
		}
		pthread_mutex_unlock(&mutexOfReadyQ);
	}
}
