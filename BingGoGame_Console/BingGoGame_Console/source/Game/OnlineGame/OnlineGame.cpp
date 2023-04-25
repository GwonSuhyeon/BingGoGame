
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
			cout << "���� ���� ��..." << endl << endl;

			commonFunction->freeBingGo(commonFunction->userBingGo, binggoSize);

			_sleep(1000);

			cout << "�¶��ΰ��� ����!" << endl << endl;

			cout << "3���� ���θ޴��� ���ư��ϴ�." << endl;

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
		cout << endl << "�¶��ΰ��� �� �����" << endl << endl;

		do
		{
			cout << "# �������� ����, ���� ũ�⸦ �Է����ּ���.(3 ~ 5) : ";
			cin >> binggoSize;
		} while (binggoSize < 3 || 5 < binggoSize);

		cin.ignore();

		cout << endl << "�� ���� ��..." << endl;

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
		cout << endl << "�¶��ΰ��� �� �����ϱ�" << endl << endl;

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
		cout << "������ �����߽��ϴ�." << endl;
		cout << "����� ��������� �����մϴ�." << endl << endl;

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

		cout << "������ ���� ��..." << endl;

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

		cout << endl << "������ ���� �Ϸ�" << endl;
		cout << endl << "������ �������� �����ϰ� �ֽ��ϴ�." << endl;

		_sleep(1000);

		while (1)
		{
			// recv(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0);
			recvfrom(socket, socketReceiveBuffer, PACKET_BUFFER_SIZE, 0, NULL, NULL);

			receivedMessage = socketReceiveBuffer;

			if (receivedMessage.compare("SettingComplete") == 0)
			{
				cout << endl << "������ �������� �����߽��ϴ�." << endl;
				cout << endl << "���� ����!" << endl;

				_sleep(2000);

				onlineGameStart(commonFunction->userBingGo, binggoSize);

				break;
			}
			else if (receivedMessage.compare("SettingFail") == 0)
			{
				cout << "������ ������ ������ �����Ͽ� ������ �����մϴ�." << endl << endl;
				cout << "���θ޴��� ���ư��ϴ�." << endl;

				_sleep(2000);

				break;
			}
		}
	}
	else
	{
		// ����
		cout << "���θ޴��� ���ư��ϴ�." << endl;

		_sleep(2000);
	}

	threadState = false;

	return;
}

int OnlineGameManager::onlineGameMenu()
{
	int num = 0;

	cout << endl << "�¶��ΰ��� �޴��Դϴ�." << endl << endl;

	cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;
	cout << "|          1. �� �����            |" << endl;
	cout << "|          2. �� �����ϱ�          |" << endl;
	cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl << endl;

	do
	{
		cout << "�¶��ΰ��� �޴����� : ";
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

		cout << "����� ������ �����Ϸ��� Y(y) Ű�� �Է��ϼ���.(��� : N(n)) : ";

		getline(cin, inputStream);
	} while (inputStream.compare("Y") != 0 && inputStream.compare("y") != 0 && inputStream.compare("N") != 0 && inputStream.compare("n") != 0);

	strncpy(sendPacketMessage, inputStream.c_str(), PACKET_BUFFER_SIZE);

	// send(server->clientSocket, sendPacketMessage, strlen(sendPacketMessage), 0);
	sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&server->toClientAddr, sizeof(server->toClientAddr));

	if (inputStream.compare("N") == 0 || inputStream.compare("n") == 0)
	{
		// ����
		cout << endl << "������ ����մϴ�." << endl << endl;

		return false;
	}
	else
	{
		cout << endl << "������ ���� ������ ��ٸ��� ��..." << endl << endl;

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
				// ������ ����
				cout << "������ ������ ����߽��ϴ�." << endl << endl;

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

		cout << "����� ������ �����Ϸ��� Y(y) Ű�� �Է��ϼ���.(��� : N(n)) : ";

		getline(cin, inputStream);
	} while (inputStream.compare("Y") != 0 && inputStream.compare("y") != 0 && inputStream.compare("N") != 0 && inputStream.compare("n") != 0);

	strncpy(sendPacketMessage, inputStream.c_str(), PACKET_BUFFER_SIZE);

	// send(client->clientSocket, sendPacketMessage, strlen(sendPacketMessage), 0);
	sendto(socket, sendPacketMessage, PACKET_BUFFER_SIZE, 0, (SOCKADDR *)&client->clientSocketAddr, sizeof(client->clientSocketAddr));

	if (inputStream.compare("N") == 0 || inputStream.compare("n") == 0)
	{
		// ����
		cout << endl << "������ ����մϴ�." << endl << endl;

		return false;
	}
	else
	{
		cout << endl << "������ ���� ������ ��ٸ��� ��..." << endl << endl;

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
				// ������ ����
				cout << endl << "������ ������ ����߽��ϴ�." << endl << endl;

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
		cout << endl << "�¶��ΰ��� * ���Ӽ����� �����ڰ� �� �Դϴ�. *" << endl << endl;

		firstRun = false;
	}
	else
		cout << endl << "�¶��ΰ���" << endl << endl;

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

	// ���� ��������� �׻� Ŭ���̾�Ʈ�� ���� �����Ѵ�.
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
				cout << "# ���� ���� �Է�(1 ~ " << maxValue << ") : ";
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
			cout << "������ ���� �Է��ϴ� ��..." << endl << endl;

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

					cout << "������ " << input << "��(��) �����߽��ϴ�." << endl;

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
		cout << "���º��Դϴ�!" << endl << endl;
	else
	{
		if (deleteLine >= size)
			cout << "����� �¸��߽��ϴ�!" << endl << endl;
		else if (stoi(receivedMessage) >= size)
			cout << "������ �¸��߽��ϴ�!" << endl << endl;
	}

	_sleep(1000);

	return;
}
