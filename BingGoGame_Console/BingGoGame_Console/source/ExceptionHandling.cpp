
#pragma warning(disable : 4996)

#include "ExceptionHandling.h"

using namespace std;

ExceptionHandlingManager::ExceptionHandlingManager()
{

}

ExceptionHandlingManager::~ExceptionHandlingManager()
{

}

void ExceptionHandlingManager::handle()
{
	cout << endl << "������ �߻��Ͽ� 5���� ���α׷��� ����˴ϴ�. �ٽ� �������ּ���." << endl;

	_sleep(5000);

	exit(0);
}