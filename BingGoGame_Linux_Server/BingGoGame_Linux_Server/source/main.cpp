
#include "MediationServer/MediationServer.h"

#include <iostream>>

using namespace std;

int main()
{
	signal(SIGINT, SIG_IGN); // ctrl + c 무시
	signal(SIGTSTP, SIG_IGN); // ctrl + z 무시

	cout << "SERVER RUNNING ..." << endl;

	MediationServerManager *serverManager = new MediationServerManager();

	if (serverManager != nullptr)
	{
		serverManager->socketCreator();

		delete serverManager;
	}

	return 0;
}