#pragma warning(disable : 4996)

#include "MultiGame.h"
#include <iomanip>

using namespace std;

MultiGameManager::MultiGameManager()
{
	cout << endl << "����� vs ��ǻ�� ���� ��������� �����մϴ�." << endl << endl;

	commonFunction = new CommonFunctionManager();

	if (commonFunction == nullptr)
	{
		cout << "CommonFunctionManager class object generation fail : MultiGameManager Constructor" << endl;

		exception.handle();
	}
}

MultiGameManager::~MultiGameManager()
{
	cout << "���� ���� ��..." << endl << endl;

	if(commonFunction->userBingGo != nullptr)
		commonFunction->freeBingGo(commonFunction->userBingGo, binggoSize);
	if (commonFunction->computerBingGo != nullptr)
		commonFunction->freeBingGo(commonFunction->computerBingGo, binggoSize);

	_sleep(2000);

	cout << "����� vs ��ǻ�� ���� ����!" << endl;

	if (commonFunction != nullptr)
		delete commonFunction;
}

void MultiGameManager::run()
{
	//int binggoSize = 0;

	do
	{
		cout << "# �������� ����, ���� ũ�⸦ �Է����ּ���.(3 ~ 30) : ";
		cin >> binggoSize;
	} while (binggoSize < 3 || 30 < binggoSize);

	cout << endl;

	cout << "����� ������ ���� ��..." << endl;

	commonFunction->userBingGo = commonFunction->makeBingGo(binggoSize);

	if (commonFunction->userBingGo == nullptr)
	{
		cout << "commonFunction->userBingGo is nullptr : run of MultiGame" << endl;

		exception.handle();
	}

	commonFunction->binggoSetting(commonFunction->userBingGo, binggoSize);

	_sleep(1000);

	cout << "��ǻ�� ������ ���� ��..." << endl;

	commonFunction->computerBingGo = commonFunction->makeBingGo(binggoSize);

	if (commonFunction->computerBingGo == nullptr)
	{
		cout << "commonFunction->computerBingGo is nullptr : run of MultiGame" << endl;

		exception.handle();
	}

	commonFunction->binggoSetting(commonFunction->computerBingGo, binggoSize);

	_sleep(1000);

	cout << endl << "������ ���� �Ϸ�" << endl;

	_sleep(1000);

	cout << endl << "���� ����!" << endl;

	_sleep(1000);

	multiGameStart(commonFunction->userBingGo, commonFunction->computerBingGo, binggoSize);
}

void MultiGameManager::multiBingGoPrint(int **user, int **computer, int size)
{
	int setwValue = 0;

	if (user == nullptr)
	{
		cout << "BingGo memory is nullptr : multiBingGoPrint" << endl;
		exception.handle();
	}

	if (computer == nullptr)
	{	
		cout << "BingGo memory is nullptr : multiBingGoPrint" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : multiBingGoPrint" << endl;
		exception.handle();
	}

	if (size * size < 100)
		setwValue = 3;
	else
		setwValue = 4;

	system("cls");

	if (firstRun == true)
	{
		cout << endl << "��Ƽ���� * ���Ӽ����� Ŭ���̾�Ʈ�� �� �Դϴ�. *" << endl;

		firstRun = false;
	}
	else
		cout << endl << "��Ƽ����" << endl;

	cout << endl << "[USER]" << endl;

	for (int i = 0; i < size; i++)
	{
		cout << "|";

		for (int k = 0; k < size; k++)
		{
			if (*(user + i) + k == nullptr)
			{
				cout << "BingGo memory is nullptr : multiBingGoPrint of *(user + i) + k" << endl;
				exception.handle();
			}

			if (*(*(user + i) + k) == 0)
				cout << "  X";
			else
				//printf("%3d", *(*(user + i) + k));
				cout << right << setw(setwValue) << *(*(user + i) + k);
		}

		cout << " |" << endl;
	}

	cout << endl << "[COMPUTER]" << endl;

	for (int i = 0; i < size; i++)
	{
		cout << "|";

		for (int k = 0; k < size; k++)
		{
			if (*(computer + i) + k == nullptr)
			{
				cout << "BingGo memory is nullptr : multiBingGoPrint of *(computer + i) + k" << endl;
				exception.handle();
			}

			if (*(*(computer + i) + k) == 0)
				cout << "  X";
			else
				cout << "  ?";
		}

		cout << " |" << endl;
	}

	cout << endl;

	return;
}

void MultiGameManager::multiGameStart(int **user, int **computer, int size)
{
	int userDeleteLine = 0;
	int computerDeleteLine = 0;
	int deleteLineValue = 0;
	int input = 0;
	int maxValue = 0;

	bool userOrder = true;
	bool computerOrder = false;

	if (user == nullptr)
	{
		cout << "BingGo memory is nullptr : multiGameStart" << endl;
		exception.handle();
	}

	if (computer == nullptr)
	{
		cout << "BingGo memory is nullptr : multiGameStart" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : multiGameStart" << endl;
		exception.handle();
	}

	maxValue = size * size;

	multiBingGoPrint(user, computer, size);

	while (userDeleteLine < size && computerDeleteLine < size)
	{
		do
		{
			if (userOrder == true)
			{
				cout << "# ���� ���� �Է�(1 ~ " << maxValue << ") : ";
				cin >> input;

				_sleep(1000);
			}

			if (computerOrder == true)
			{
				cout << "��ǻ�Ͱ� ���� �����ϴ� ��..." << endl << endl;

				_sleep(2000);

				input = (rand() % maxValue) + 1;

				cout << "��ǻ�ʹ� " << input << "��(��) �����߽��ϴ�." << endl;

				_sleep(1000);
			}
		} while (input < 1 || maxValue < input);

		deleteLineValue = commonFunction->binggoNumberCheck(user, size, input);

		if (deleteLineValue != -1)
		{
			userDeleteLine += deleteLineValue;

			if (userOrder == true)
				userOrder = false;
			else if (userOrder == false)
				userOrder = true;

			deleteLineValue = 0;

			deleteLineValue = commonFunction->binggoNumberCheck(computer, size, input);

			if (deleteLineValue != -1)
			{
				computerDeleteLine += deleteLineValue;

				if (computerOrder == true)
					computerOrder = false;
				else if (computerOrder == false)
					computerOrder = true;

				deleteLineValue = 0;
			}

			multiBingGoPrint(user, computer, size);

			_sleep(1000);
		}
	}

	_sleep(1000);

	if (userDeleteLine >= size && computerDeleteLine >= size)
		cout << "���º�!" << endl << endl;
	else
	{
		if (userDeleteLine >= size)
			cout << "����� �¸�!" << endl << endl;
		else if (computerDeleteLine >= size)
			cout << "��ǻ�� �¸�!" << endl << endl;
	}

	_sleep(2000);

	return;
}