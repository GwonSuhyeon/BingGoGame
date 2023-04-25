
#pragma warning(disable : 4996)

#include "Server.h"

using namespace std;

SOCKET ServerManager::mediationSocket = NULL;

int ServerManager::toClientPort = 0;

string ServerManager::toClientIP = "";

SOCKADDR_IN ServerManager::toClientAddr = {0};

ServerManager::ServerManager()
{
	memset(&mediationSocketSize, 0, sizeof(mediationSocketSize));
	memset(&packetSize, 0, sizeof(packetSize));
	memset(&ipAddress, 0, sizeof(ipAddress));
	memset(&hostName, 0, sizeof(hostName));
	memset(&mediationBuffer, 0, sizeof(mediationBuffer));
	memset(&wsadata, 0, sizeof(wsadata));
	memset(&serverSocketAddr, 0, sizeof(serverSocketAddr));
	memset(&clientSocketAddr, 0, sizeof(clientSocketAddr));
	memset(&mediationSocketAddr, 0, sizeof(mediationSocketAddr));
	memset(&host, 0, sizeof(host));
	memset(&mediationHost, 0, sizeof(mediationHost));
	memset(&tHandle, 0, sizeof(tHandle));
	memset(&threadID, 0, sizeof(threadID));
	memset(&mediationSocket, 0, sizeof(mediationSocket));

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) // Window Socket 초기화
	{
		cout << "WindowSocket initialization fail : serverSocketCreator" << endl;
		exception.handle();
	}

	mysqlConnect();
}

ServerManager::~ServerManager()
{

}

void ServerManager::mysqlConnect()
{
	PHOSTENT tempHost;

	MYSQL dbConnector;
	MYSQL* connectorPtr = NULL;

	//memset(&tempHost, 0, sizeof(tempHost));

	tempHost = gethostbyname("gwonssuuu.hopto.org");

	if (tempHost != nullptr)
	{
		cout << "IP(IPv4) : " << inet_ntoa(*(struct in_addr*)*tempHost->h_addr_list) << endl;
		/*
		for (int i = 0; host->h_addr_list[i]; i++)
		{
			cout << "IP(IPv4) : " << inet_ntoa(*(struct in_addr*)*host->h_addr_list[i]) << endl;
		}
		*/
	}
	else
	{
		cout << "host is null" << endl;

		return;
	}

	mysql_init(&dbConnector);

	connectorPtr = mysql_real_connect(&dbConnector, inet_ntoa(*(struct in_addr*)*tempHost->h_addr_list), "binggo_user", "Qldrh_authentication03^^", "binggo_db", 1393, (char*)NULL, 0);

	if (connectorPtr == NULL)
	{
		cout << "DB connect fail" << endl;

		return;
	}
	else
	{
		cout << "DB connect success" << endl;

		mysql_close(connectorPtr);
	}

	return;
}

