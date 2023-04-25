
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
			cout << endl << "빙고게임을 종료합니다." << endl;

			_sleep(1000); // 리눅스에서는 sleep()이 작동하지만 VS에선 _sleep()을 사용해야 함

			// Sleep은 VS에서 권장하는 방법
			// winApi에서 제공하는 함수이므로 windos.h 필요함
			// 윈도우에 종속된 API 함수이므로 다른 OS의 컴파일러로 컴파일 시 오류 발생 가능성 있음
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
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
	cout << "|             빙고게임             |" << endl;
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl;
	cout << "|  1. 싱글게임(연습 게임)          |" << endl;
	cout << "|  2. 멀티게임(사용자 vs 컴퓨터)   |" << endl;
	cout << "|  3. 온라인게임(사용자 vs 사용자) |" << endl;
	cout << "|  4. 빙고게임 종료                |" << endl;
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << endl << endl;

	do
	{
		cout << "# 메뉴선택 : ";
		cin >> num;
	} while (num < 1 || 4 < num);

	return num;
}