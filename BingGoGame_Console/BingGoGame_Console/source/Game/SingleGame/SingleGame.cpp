#pragma warning(disable : 4996)

#include "SingleGame.h"
#include <iomanip>

using namespace std;

SingleGameManager::SingleGameManager()
{
	cout << endl << "연습용 빙고게임을 시작합니다." << endl << endl;

	commonFunction = new CommonFunctionManager();

	if (commonFunction == nullptr)
	{
		cout << "CommonFunctionManager class object generation fail : SingleGameManager Constructor" << endl;
		exception.handle();
	}
}

SingleGameManager::~SingleGameManager()
{
	cout << "게임 종료 중..." << endl << endl;

	if(commonFunction->userBingGo != nullptr)
		commonFunction->freeBingGo(commonFunction->userBingGo, binggoSize);

	_sleep(2000);

	cout << "연습 게임 종료!" << endl << endl;

	if (commonFunction != nullptr)
		delete commonFunction;
}

void SingleGameManager::run()
{
	//int binggoSize = 0;

	do
	{
		cout << "# 빙고판의 가로, 세로 크기를 입력해주새요.(3 ~ 30) : ";
		cin >> binggoSize;
	} while (binggoSize < 3 || 30 < binggoSize);

	//CommonFunctionManager commonFunction;

	commonFunction->userBingGo = commonFunction->makeBingGo(binggoSize);

	if (commonFunction->userBingGo == nullptr)
	{
		cout << "commonFunction->userBingGo is nullptr : run of SingleGame" << endl;
		exception.handle();
	}

	commonFunction->binggoSetting(commonFunction->userBingGo, binggoSize);

	singleGameStart(commonFunction->userBingGo, binggoSize);

	return;
}

void SingleGameManager::singleBingGoPrint(int **temp, int size)
{
	int setwValue = 0;

	if (temp == nullptr)
	{
		cout << "BingGo memory is nullptr : singleBingGoPrint" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : singleBingGoPrint" << endl;
		exception.handle();
	}

	if (size * size < 100)
		setwValue = 3;
	else
		setwValue = 4;

	system("cls");

	cout << endl << "싱글게임" << endl << endl;

	for (int i = 0; i < size; i++)
	{
		cout << "|";

		for (int k = 0; k < size; k++)
		{
			if (*(temp + i) + k == nullptr)
			{
				cout << "BingGo memory is nullptr : singleBingGoPrint of *(temp + i) + k" << endl;
				exception.handle();
			}

			if (*(*(temp + i) + k) == 0)
				cout << "  X";
			else
				//printf("%3d", *(*(temp + i) + k));
				cout << right << setw(setwValue) << *(*(temp + i) + k);
		}

		cout << " |" << endl;
	}

	cout << endl;

	return;
}

void SingleGameManager::singleGameStart(int **temp, int size)
{
	int deleteLine = 0;
	int deleteLineValue = 0;
	int input = 0;
	int maxValue = 0;

	if (temp == nullptr)
	{
		cout << "BingGo memory is nullptr : singleGameStart" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : singleGameStart" << endl;
		exception.handle();
	}

	maxValue = size * size;

	singleBingGoPrint(temp, size);

	while (deleteLine < size)
	{
		do
		{
			cout << "# 지울 숫자 입력(1 ~ " << maxValue << ") : ";
			cin >> input;
		} while (input < 1 || maxValue < input);

		deleteLineValue = commonFunction->binggoNumberCheck(temp, size, input);

		if (deleteLineValue != -1)
		{
			deleteLine += deleteLineValue;

			singleBingGoPrint(temp, size);
		}
	}

	return;
}
