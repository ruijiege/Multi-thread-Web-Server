/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: options.h
*/
#ifndef _OPTIONS_H_
#define _OPTIONS_H_

extern int debugMode;
extern int portNumber;
extern int numberOfThreads;
extern int schedAlgo;
extern int timeOfWait;
extern char *logFilename;
extern char *directoryName;
extern int isLog;
char dircLocation[100];

void getOptions(int, char **);

#endif
