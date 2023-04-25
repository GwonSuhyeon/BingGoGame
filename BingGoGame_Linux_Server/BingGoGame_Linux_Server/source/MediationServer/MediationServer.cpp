
#include "MediationServer.h"

using namespace std;

MediationServerManager::MediationServerManager()
{
	threadCreate = pthread_create(&threadID, NULL, threadHandle, NULL);

	/*
	if (threadCreate == 0)
	{
		//pthread_detach(threadID);
	}
	*/
}

MediationServerManager::~MediationServerManager()
{
	socketCloser();
}

void MediationServerManager::socketCreator()
{
	// cout << "Socket Function Start" << endl;

	serverSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (serverSocket == -1)
	{
		cout << "Scoket initialization fail : socketCreator" << endl;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(1609);

	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		cout << "serverSocket bind fail : socketCreator" << endl;
	}

	// cout << "Socket Function ..." << endl;

	while (1) // pthread_kill(threadID, 0) != ESRCH
	{
		// cout << "Recv & Send" << endl;
		
		memset(&fromClientAddr, 0, sizeof(fromClientAddr));

		fromClientSize = sizeof(fromClientAddr);
		// cout << "Before recv" << endl;
		packetSize = recvfrom(serverSocket, packet, PACKET_BUFFER_SIZE, 0, (struct sockaddr *)&fromClientAddr, &fromClientSize);
		// cout << "After recv" << endl;
		// cout << "Packet : " << packet << endl;

		if (packetSize < 0)
			continue;

		if (strcmp(packet, "REQUEST") != 0)
			continue;

		// cout << "Recv Success" << endl;

		strcpy(packet, inet_ntoa(fromClientAddr.sin_addr));

		strcat(packet, ":");
		strcat(packet, (to_string(fromClientAddr.sin_port)).c_str());
		strcat(packet, ":");
		strcat(packet, (to_string(ntohs(fromClientAddr.sin_port))).c_str());

		sendto(serverSocket, packet, packetSize, 0, (struct sockaddr*)&fromClientAddr, fromClientSize);

		// cout << "Send Success" << endl;

		// cout << "Client Address : " << packet << endl;
	}

	// cout << "Socket Function End" << endl;

	pthread_join(threadID, &threadReturn);

	return;
}

void MediationServerManager::socketCloser()
{
	if (serverSocket != -1)
		close(serverSocket);

	return;
}

void* MediationServerManager::threadHandle(void *data)
{
	string inputStream;

	// cout << "Thread Start" << endl;

	while (1)
	{
		// cout << "Thread Running 1" << endl;

		getline(cin, inputStream);

		if (inputStream.compare("stop") == 0)
		{
			cout << "Press 'Ctrl + c' for server stop" << endl;

			signal(SIGINT, SIG_DFL);

			break;
		}

		// cout << "Thread Running 2" << endl;

	}
	
	// cout << "Thread End" << endl;

	return NULL;
}