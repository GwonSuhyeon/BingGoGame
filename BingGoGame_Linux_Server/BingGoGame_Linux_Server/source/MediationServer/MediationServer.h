
#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <netdb.h>
//#include <netinet/in.h>

#define SOCKET_PORT 10800
#define PACKET_BUFFER_SIZE 512

class MediationServerManager
{
public:
	MediationServerManager();
	~MediationServerManager();

	void socketCreator();

	void socketCloser();

	static void* threadHandle(void *);

private:

	int serverSocket;
	int packetSize;
	int threadCreate;
	// int threadState;

	socklen_t fromClientSize;

	char packet[PACKET_BUFFER_SIZE];

	struct sockaddr_in serverAddr;
	struct sockaddr_in fromClientAddr;

	pthread_t threadID;

	void* threadReturn = nullptr;
};