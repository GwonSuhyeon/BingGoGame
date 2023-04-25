
#include "OnlineGame.h"

using namespace std;

OnlineGameManager::OnlineGameManager()
{
	memset(&sendPacketMessage, 0, sizeof(sendPacketMessage));
	memset(&socket, 0, sizeof(socket));
	memset(&socketReceiveBuffer, 0, sizeof(socketReceiveBuffer));
}

OnlineGameManager::~OnlineGameManager()
{
	if (commonFunction != nullptr)
	{
		if (commonFunction->userBingGo != nullptr)
		{
			cout << "게임 종료 중..." << endl << endl;

			commonFunction->freeBingGo(commonFunction->userBingGo, binggoSize);

			_sleep(1000);

			cout << "온라인게임 종료!" << endl << endl;

			cout << "3초후 메인메뉴로 돌아갑니다." << endl;

			_sleep(3000);
		}
	}

	if (socket != -1)
		closesocket(socket);

	if (commonFunction != nullptr)
		delete commonFunction;

	if (server != nullptr)
		delete server;

	if (client != nullptr)
		delete client;
}

void OnlineGameManager::run()
{
	int gameMenu = 0;
	//int binggoSize = 0;

	//bool playOrder = true;
	bool playState = false;

	string receivedMessage;

	gameMenu = onlineGameMenu();

	system("cls");

	if (gameMenu == 1)
	{
		cout << endl << "온라인게임 방 만들기" << endl << endl;

		do
		{
			cout << "# 빙고판의 가로, 세로 크기를 입력해주세요.(3 ~ 5) : ";
			cin >> binggoSize;
		} while (binggoSize < 3 || 5 < binggoSize);

		cin.ignore();

		cout << endl << "방 생성 중..." << endl;

		_sleep(1000);

		server = new ServerManager();

		if (server == nullptr)
		{
			cout << "Server class object generation fail : OnlineGame of run" << endl;
			exception.handle();
		}

		if (server->mediationSocketCreator() == false)
		{
			cout << "MediationServer connect fail : OnlineGame of run & Server of mediationSocket" << endl;
			exception.handle();
		}

		server->serverSocketCreator();

		// socket = server->clientSocket;
		socket = server->mediationSocket;

		threadHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)server->toClientThread, &threadState, 0, &threadID);

		if (threadHandle == NULL || threadState == false)
		{
			cout << "Failed to execute toClientHandle : run of OnlineGame" << endl;
			exception.handle();
		}

		socketSize = sizeof(sizeof(server->toClientAddr));

		if(socket != -1)
			playState = serverPlayer();
	}
	else if (gameMenu == 2)
	{
		cout << endl << "온라인게임 방 참가하기" << endl << endl;

		client = new ClientManager();

		if (client == nullptr)
		{
			cout << "Client class object generation fail : OnlineGame of run" << endl;
			exception.handle();
		}

		if (client->mediationSocketCreator() == false)
		{
			cout << "MediationServer connect fail : OnlineGame of run & Server of mediationSocket" << endl;
			exception.handle();
		}

		if (client->clientSocketCreator() == true)
		{
			// socket = client->clientSocket;
			socket = client->mediationSocket;

			threadHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)client->toServerThread, &threadState, 0, &threadID);

			if (threadHandle == NULL || threadState == false)
			{
				cout << "Failed to execute toClientHandle : run of OnlineGame" << endl;
				exception.handle();
			}

			socketSize = sizeof(sizeof(client->clientSocketAddr));

			if(socket != -1)
				playState = clientPlayer();
		}
	}
	else
	{
		cout << "OnlineGame menu select fail : run of OnlineGame" << endl;
		exception.handle();
	}

	if (playState == true)
	{
		cout << "상대방이 수락했습니다." << endl;
		cout << "상대방과 빙고게임을 시작합니다." << endl << endl;

		commonFunction = new CommonFunctionManager();

		if (commonFunction == nullptr)
		{
			cout << "CommonFunctionManager class object generation fail : OnlineGame of run" << endl;

			strncpy(sendPacketMessage, "SettingFail", PACKET_BUFFER_SIZE);

			// send(socket, sendPacketMessage, strlen(sendPacketMessage), 0);
			if(server == nullptr)
				sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&client->clientSocketAddr, sizeof(client->clientSocketAddr));
			else
				sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&server->toClientAddr, sizeof(server->toClientAddr));

			exception.handle();
		}

		_sleep(1000);

		cout << "빙고판 생성 중..." << endl;

		if (server != nullptr && client == nullptr)
		{
			strncpy(sendPacketMessage, to_string(binggoSize).c_str(), PACKET_BUFFER_SIZE);

			// send(socket, sendPacketMessage, strlen(sendPacketMessage), 0);
			sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&server->toClientAddr, sizeof(server->toClientAddr));
		}
		else if (server == nullptr && client != nullptr)
		{
			while (1)
			{
				// recv(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
				recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

				receivedMessage = socketReceiveBuffer;

				if (receivedMessage.compare("PING") == 0)
				{
					continue;
				}
				else
				{
					binggoSize = stoi(receivedMessage);

					break;
				}
			}
		}

		commonFunction->userBingGo = commonFunction->makeBingGo(binggoSize);

		if (commonFunction->userBingGo == nullptr)
		{
			cout << "commonFunction->userBingGo is nullptr : OnlineGame of run" << endl;

			strncpy(sendPacketMessage, "SettingFail", PACKET_BUFFER_SIZE);

			// send(socket, sendPacketMessage, strlen(sendPacketMessage), 0);
			if (server == nullptr)
				sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&client->clientSocketAddr, sizeof(client->clientSocketAddr));
			else
				sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&server->toClientAddr, sizeof(server->toClientAddr));

			exception.handle();
		}

		commonFunction->binggoSetting(commonFunction->userBingGo, binggoSize);

		_sleep(1000);

		strncpy(sendPacketMessage, "SettingComplete", PACKET_BUFFER_SIZE);

		// send(socket, sendPacketMessage, strlen(sendPacketMessage), 0);
		if (server == nullptr)
			sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&client->clientSocketAddr, sizeof(client->clientSocketAddr));
		else
			sendto(socket, sendPacketMessage, strlen(sendPacketMessage), 0, (struct sockaddr*)&server->toClientAddr, sizeof(server->toClientAddr));

		_sleep(1000);

		cout << endl << "빙고판 생성 완료" << endl;
		cout << endl << "상대방이 빙고판을 생성하고 있습니다." << endl;

		_sleep(1000);

		while (1)
		{
			// recv(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
			recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

			receivedMessage = socketReceiveBuffer;

			if (receivedMessage.compare("SettingComplete") == 0)
			{
				cout << endl << "상대방이 빙고판을 생성했습니다." << endl;
				cout << endl << "게임 시작!" << endl;

				_sleep(2000);

				onlineGameStart(commonFunction->userBingGo, binggoSize);

				break;
			}
			else if (receivedMessage.compare("SettingFail") == 0)
			{
				cout << "상대방이 빙고판 생성을 실패하여 게임을 종료합니다." << endl << endl;
				cout << "메인메뉴로 돌아갑니다." << endl;

				_sleep(2000);

				break;
			}
		}
	}
	else
	{
		// 종료
		cout << "메인메뉴로 돌아갑니다." << endl;

		_sleep(2000);
	}

	threadState = false;

	return;
}

