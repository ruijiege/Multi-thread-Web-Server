/*
Team Number G11
CSE 521 Operatin Systems : Project 1
Multi-threaded web server
File: network.c
*/
#include "main.h"
#include "response.h"
#include "options.h"
#include "network.h"
#include "threadpool.h"

// usage: create a socket, bind and listen to request
void network()
{
	// create a socket
	serverSocketDescr = socket(AF_INET, SOCK_STREAM, 0);

	//fill port, address details and bind
	address.sin_family = AF_INET;
	address.sin_port = htons(portNumber);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(serverSocketDescr, (struct sockaddr *) &address, sizeof(address));

	//listen to requests
	listen(serverSocketDescr, REQ);
}

