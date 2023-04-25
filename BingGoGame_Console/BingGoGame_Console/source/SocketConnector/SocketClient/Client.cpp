
#include "Client.h"

using namespace std;

SOCKET ClientManager::mediationSocket = NULL;

int ClientManager::toServerPort = 0;

string ClientManager::toServerIP = "";

SOCKADDR_IN ClientManager::toServerAddr = { 0 };

ClientManager::ClientManager()
{
	memset(&clientSocket, 0, sizeof(clientSocket));
	memset(&mediationSocket, 0, sizeof(mediationSocket));
	memset(&clientSocketAddr, 0, sizeof(clientSocketAddr));
	memset(&wsadata, 0, sizeof(wsadata));

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) // Window Socket 초기화
	{
		cout << "WindowSocket initialization fail : clientSocketCreator" << endl;
		exception.handle();
	}
}

ClientManager::~ClientManager()
{
	clientSocketCloser();
}

bool ClientManager::clientSocketCreator()
{
	int serverPort = -1;

	char recvBuffer[512];

	string serverIP;
	string recvBufferString;

	if (pingThreadHandle == NULL || pingThreadState == false)
	{
		cout << "Failed to execute pingThreadHandle : serverSocketCreator" << endl;
		exception.handle();
	}

	while (1)
	{
		cin.ignore();

		cout << "접속할 서버의 IP를 입력하세요.(IPv4) : ";

		getline(cin, toServerIP);

		if (toServerIP.empty() == false)
			break;
	}

	while (1)
	{
		cout << "접속할 서버의 포트를 입력하세요. : ";

		cin >> toServerPort;

		if (toServerPort != -1)
			break;
	}

	/*
	clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // Socket 생성

	if (clientSocket == -1)
	{
		cout << "Socket generate fail : clientSocketCreator" << endl;
		exception.handle();
	}
	*/

	toServerHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)toServerThread, &toServerThreadState, 0, &toServerThreadID);

	pingThreadState = false;

	// Client(주소 정보) 생성 시작
	memset(&clientSocketAddr, 0, sizeof(clientSocketAddr));

	clientSocketAddr.sin_family = AF_INET;
	// inet_addr() : IP주소를 문자열로 입력 받은 후 32비트 숫자로 리턴(네트워크 바이트 저장 방식 따름)
	clientSocketAddr.sin_addr.s_addr = inet_addr(toServerIP.c_str());
	clientSocketAddr.sin_port = htons(toServerPort);
	// Client(주소 정보) 생성 종료

	/*
	cout << endl << "서버 접속 요청 승인 대기 중..." << endl << endl;

	if (connect(clientSocket, (SOCKADDR *)&clientSocketAddr, sizeof(clientSocketAddr)) == -1)
	{
		//cout << "clientSocket connect fail : clientSocketCreator" << endl;
		cout << "서버 접속 실패!" << endl << endl;

		pingThreadState = false;

		return false;
	}
	else
	{
		_sleep(500);

		cout << "서버 접속 성공!" << endl << endl;

		pingThreadState = false;

		return true;
	}
	*/

	cout << endl << "서버 접속 요청 승인 대기 중..." << endl << endl;

	while (1)
	{
		// recv(client->clientSocket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
		recvfrom(mediationSocket, recvBuffer, 512, 0, NULL, NULL);

		recvBufferString = recvBuffer;

		if (recvBufferString.compare("CONNECT") == 0)
		{
			// sendto ACCEPT
			strcpy(recvBuffer, "ACCEPT");

			sendto(mediationSocket, recvBuffer, 512, 0, (SOCKADDR *)&clientSocketAddr, mediationSocketSize);

			break;
		}
	}

	_sleep(1000);

	strcpy(recvBuffer, "CONNECT");

	sendto(mediationSocket, recvBuffer, 512, 0, (SOCKADDR *)&clientSocketAddr, mediationSocketSize);

	while (1)
	{
		recvfrom(mediationSocket, recvBuffer, 512, 0, NULL, NULL);

		recvBufferString = recvBuffer;

		if (recvBufferString.compare("ACCEPT") == 0)
		{
			toServerThreadState = false;

			cout << "서버 접속 성공!" << endl << endl;

			break;
		}
	}

	return true;
}

void ClientManager::clientSocketCloser()
{
	if (clientSocket != -1)
		closesocket(clientSocket);

	WSACleanup();

	return;
}

