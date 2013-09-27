/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: options.c
*/
#include "main.h"
#include "response.h"
#include "options.h"
#include "network.h"
#include "threadpool.h"

//usage: used to read the parameters from the user input and assign values to variables in order to process it further
void getOptions(int argc, char **argv)
{
	int choice;
	opterr = 0;
		while ((choice = getopt(argc, argv, "l:p:r:t:n:s:dh")) != -1) 
		{
			switch (choice) 
			{
				case 'd':
					debugMode = 1;
					break;
				case 'h':
					printf("-d\t:Enter debugging mode. That is, do not daemonize, only accept one connection at a time and 						enable logging to stdout.\n");
					printf("-h\t:Print a usage summary with all options and exit.\n");
					printf("-l file\t:Log all requests to the given file.\n");
					printf("-p port\t:Listen on the given port. If not provided, myhttpd will listen on port 8080.\n");
					printf("-r dir\t:Set the root directory for the http server to dir\n");
					printf("-t time\t:Set the queuing time to time seconds. The default should be 60 seconds\n");
					printf("-n threadnum\t:Set number of threads waiting ready in the execution thread pool to threadnum. 						The default should be 4 execution threads.\n");
					printf("-s sched\t:Set the scheduling policy. It can be either FCFS or SJF. The default will be 						FCFS.\n");
					exit(1);
					break;
				
				case 'l':
					logFilename = (char *) malloc(sizeof optarg);
					strcpy(logFilename, optarg);
					isLog = 1;
					break;
				case 'p':
					portNumber = atoi(optarg);
					break;
				case 'r':
					directoryName = (char *) malloc(sizeof optarg);
					strcpy(directoryName, optarg);
					break;
				case 't':
					timeOfWait = atoi(optarg);
					break;
				case 'n':
					numberOfThreads = atoi(optarg);
					break;
				case 's':
					schedAlgo = optarg;
					break;
				default:
					printf("Please enter correct options...");
			}
		}
	// printing the choice of user for scheduling policy, queuing time, directory, log file name, etc
	if (debugMode == 1)
	{
		printf("Mode: Debugging mode\n");
		printf("Port: %d\n", portNumber);
		if (directoryName != NULL)
		{
			printf("Directory: %s\n", directoryName);
		}
        	else
		{
			getcwd(dircLocation, sizeof(dircLocation));
			directoryName = (char *) malloc(sizeof dircLocation);
			strcpy(directoryName, dircLocation);
			printf("Directory: %s\n", directoryName);
		}
		printf("Queueing time: %d seconds\n", timeOfWait);
		numberOfThreads = 1;
	}
    	else
    	{
		if (isLog == 1) 
		{
			printf("Log filename: %s\n", logFilename);
		}
		printf("Port: %d\n", portNumber);
		if (directoryName != NULL) 
		{
			printf("Directory: %s\n", directoryName);
		} 
		else 
		{
			getcwd(dircLocation, sizeof(dircLocation));
			directoryName = (char *) malloc(sizeof dircLocation);
			strcpy(directoryName, dircLocation);
			printf("Directory: %s\n", directoryName);
		}
		printf("Queueing time:%d seconds\n", timeOfWait);
		printf("Number of execution threads: %d\n", numberOfThreads);
		if (schedAlgo == 0) 
		{
			printf("Scheduling policy: FCFS\n");
		} 
		else 
		{
			printf("Scheduling policy: SJF\n");
		}
	}
}
