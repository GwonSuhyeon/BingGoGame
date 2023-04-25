
#pragma warning(disable : 4996)

#define HAVE_STRUCT_TIMESPEC

#pragma once

#include "Game/SingleGame/SingleGame.h"
#include "Game/MultiGame/MultiGame.h"
#include "Game/OnlineGame/OnlineGame.h"
#include "ExceptionHandling.h"

#include <time.h>
#include <Windows.h>

// #define CRT_SECURE_NO_WARNING

using namespace std;

int menuSelect();

int main(void)
{
	system("mode con cols=70 lines=30");
	system("title BingGo v2.0 made by Gwon SuHyeon");
	system("color f0");

	int menuNumber = 0;

	ExceptionHandlingManager exception;

	srand((unsigned int)time(NULL));

	while (1)
	{
		system("cls");

		menuNumber = menuSelect();

		system("cls");

		if (menuNumber == 1)
		{
			SingleGameManager *singleGame = new SingleGameManager();

			if (singleGame != nullptr)
			{
				singleGame->run();

				delete singleGame;
			}
			else
			{
				cout << "SingleGameManager class object generation fail : main" << endl;

				exception.handle();
			}
		}
		else if (menuNumber == 2)
		{
			MultiGameManager *multiGame = new MultiGameManager();

			if (multiGame != nullptr)
			{
				multiGame->run();

				delete multiGame;
			}
			else
			{
				cout << "MultiGameManager class object generation fail : main" << endl;

				exception.handle();
			}
		}
		else if (menuNumber == 3)
		{
			OnlineGameManager *onlineGame = new OnlineGameManager();

			if (onlineGame != nullptr)
			{
				onlineGame->run();

				delete onlineGame;
			}
			else
			{
				cout << "OnlineGameManager class object generation fail : main" << endl;

				exception.handle();
			}
		}
		else if (menuNumber == 4)
		{
			cout << endl << "��������� �����մϴ�." << endl;

			_sleep(1000); // ������������ sleep()�� �۵������� VS���� _sleep()�� ����ؾ� ��

			// Sleep�� VS���� �����ϴ� ���
			// winApi���� �����ϴ� �Լ��̹Ƿ� windos.h �ʿ���
			// �����쿡 ���ӵ� API �Լ��̹Ƿ� �ٸ� OS�� �����Ϸ��� ������ �� ���� �߻� ���ɼ� ����
			// Sleep(1000);

			break;
		}
		else
		{
			cout << "Menu select fail : main" << endl;

			return 0;
		}
	}

	return 0;
}

int menuSelect()
{
	int num = 0;

	cout << endl;
	cout << "               * 2021.01.29 * v2.0 *" << endl;
	cout << "            * made by Gwon SuHyeon *" << endl;
	cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;
	cout << "|             �������             |" << endl;
	cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl;
	cout << "|  1. �̱۰���(���� ����)          |" << endl;
	cout << "|  2. ��Ƽ����(����� vs ��ǻ��)   |" << endl;
	cout << "|  3. �¶��ΰ���(����� vs �����) |" << endl;
	cout << "|  4. ������� ����                |" << endl;
	cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << endl << endl;

	do
	{
		cout << "# �޴����� : ";
		cin >> num;
	} while (num < 1 || 4 < num);

	return num;
}