int OnlineGameManager::onlineGameMenu()
{
	int num = 0;

	cout << endl << "온라인게임 메뉴입니다." << endl << endl;

	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
	cout << "|          1. 방 만들기            |" << endl;
	cout << "|          2. 방 참가하기          |" << endl;
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl << endl;

	do
	{
		cout << "온라인게임 메뉴선택 : ";
		cin >> num;
	} while (num < 1 || 2 < num);

	return num;
}

bool OnlineGameManager::serverPlayer()
{
	string inputStream;
	string receivedMessage;

	do
	{
		cin.ignore();

		cout << "상대방과 게임을 시작하려면 Y(y) 키를 입력하세요.(취소 : N(n)) : ";

		getline(cin, inputStream);
	} while (inputStream.compare("Y") != 0 && inputStream.compare("y") != 0 && inputStream.compare("N") != 0 && inputStream.compare("n") != 0);

	strncpy(sendPacketMessage, inputStream.c_str(), PACKET_BUFFER_SIZE);

	// send(server->clientSocket, sendPacketMessage, strlen(sendPacketMessage), 0);
	sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&server->toClientAddr, sizeof(server->toClientAddr));

	if (inputStream.compare("N") == 0 || inputStream.compare("n") == 0)
	{
		// 종료
		cout << endl << "게임을 취소합니다." << endl << endl;

		return false;
	}
	else
	{
		cout << endl << "상대방의 게임 수락을 기다리는 중..." << endl << endl;

		while (1)
		{
			// recv(server->clientSocket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
			recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

			receivedMessage = socketReceiveBuffer;

			if (receivedMessage.compare("Y") == 0 || receivedMessage.compare("y") == 0)
			{
				return true;
			}
			else if (receivedMessage.compare("N") == 0 || receivedMessage.compare("n") == 0)
			{
				// 상대방이 종료
				cout << "상대방이 게임을 취소했습니다." << endl << endl;

				return false;
			}
		}
	}
}

bool OnlineGameManager::clientPlayer()
{
	string inputStream;
	string receivedMessage;

	do
	{
		cin.ignore();

		cout << "상대방과 게임을 시작하려면 Y(y) 키를 입력하세요.(취소 : N(n)) : ";

		getline(cin, inputStream);
	} while (inputStream.compare("Y") != 0 && inputStream.compare("y") != 0 && inputStream.compare("N") != 0 && inputStream.compare("n") != 0);

	strncpy(sendPacketMessage, inputStream.c_str(), PACKET_BUFFER_SIZE);

	// send(client->clientSocket, sendPacketMessage, strlen(sendPacketMessage), 0);
	sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&client->clientSocketAddr, sizeof(client->clientSocketAddr));

	if (inputStream.compare("N") == 0 || inputStream.compare("n") == 0)
	{
		// 종료
		cout << endl << "게임을 취소합니다." << endl << endl;

		return false;
	}
	else
	{
		cout << endl << "상대방의 게임 수락을 기다리는 중..." << endl << endl;

		while (1)
		{
			// recv(client->clientSocket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
			recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

			receivedMessage = socketReceiveBuffer;

			if (receivedMessage.compare("Y") == 0 || receivedMessage.compare("y") == 0)
			{
				return true;
			}
			else if (receivedMessage.compare("N") == 0 || receivedMessage.compare("n") == 0)
			{
				// 상대방이 종료
				cout << endl << "상대방이 게임을 취소했습니다." << endl << endl;

				return false;
			}
		}
	}
}

