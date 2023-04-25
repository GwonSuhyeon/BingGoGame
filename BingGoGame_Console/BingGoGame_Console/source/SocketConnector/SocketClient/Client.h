
#pragma warning(disable : 4996)

#pragma once

#pragma comment(lib, "ws2_32")

#include "../../ExceptionHandling.h"
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <process.h>

#define SERVER_SOCKET_PORT 10800

class ClientManager
{
public:
	ClientManager();
	~ClientManager();

	static int toServerPort;

	static std::string toServerIP;
	
	SOCKET clientSocket;
	static SOCKET mediationSocket;

	SOCKADDR_IN clientSocketAddr;

	static SOCKADDR_IN toServerAddr;

	bool clientSocketCreator();

	void clientSocketCloser();

	bool mediationSocketCreator();

	void mediationSocketCloser();

	static unsigned int __stdcall threadHandle(int*);

	static unsigned int __stdcall pingThread(bool*);

	static unsigned int __stdcall toServerThread(bool*);

private :

	int mediationSocketSize;
	int packetSize;

	char hostName[255];
	char mediationBuffer[512];

	bool pingThreadState = true;
	bool toServerThreadState = true;

	PHOSTENT mediationHost;

	WSADATA wsadata;

	SOCKADDR_IN mediationSocketAddr;
	SOCKADDR_IN udpSocketAddr;

	ExceptionHandlingManager exception;

	HANDLE tHandle;
	HANDLE pingThreadHandle;
	HANDLE toServerHandle;

	unsigned threadID;
	unsigned pingThreadID;
	unsigned toServerThreadID;
};
