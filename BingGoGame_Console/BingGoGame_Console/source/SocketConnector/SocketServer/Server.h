
#pragma once

#pragma comment(lib, "ws2_32")

#include "../../ExceptionHandling.h"
#include <iostream>
#include <WinSock2.h>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <process.h>
#include <time.h>

#include <mysql.h>
//#include <my_global.h>

#pragma comment(lib, "libmysql.lib")

#define SOCKET_PORT 10800

class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	static int toClientPort;

	static std::string toClientIP;

	SOCKET serverSocket;
	SOCKET clientSocket;
	static SOCKET mediationSocket;

	static SOCKADDR_IN toClientAddr;

	void mysqlConnect();

	void serverSocketCreator();

	void serverSocketCloser();

	bool mediationSocketCreator();

	void mediationSocketCloser();

	static unsigned int __stdcall threadHandle(int *);
	static unsigned int __stdcall pingThread(bool *);
	static unsigned int __stdcall toClientThread(bool*);

private:

	int mediationSocketSize;
	int packetSize;
	int socketPort;

	char *ipAddress;
	char hostName[255];
	char mediationBuffer[512];

	bool pingThreadState = true;
	bool toClientThreadState = true;

	WSADATA wsadata;

	SOCKADDR_IN clientSocketAddr;
	SOCKADDR_IN serverSocketAddr;
	SOCKADDR_IN mediationSocketAddr;
	SOCKADDR_IN udpSocketAddr;

	PHOSTENT host;
	PHOSTENT mediationHost;

	ExceptionHandlingManager exception;

	HANDLE tHandle;
	HANDLE pingThreadHandle;
	HANDLE toClientHandle;

	unsigned threadID;
	unsigned pingThreadID;
	unsigned toClientThreadID;
};