void OnlineGameManager::onlineBingGoPrint(int **temp, int size)
{
	int setwValue = 0;

	if (temp == nullptr)
	{
		cout << "BingGo memory is nullptr : onlineBingGoPrint" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : onlineBingGoPrint" << endl;
		exception.handle();
	}

	if (size * size < 100)
		setwValue = 3;
	else
		setwValue = 4;

	system("cls");

	if (firstRun == true)
	{
		cout << endl << "온라인게임 * 게임순서는 참가자가 선 입니다. *" << endl << endl;

		firstRun = false;
	}
	else
		cout << endl << "온라인게임" << endl << endl;

	for (int i = 0; i < size; i++)
	{
		cout << "|";

		for (int k = 0; k < size; k++)
		{
			if (*(temp + i) + k == nullptr)
			{
				cout << "BingGo memory is nullptr : onlineBingGoPrint of *(temp + i) + k" << endl;
				exception.handle();
			}

			if (*(*(temp + i) + k) == 0)
				cout << "  X";
			else
				cout << right << setw(setwValue) << *(*(temp + i) + k);
		}

		cout << " |" << endl;
	}

	cout << endl;

	return;
}

void OnlineGameManager::onlineGameStart(int **user, int size)
{
	int deleteLine = 0;
	int deleteLineValue = 0;
	int input = 0;
	int maxValue = 0;

	bool playOrder = false;

	string receivedMessage;

	if (user == nullptr)
	{
		cout << "BingGo memory is nullptr : onlineGameStart" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : onlineGameStart" << endl;
		exception.handle();
	}

	maxValue = size * size;

	onlineBingGoPrint(user, size);

	// 게임 진행순서는 항상 클라이언트가 먼저 시작한다.
	if (server == nullptr && client != nullptr)
		playOrder = true;
	else if (server != nullptr && client == nullptr)
		playOrder = false;

	while (1)
	{
		if (playOrder == true)
		{
			do
			{
				cout << "# 지울 숫자 입력(1 ~ " << maxValue << ") : ";
				cin >> input;
			} while (input < 1 || maxValue < input);

			strncpy(sendPacketMessage, to_string(input).c_str(), PACKET_BUFFER_SIZE);

			// send(socket, sendPacketMessage, strlen(sendPacketMessage), 0);
			if (server == nullptr)
				sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&client->clientSocketAddr, sizeof(client->clientSocketAddr));
			else
				sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&server->toClientAddr, sizeof(server->toClientAddr));
		}
		else if (playOrder == false)
		{
			cout << "상대방이 숫자 입력하는 중..." << endl << endl;

			while (1)
			{
				// recv(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
				recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

				receivedMessage = socketReceiveBuffer;

				if (receivedMessage.compare("PING") == 0)
				{
					continue;
				}
				else
				{
					input = stoi(receivedMessage);

					cout << "상대방은 " << input << "을(를) 선택했습니다." << endl;

					break;
				}
			}
		}

		deleteLineValue = commonFunction->binggoNumberCheck(user, size, input);

		if (deleteLineValue != -1)
		{
			if (playOrder == true)
				playOrder = false;
			else if (playOrder == false)
				playOrder = true;

			deleteLine += deleteLineValue;

			strncpy(sendPacketMessage, to_string(deleteLine).c_str(), PACKET_BUFFER_SIZE);

			// send(socket, sendPacketMessage, strlen(sendPacketMessage), 0);
			if (server == nullptr)
				sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&client->clientSocketAddr, sizeof(client->clientSocketAddr));
			else
				sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&server->toClientAddr, sizeof(server->toClientAddr));


			while (1)
			{
				// recv(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
				recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

				receivedMessage = socketReceiveBuffer;

				if (receivedMessage.compare("PING") == 0)
				{
					continue;
				}
				else
				{
					_sleep(1000);

					onlineBingGoPrint(user, size);

					_sleep(1000);

					break;
				}
			}

			if(deleteLine >= size || stoi(receivedMessage) >= size)
				break;
		}
	}

	if (deleteLine >= size && stoi(receivedMessage) >= size)
		cout << "무승부입니다!" << endl << endl;
	else
	{
		if (deleteLine >= size)
			cout << "당신이 승리했습니다!" << endl << endl;
		else if (stoi(receivedMessage) >= size)
			cout << "상대방이 승리했습니다!" << endl << endl;
	}

	_sleep(1000);

	return;
}
