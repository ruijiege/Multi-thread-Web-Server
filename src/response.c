/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: response.c
*/
#include "main.h"
#include "response.h"
#include "options.h"
#include "network.h"
#include "threadpool.h"

// prepare and send response
void* sendResponse(void* num)
{
	pthread_mutex_lock(&mutexOfQueue);
	int rNumber = *(int*) num;
	printf("Ready thread: %d\n", rNumber + 1);
	pthread_cond_signal(&condVarOfQueue);
	pthread_mutex_unlock(&mutexOfQueue);

	while (1)
	{
		sem_wait(&wThreads[rNumber]);
		pthread_mutex_lock(&mutexOfQueue);

		char outputBuffer[BUFFERSIZE]; 
		char *fname;
		unsigned int fileHandler;
		unsigned int lengthOfBuffer;
		unsigned int fileDescriptor;
		char charBuffer[150] = { '\0' };
		time_t curTime;
		struct stat st;
		struct dirent **dir;
		int n, position;
		char * charptr;
		char ipAddString[150] = { '\0' };
		socklen_t socketLength;
		struct sockaddr_storage sckStorageAddr;
		FILE *filePtr = NULL;
		struct tm * requestTime, *requestAssignedTime;
		
		// thread scheduling started
		if ((readyQThreads + rNumber)->isSched == 0) 
		{ 
			fname = (readyQThreads + rNumber)->queueRequestptr->locationOfRequest;
			fileDescriptor = (readyQThreads + rNumber)->queueRequestptr->reqNumber;

			socketLength = sizeof sckStorageAddr;
			getpeername(fileDescriptor, (struct sockaddr*) &sckStorageAddr, &socketLength);
			struct sockaddr_in *a = (struct sockaddr_in *) &sckStorageAddr;
			inet_ntop(AF_INET, &a->sin_addr, ipAddString, sizeof ipAddString);

			if ((debugMode == 0) && (isLog == 1)) 
			{
				if (!(filePtr = fopen(logFilename, "a"))) 
				{
					printf("Unable to open log file");
					exit(1);
				}
				fputs(ipAddString, filePtr);
				fprintf(filePtr, " - ");
				// get the time in GMT
				requestTime = gmtime(&(readyQThreads + rNumber)->queueRequestptr->timeOfRequest); 
				fprintf(filePtr,"[%d/%d/%d:%d:%d:%d -0400]", requestTime->tm_mday, requestTime->tm_mon+1, requestTime->tm_year+1900, requestTime->tm_hour, requestTime->tm_min, requestTime->tm_sec);
				requestAssignedTime = gmtime(&(readyQThreads + rNumber)->queueRequestptr->timeOfExec); 
				fprintf(filePtr,"[%d/%d/%d:%d:%d:%d -0400]", requestAssignedTime->tm_mday, requestAssignedTime->tm_mon + 1, requestAssignedTime->tm_year + 1900, requestAssignedTime->tm_hour, requestAssignedTime->tm_min, requestAssignedTime->tm_sec);
			} 
			else 
			{
				printf("%s", ipAddString);
				printf(" - ");
				requestTime = gmtime(&(readyQThreads + rNumber)->queueRequestptr->timeOfRequest);
				printf("[%d/%d/%d:%d:%d:%d -0400]", requestTime->tm_mday, requestTime->tm_mon + 1, requestTime->tm_year + 1900, requestTime->tm_hour, requestTime->tm_min, requestTime->tm_sec);
				requestAssignedTime = gmtime(&(readyQThreads + rNumber)->queueRequestptr->timeOfExec);
				printf("[%d/%d/%d:%d:%d:%d -0400]", requestAssignedTime->tm_mday, requestAssignedTime->tm_mon + 1, requestAssignedTime->tm_year + 1900, requestAssignedTime->tm_hour, requestAssignedTime->tm_min, requestAssignedTime->tm_sec);
			}

			if (strstr(fname, ".") == NULL) 
			{
				n = scandir(fname, &dir, 0, alphasort);
				if (n < 0) 
				{
					strcpy(outputBuffer, NOT_OK_MSG);
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					strcpy(outputBuffer, ERR_MSG);
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					if ((debugMode == 0) && (isLog == 1)) 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							fprintf(filePtr, " \"GET /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 404 %d\n",(readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							fprintf(filePtr, " \"HEAD /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 404 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
						fclose(filePtr);
					} 
					else 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							printf(" \"GET /%s HTTP1.0\"", fname);
							printf(" 404 %d\n",(readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							printf(" \"HEAD /%s HTTP1.0\"", fname);
							printf(" 404 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
					}
				} 
				else 
				{
					strcpy(outputBuffer, OK_MSG);
					strcat(outputBuffer, "Content-Type:  text/html\n\n");
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					strcpy(outputBuffer, "<html>\n<body>\n");
					while (n--) 
					{
						charptr = strchr(dir[n]->d_name, '.');
						position = charptr - dir[n]->d_name + 1;
						if (position != 1) 
						{
							strcat(outputBuffer, "<p>");
							strcat(outputBuffer, dir[n]->d_name);
							strcat(outputBuffer, "</p>\n");
							free(dir[n]);
						}
					}
					free(dir);
					strcat(outputBuffer, "</body>\n</html>\n");
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);

					if ((debugMode == 0) && (isLog == 1)) 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							fprintf(filePtr, " \"GET /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							fprintf(filePtr, " \"HEAD /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
						fclose(filePtr);
					} 
					else 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							printf(" \"GET /%s HTTP1.0\"", fname);
							printf(" 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							printf(" \"HEAD /%s HTTP1.0\"", fname);
							printf(" 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
					}
				}
				close(fileDescriptor);
			} 
			else 
			{
				fileHandler = open(&fname[0], O_RDONLY);
				if (fileHandler == -1) {
					printf("File not found %s \n", &fname[0]);
					strcpy(outputBuffer, NOT_OK_MSG);
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					time(&curTime);
					strcpy(outputBuffer, "Date:");
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					strcpy(outputBuffer, ctime(&curTime));
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					strcpy(outputBuffer,"Server: myhttpd Version 1.0\n\n");
					send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					
					if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0)
					{
						strcpy(outputBuffer, ERR_MSG);
						send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					}
					if ((debugMode == 0) && (isLog == 1)) 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							fprintf(filePtr, " \"GET /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 404 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							fprintf(filePtr, " \"HEAD /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 404 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
						fclose(filePtr);
					}
					else 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							printf(" \"GET /%s HTTP1.0\"", fname);
							printf(" 404 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							printf(" \"HEAD /%s HTTP1.0\"", fname);
							printf(" 404 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
					}
					close(fileDescriptor);
				} 
				else 
				{
					if ((strstr(fname, ".jpg") != NULL) || (strstr(fname, ".gif") != NULL)) 
					{
						strcpy(outputBuffer, OK_MSG);
						time(&curTime);
						stat(&fname[0], &st);
						strcat(outputBuffer, "Date:          ");
						strcat(outputBuffer, ctime(&curTime));
						strcat(outputBuffer, "Server: myhttpd Version 1.0\n");
						strcat(outputBuffer, "Last-Modified: ");
						strcat(outputBuffer, ctime(&st.st_mtime));
						strcat(outputBuffer, "Content-Type:  image/gif\n");
						strcat(outputBuffer, "Content-len:");
						sprintf(charBuffer, "%d",(readyQThreads + rNumber)->queueRequestptr->len);
						strcat(outputBuffer, charBuffer);
						strcat(outputBuffer, "\n\n");
						send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					} 
					else 
					{
						strcpy(outputBuffer, OK_MSG);
						time(&curTime);
						stat(&fname[0], &st);
						strcat(outputBuffer, "Date:          ");
						strcat(outputBuffer, ctime(&curTime));
						strcat(outputBuffer,"Server: myhttpd Version 1.0\n");
						strcat(outputBuffer, "Last-Modified: ");
						strcat(outputBuffer, ctime(&st.st_mtime));
						strcat(outputBuffer, "Content-Type:  text/html\n");
						strcat(outputBuffer, "Content-len:");
						sprintf(charBuffer, "%d",(readyQThreads + rNumber)->queueRequestptr->len);
						strcat(outputBuffer, charBuffer);
						strcat(outputBuffer, "\n\n");
						send(fileDescriptor, outputBuffer, strlen(outputBuffer), 0);
					}
					if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
					{
						lengthOfBuffer = 1;
						while (lengthOfBuffer > 0) 
						{
							lengthOfBuffer = read(fileHandler, outputBuffer, BUFFERSIZE);
							if (lengthOfBuffer > 0) 
							{
								send(fileDescriptor, outputBuffer, lengthOfBuffer, 0);
							}
						}
					}
					close(fileHandler);
					
					if ((debugMode== 0) && (isLog == 1)) 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							fprintf(filePtr, " \"GET /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							fprintf(filePtr, " \"HEAD /%s HTTP1.0\"", fname);
							fprintf(filePtr, " 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
						fclose(filePtr);
					} 
					else 
					{
						if ((readyQThreads + rNumber)->queueRequestptr->typeOfRequest == 0) 
						{
							printf(" \"GET /%s HTTP1.0\"", fname);
							printf(" 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						} 
						else 
						{
							printf(" \"HEAD /%s HTTP1.0\"", fname);
							printf(" 200 %d\n", (readyQThreads + rNumber)->queueRequestptr->len);
						}
					}
					close(fileDescriptor);
				}
			}
			(readyQThreads + rNumber)->isSched = 1;
			sem_post(&waitingThreading);
			pthread_mutex_unlock(&mutexOfQueue);
		}
	}
}



//Parse the request
struct queueRequest* parseRequest(char* cptr) 
{
	struct queueRequest* userReq = (struct queueRequest*) malloc(sizeof(struct queueRequest));
	char *fname, *typeOfRequest;
	struct stat st;
	int l, l1, l2, l3, fileHandler;

	typeOfRequest = strtok(cptr, " ");
	if (strcmp(typeOfRequest, "GET") == 0) 
	{
		userReq->typeOfRequest = 0;
	} 
	else 
	{
		userReq->typeOfRequest = 1;
	}

	fname = strtok(NULL, " ");

	if (strstr(fname, "~") != NULL) 
	{
		if (strstr(fname, ".") != NULL) 
		{
			l1 = strlen(&fname[2]);
			l2 = strlen(directoryName);
			userReq->locationOfRequest = malloc(l1 + l2 + 2);
			strcpy(userReq->locationOfRequest, directoryName);
			strcat(userReq->locationOfRequest, "/");
			strcat(userReq->locationOfRequest, &fname[2]);
			userReq->len = 0;
			return userReq;
		} 
		else 
		{
			l1 = strlen("index.html");
			l3 = strlen(&fname[2]);
			l2 = strlen(directoryName);
			userReq->locationOfRequest = malloc(l1 + l2 + l3 + 3);
			strcpy(userReq->locationOfRequest, directoryName);
			strcat(userReq->locationOfRequest, "/");
			strcat(userReq->locationOfRequest, &fname[2]);
			strcat(userReq->locationOfRequest, "/");
			strcat(userReq->locationOfRequest, "index.html");
			fileHandler = open(userReq->locationOfRequest, O_RDONLY);
			if (fileHandler != -1) 
			{
				stat(userReq->locationOfRequest, &st);
				userReq->len = st.st_size;
				return userReq;
			} 
			else 
			{
				l1 = strlen(&fname[2]);
				l2 = strlen(directoryName);
				userReq->locationOfRequest = malloc(l1 + l2 + 2);
				strcpy(userReq->locationOfRequest, directoryName);
				strcat(userReq->locationOfRequest, "/");
				strcat(userReq->locationOfRequest, &fname[2]);
				userReq->len = 0;
				return userReq;
			}
		}
	} 
	else 
	{
		if (strstr(fname, ".") != NULL) 
		{
			l = strlen(&fname[1]);
			userReq->locationOfRequest = malloc(l + 1);
			strcpy(userReq->locationOfRequest, &fname[1]);
			stat(&fname[1], &st);
			userReq->len = st.st_size;
			return userReq;
		} 
		else 
		{
			l1 = strlen("index.html");
			l3 = strlen(&fname[1]);
			l2 = strlen(directoryName);
			userReq->locationOfRequest = malloc(l1 + l2 + l3 + 3);
			strcpy(userReq->locationOfRequest, directoryName);
			strcat(userReq->locationOfRequest, "/");
			strcat(userReq->locationOfRequest, &fname[1]);
			strcat(userReq->locationOfRequest, "/");
			strcat(userReq->locationOfRequest, "index.html");
			fileHandler = open(userReq->locationOfRequest, O_RDONLY);
			if (fileHandler != -1) 
			{
				stat(userReq->locationOfRequest, &st);
				userReq->len = st.st_size;
				return userReq;
			} 
			else 
			{
				l = strlen(&fname[1]);
				l2 = strlen(directoryName);
				userReq->locationOfRequest = malloc(l + l2 + 2);
				strcpy(userReq->locationOfRequest, directoryName);
				strcat(userReq->locationOfRequest, "/");
				strcpy(userReq->locationOfRequest, &fname[1]);
				userReq->len = 0;
				return userReq;
			}
		}
	}
}