void ServerManager::serverSocketCreator()
{
	int clientSize = 0;

	char recvBuffer[512];

	string recvBufferString;

	if (pingThreadHandle == NULL || pingThreadState == false)
	{
		cout << "Failed to execute pingThreadHandle : serverSocketCreator" << endl;
		exception.handle();
	}

	// 여기에서 스레드 생성해서 client로 udp ping패킷 전송
	while (1)
	{
		cout << "접속을 허용할 클라이언트의 IP를 입력하세요.(IPv4) : ";

		getline(cin, toClientIP);

		if (toClientIP.empty() == false)
			break;
	}

	while (1)
	{
		cout << "접속을 허용할 클라이언트의 포트를 입력하세요. : ";

		cin >> toClientPort;

		if (toClientPort != -1)
			break;
	}

	toClientHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)toClientThread, &toClientThreadState, 0, &toClientThreadID);

	pingThreadState = false;

	if (toClientHandle == NULL || toClientThreadState == false)
	{
		cout << "Failed to execute toClientHandle : serverSocketCreator" << endl;
		exception.handle();
	}

	/*
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // Socket 생성

	if (serverSocket == -1) // -1 대신 INVALID_SOCKET 사용 가능
	{
		cout << "Socket generate fail : serverSocketCreator" << endl;
		exception.handle();
	}

	// Server(주소 정보) 생성 시작
	memset(&serverSocketAddr, 0, sizeof(serverSocketAddr));

	serverSocketAddr.sin_family = AF_INET;

	// INADDR_ANY : IP주소를 32비트 숫자로 리턴(네트워크 바이트 저장 방식 따름)
	serverSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serverSocketAddr.sin_addr.s_addr = htonl(stoul(ipAddress, nullptr, 0));
	// cout << "port : ";
	// cin >> socketPort;
	serverSocketAddr.sin_port = htons(SOCKET_PORT);
	// Server(주소 정보) 생성 종료

	if (bind(serverSocket, (SOCKADDR *)&serverSocketAddr, sizeof(serverSocketAddr)) == -1)
	{
		cout << "serverSocket bind fail : serverSocketCreator" << endl;
		exception.handle();
	}

	if (listen(serverSocket, 1) == -1) // 클라이언트에서 연결을 요청한 소켓들이 대기하는 대기열을 1개로 제한함
	{
		cout << "serverSocket listen fail : serverSocketCreator" << endl;
		exception.handle();
	}
	*/

	cout << "방 생성 완료!" << endl << endl;

	//string aaa = inet_ntoa(serverSocketAddr.sin_addr);
	if (gethostname(hostName, sizeof(hostName)) == 0) // gethostname : 현재 호스트 이름 반환
	{
		host = gethostbyname(hostName); // gethostbyname : 호스트 이름(도메인 명)으로 IP주소를 획득하여 반환
										// gethostbyaddr : 현재 IP주소로 호스트 이름(도메인 명)을 획득하여 반환
		if (host != nullptr)
		{
			/*
			for (int i = 0; host->h_addr_list[i]; i++)
			{
				ipAddress = inet_ntoa(*(struct in_addr *)*host->h_addr_list[i]);

				cout << "1 현재 서버 IP(IPv4) : " << ipAddress << endl;
			}

			cout << endl;
			*/

			ipAddress = inet_ntoa(*(struct in_addr *)*host->h_addr_list);

			cout << "현재 서버 IP(IPv4) : " << ipAddress << endl << endl;
			//cout << "3 현재 서버 IP(IPv4) : " << aaa << endl << endl;
		}
		else
		{
			cout << "Host get fail : serverSocketCreator" << endl;
			exception.handle();
		}
	}
	else
	{
		cout << "Hostname get fail : serverSocketCreator" << endl;
		exception.handle();
	}

	memset(&clientSocketAddr, 0, sizeof(clientSocketAddr));
	clientSize = sizeof(clientSocketAddr);

	cout << "클라이언트 접속 대기 중..." << endl << endl;

	/*
	// accept 함수는 동기식으로 동작함
	// 동기식 : 요청(클라이언트 소켓 접속 요청&승인)을 마무리하기 전까지는 함수가 종료되지 안고 계속 대기함
	clientSocket = accept(serverSocket, (SOCKADDR *)&clientSocketAddr, &clientSize);

	if (clientSocket == -1)
	{
		cout << "Server accept to client fail : serverSocketCreator" << endl;

		exception.handle();
	}
	else
	{
		cout << "클라이언트 접속 성공!" << endl << endl;

		toClientThreadState = false;
	}
	*/

	strcpy(recvBuffer, "CONNECT");

	sendto(mediationSocket, recvBuffer, 512, 0, (SOCKADDR *)&toClientAddr, mediationSocketSize);

	while (1)
	{
		recvfrom(mediationSocket, recvBuffer, 512, 0, NULL, NULL);

		recvBufferString = recvBuffer;

		if (recvBufferString.compare("ACCEPT") == 0)
		{
			break;
		}
	}

	while (1)
	{
		recvfrom(mediationSocket, recvBuffer, 512, 0, NULL, NULL);

		recvBufferString = recvBuffer;

		if (recvBufferString.compare("CONNECT") == 0)
		{
			strcpy(recvBuffer, "ACCEPT");

			sendto(mediationSocket, recvBuffer, 512, 0, (SOCKADDR *)&toClientAddr, mediationSocketSize);

			cout << "클라이언트 접속 성공!" << endl << endl;

			toClientThreadState = false;

			break;
		}
	}

	return;
}