bool ClientManager::mediationSocketCreator()
{
	mediationSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // Socket 생성

	if (mediationSocket == -1) // -1 대신 INVALID_SOCKET 사용 가능
	{
		cout << "Socket generate fail : mediationSocketCreator" << endl;

		return false;
	}
	
	memset(&udpSocketAddr, 0, sizeof(udpSocketAddr));
	
	udpSocketAddr.sin_family = AF_INET;
	udpSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	udpSocketAddr.sin_port = htons(10801);

	if (bind(mediationSocket, (SOCKADDR*)&udpSocketAddr, sizeof(udpSocketAddr)) == -1)
	{
		cout << "serverSocket bind fail : mediationSocketCreator" << endl;

		return false;
	}
	
	// Server(주소 정보) 생성 시작
	memset(&mediationSocketAddr, 0, sizeof(mediationSocketAddr));

	mediationHost = gethostbyname("gwonssuuu.hopto.org");

	mediationSocketAddr.sin_family = AF_INET;
	mediationSocketAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*mediationHost->h_addr_list));
	mediationSocketAddr.sin_port = htons(1609);

	mediationSocketSize = sizeof(mediationSocketAddr);

	memset(mediationBuffer, 0, sizeof(mediationBuffer));

	strcpy(mediationBuffer, "REQUEST");

	packetSize = sendto(mediationSocket, mediationBuffer, 512, 0, (struct sockaddr*)&mediationSocketAddr, mediationSocketSize);

	pingThreadHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)pingThread, &pingThreadState, 0, &pingThreadID);

	if (packetSize != 512)
	{
		cout << "Send packet fail : mediationSocketCreator" << endl;

		mediationSocketCloser();

		return false;
	}

	packetSize = 0;

	tHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadHandle, &packetSize, 0, &threadID);

	mediationSocketAddr = {};

	mediationSocketSize = sizeof(mediationSocketAddr);

	_sleep(500);

	packetSize = recvfrom(mediationSocket, mediationBuffer, 512, 0, (struct sockaddr*)&mediationSocketAddr, &mediationSocketSize);

	if (pingThreadState == false)
	{
		cout << "Socket fail of pingThread : mediationSocketCreator" << endl;

		exception.handle();
	}

	//WaitForSingleObject(tHandle, 5000);

	cout << "IP Address : " << mediationBuffer << endl;

	_sleep(500);

	// mediationSocketCloser();

	return true;
}

void ClientManager::mediationSocketCloser()
{
	if (mediationSocket != -1)
		closesocket(mediationSocket);

	return;
}

unsigned int __stdcall ClientManager::threadHandle(int* packet)
{
	double runningTime;

	clock_t start;
	clock_t end;

	start = clock();
	end = clock();

	runningTime = (double)end - (double)start;

	while (runningTime <= 5000)
	{
		if (*packet == 0)
		{
			end = clock();

			runningTime = (double)end - (double)start;
		}
		else
			return 0;
	}

	cout << endl << "Server is not responding" << endl;

	_sleep(5000);

	exit(0);
}

unsigned int __stdcall ClientManager::pingThread(bool* state)
{
	int pingSocketSize;

	double pingTime;

	char buffer[512] = "PING";

	clock_t start;
	clock_t end;

	// SOCKET pingSocket;

	SOCKADDR_IN pingSocketAddr;

	PHOSTENT pingHost;

	// pingSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (mediationSocket == -1)
	{
		cout << "Socket generate fail : pingThread" << endl;

		*state = false;

		return 0;
	}

	memset(&pingSocketAddr, 0, sizeof(mediationSocketAddr));

	pingHost = gethostbyname("gwonssuuu.hopto.org");

	pingSocketAddr.sin_family = AF_INET;
	pingSocketAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*pingHost->h_addr_list));
	pingSocketAddr.sin_port = htons(1609);

	pingSocketSize = sizeof(mediationSocketAddr);

	start = clock();
	end = clock();

	pingTime = (double)end - (double)start;

	while (*state == true)
	{
		if (pingTime >= 1000)
		{
			if (sendto(mediationSocket, buffer, 512, 0, (struct sockaddr*)&pingSocketAddr, pingSocketSize) != 512)
			{
				cout << "Send packet fail : pingThread" << endl;

				if (mediationSocket != -1)
					closesocket(mediationSocket);

				*state = false;

				return 0;
			}

			pingTime = 0;

			start = clock();
		}

		end = clock();

		pingTime = (double)end - (double)start;
	}

	return 0;
}

unsigned int __stdcall ClientManager::toServerThread(bool* state)
{
	int addrSize;

	double pingTime;

	char buffer[512] = "PING";

	clock_t start;
	clock_t end;

	if (mediationSocket == -1)
	{
		cout << "Socket generate fail : toServerThread" << endl;

		*state = false;

		return 0;
	}

	memset(&toServerAddr, 0, sizeof(toServerAddr));

	toServerAddr.sin_family = AF_INET;
	toServerAddr.sin_addr.s_addr = inet_addr(toServerIP.c_str());
	toServerAddr.sin_port = htons(toServerPort);

	addrSize = sizeof(toServerAddr);

	start = clock();
	end = clock();

	pingTime = (double)end - (double)start;

	while (*state == true)
	{
		if (pingTime >= 1000)
		{
			// cout << inet_ntoa(toClientAddr.sin_addr) << ":" << to_string(toClientAddr.sin_port).c_str() << "(" << to_string(ntohs(toClientAddr.sin_port)).c_str() << ")" << endl;

			if (sendto(mediationSocket, buffer, 512, 0, (SOCKADDR*)&toServerAddr, addrSize) != 512)
			{
				cout << "Send packet fail : toServerThread" << endl;

				if (mediationSocket != -1)
					closesocket(mediationSocket);

				*state = false;

				return 0;
			}

			pingTime = 0;

			start = clock();
		}

		end = clock();

		pingTime = (double)end - (double)start;
	}

	return 0;
}