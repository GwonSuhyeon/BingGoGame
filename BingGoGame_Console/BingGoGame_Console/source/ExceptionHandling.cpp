
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
	cout << endl << "에러가 발생하여 5초후 프로그램이 종료됩니다. 다시 실행해주세요." << endl;

	_sleep(5000);

	exit(0);
}