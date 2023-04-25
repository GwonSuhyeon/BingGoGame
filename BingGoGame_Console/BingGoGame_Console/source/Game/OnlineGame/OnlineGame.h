
#pragma once

#include "../../SocketConnector/SocketServer/Server.h"
#include "../../SocketConnector/SocketClient/Client.h"
#include "../CommonFunction.h"
#include "../../ExceptionHandling.h"
#include <iostream>
#include <cstring>
#include <iomanip>

#define PACKET_BUFFER_SIZE 512

class OnlineGameManager
{
public :
	OnlineGameManager();
	~OnlineGameManager();

	void run();

	int onlineGameMenu();

	bool serverPlayer();

	bool clientPlayer();

	void onlineBingGoPrint(int **, int);

	void onlineGameStart(int **, int);

private:

	int binggoSize = 0;
	int socketSize = 0;

	bool firstRun = true;
	bool threadState = true;

	char socketReceiveBuffer[PACKET_BUFFER_SIZE];
	char sendPacketMessage[PACKET_BUFFER_SIZE];

	ServerManager *server = nullptr;

	ClientManager *client = nullptr;

	SOCKET socket;

	ExceptionHandlingManager exception;

	CommonFunctionManager *commonFunction = nullptr;

	HANDLE threadHandle;

	unsigned threadID;
};