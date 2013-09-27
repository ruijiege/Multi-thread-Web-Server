/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: main.h
*/
// consists of all the declaration of variables, functions and data structure

#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>          
#include <stdlib.h>         
#include <string.h>         
#include <fcntl.h>          
#include <sys/stat.h>       
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>     
#include <arpa/inet.h>      
#include <pthread.h>      
#include <semaphore.h>

#define OK_MSG    "HTTP/1.0 200 OK\n"
#define NOT_OK_MSG   "HTTP/1.0 404 Not Found\n"
#define ERR_MSG    "<html><body><h1>FILE NOT FOUND</h1></body></html>\n"
#define FCFS                   0
#define SJF                    1
#define BUFFERSIZE            1024     
#define REQ		      50 

int debugMode;
int portNumber;
int numberOfThreads;
int schedAlgo;
int timeOfWait;
char *logFilename;
char *directoryName;
int isLog;
extern char dircLocation[100];

#endif
