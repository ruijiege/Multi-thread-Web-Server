/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: main.c
*/
#include "main.h"
#include "options.h"
#include "network.h"
#include "response.h"
#include "threadpool.h"

//declaration of default variables, if user fails to provide the parameters
int debugMode = 0;
int portNumber = 8080;
int numberOfThreads = 4;
int schedAlgo = FCFS;
int timeOfWait = 60;
char *logFilename;
char *directoryName;
int isLog = 0;

// main program begins here
int main(int argc, char **argv) 
{
	printf("Starting server...\n");

	// getOption is function that reads user arguments and assign values to various parameters like port, scheduling algo, etc
	getOptions(argc, argv);

	// debug mode
	if (debugMode == 0)
	{
		pid_t processID;
		if ((processID = fork()) < 0)
			return (-1);
		else if (processID != 0)
			exit(0);
		setsid();
		chdir(directoryName);
		umask(0);
	}

	// function for establish and bind the socket
	network();

	// Intialize the request parameters 
	initializeRequestQueue();

	// assign the memory to struct's of ready queue 
	mallocThreadpool();

	//initialize the mutex variables
	initializeMutex();

	// creates the threadpool
	createThreadpool();

	// intialize scheduling thread
	initializeSchedulingThread();

	printf("Welcome!! myhttps server is started!!!\n");
	// buffer for storage of input request
	char inputRequestBuffer[BUFFERSIZE];
	time_t tm;

	//here server is keep accepting to user's requests
	while (1)
	{
		lengthOfAddress = sizeof(addressOfClient);
		clientSocketDescr = accept(serverSocketDescr, (struct sockaddr *) &addressOfClient, &lengthOfAddress);
		if (clientSocketDescr == 0) 
		{
			printf("failed to create the socket..\n");
			exit(0);
		}
		else 
		{
			int j;		
			printf("New request from client..\n");
			recv(clientSocketDescr, inputRequestBuffer, BUFFERSIZE, 0);
			//lock using mutex 		
			pthread_mutex_lock(&mutexOfReadyQ);
					
			for (j = 0; j < REQ; j++) 
			{
				if (queueOfRequests[j]->reqNumber == 0) 
				{
					queueOfRequests[j] = parseRequest(inputRequestBuffer);
					queueOfRequests[j]->reqNumber = clientSocketDescr;
					time(&tm);
					queueOfRequests[j]->timeOfRequest = tm;
					sem_post(&cnt);
					break;
				}
			}
			//unlock the queue
			pthread_mutex_unlock(&mutexOfReadyQ);
		}
	}
}