bool ServerManager::mediationSocketCreator()
{
	char* ptr;

	mediationSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // Socket 생성

	if (mediationSocket == -1) // -1 대신 INVALID_SOCKET 사용 가능
	{
		cout << "Socket generate fail : mediationSocketCreator" << endl;
		
		return false;
	}
	
	memset(&udpSocketAddr, 0, sizeof(udpSocketAddr));

	udpSocketAddr.sin_family = AF_INET;
	udpSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	udpSocketAddr.sin_port = htons(10800);
	
	if (bind(mediationSocket, (SOCKADDR *)&udpSocketAddr, sizeof(udpSocketAddr)) == -1)
	{
		cout << "serverSocket bind fail : mediationSocketCreator" << endl;

		return false;
	}
	
	// Server(주소 정보) 생성 시작
	memset(&mediationSocketAddr, 0, sizeof(mediationSocketAddr));

	mediationHost = gethostbyname("gwonssuuu.hopto.org");

	mediationSocketAddr.sin_family = AF_INET;
	mediationSocketAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*mediationHost->h_addr_list));
	mediationSocketAddr.sin_port = htons(1609);

	mediationSocketSize = sizeof(mediationSocketAddr);

	memset(mediationBuffer, 0, sizeof(mediationBuffer));

	strcpy(mediationBuffer, "REQUEST");

	packetSize = sendto(mediationSocket, mediationBuffer, 512, 0, (SOCKADDR *)&mediationSocketAddr, mediationSocketSize);
	
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

void ServerManager::mediationSocketCloser()
{
	if (mediationSocket != -1)
		closesocket(mediationSocket);

	return;
}

void ServerManager::serverSocketCloser()
{
	if (clientSocket != -1)
		closesocket(clientSocket);

	if (serverSocket != -1)
		closesocket(serverSocket);

	WSACleanup();

	return;
}

unsigned int __stdcall ServerManager::threadHandle(int *packet)
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

unsigned int __stdcall ServerManager::pingThread(bool *state)
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

	memset(&pingSocketAddr, 0, sizeof(pingSocketAddr));

	pingHost = gethostbyname("gwonssuuu.hopto.org");

	pingSocketAddr.sin_family = AF_INET;
	pingSocketAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*pingHost->h_addr_list));
	pingSocketAddr.sin_port = htons(1609);

	pingSocketSize = sizeof(pingSocketAddr);

	start = clock();
	end = clock();

	pingTime = (double)end - (double)start;

	while (*state == true)
	{
		if (pingTime >= 1000)
		{
			if (sendto(mediationSocket, buffer, 512, 0, (SOCKADDR *)&pingSocketAddr, pingSocketSize) != 512)
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

unsigned int __stdcall ServerManager::toClientThread(bool* state)
{
	int addrSize;

	double pingTime;

	char buffer[512] = "PING";

	// SOCKET toClientSocket;

	clock_t start;
	clock_t end;

	// toClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP); // Socket 생성

	if (mediationSocket == -1)
	{
		cout << "Socket generate fail : toClientThread" << endl;
		
		*state = false;

		return 0;
	}

	memset(&toClientAddr, 0, sizeof(toClientAddr));

	toClientAddr.sin_family = AF_INET;
	toClientAddr.sin_addr.s_addr = inet_addr(toClientIP.c_str());
	toClientAddr.sin_port = htons(toClientPort);

	addrSize = sizeof(toClientAddr);

	start = clock();
	end = clock();

	pingTime = (double)end - (double)start;

	while (*state == true)
	{
		if (pingTime >= 1000)
		{
			// cout << inet_ntoa(toClientAddr.sin_addr) << ":" << to_string(toClientAddr.sin_port).c_str() << "(" << to_string(ntohs(toClientAddr.sin_port)).c_str() << ")" << endl;

			if (sendto(mediationSocket, buffer, 512, 0, (SOCKADDR *)&toClientAddr, addrSize) != 512)
			{
				cout << "Send packet fail : toClientThread" << endl;

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