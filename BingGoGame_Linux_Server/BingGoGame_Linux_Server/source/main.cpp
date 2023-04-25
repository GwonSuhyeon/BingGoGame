
#include "MediationServer/MediationServer.h"

#include <iostream>>

using namespace std;

int main()
{
	signal(SIGINT, SIG_IGN); // ctrl + c ����
	signal(SIGTSTP, SIG_IGN); // ctrl + z ����

	cout << "SERVER RUNNING ..." << endl;

	MediationServerManager *serverManager = new MediationServerManager();

	if (serverManager != nullptr)
	{
		serverManager->socketCreator();

		delete serverManager;
	}

	return 0;
}