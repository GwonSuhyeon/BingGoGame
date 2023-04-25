#pragma warning(disable : 4996)

#include "CommonFunction.h"

using namespace std;

CommonFunctionManager::CommonFunctionManager()
{

}

CommonFunctionManager::~CommonFunctionManager()
{

}

int** CommonFunctionManager::makeBingGo(int size)
{
	//int **temp = (int **)malloc(sizeof(int *) * size);
	int **temp = new int*[size];

	if (temp == nullptr)
	{
		cout << "Memory alloc fail : makeBingGo of **temp" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : makeBingGo" << endl;
		exception.handle();
	}

	for (int i = 0; i < size; i++)
	{
		*(temp + i) = new int[size];

		if (*(temp + i) == nullptr)
		{
			cout << "Memory alloc fail : makeBingGo of *(temp + i)" << endl;
			exception.handle();
		}
	}

	return temp;
}

void CommonFunctionManager::binggoSetting(int **temp, int size)
{
	int maxValue = 0;

	bool exitLoop = false;

	if (temp == nullptr)
	{
		cout << "BingGo memory is nullptr : binggoSetting" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : binggoSetting" << endl;
		exception.handle();
	}

	maxValue = size * size;

	for (int i = 0; i < size; i++)
	{
		for (int k = 0; k < size; k++)
		{
			if (*(temp + i) + k == nullptr)
			{
				cout << "BingGo memory is nullptr : binggoSetting of *(temp + i) + k" << endl;
				exception.handle();
			}

			*(*(temp + i) + k) = (rand() % maxValue) + 1;

			if (!(i == 0 && k == 0))
			{
				exitLoop = false;

				for (int j = 0; j <= i; j++)
				{
					int num = (j == i) ? k : size;

					for (int n = 0; n < num; n++)
					{
						if (*(*(temp + i) + k) == *(*(temp + j) + n))
						{
							k--;
							exitLoop = true;
							break;
						}
					}

					if (exitLoop == true)
						break;
				}
			}
		}
	}

	return;
}

int CommonFunctionManager::binggoNumberCheck(int **temp, int size, int input)
{
	int result = -1;

	if (temp == nullptr)
	{
		cout << "BingGo memory is nullptr : binggoNumberCheck" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : binggoNumberCheck" << endl;
		exception.handle();
	}

	if (input < 1 || (size * size) < input)
	{
		cout << "input variable is no exist : binggoNumberCheck" << endl;
		exception.handle();
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (*(temp + i) + j == nullptr)
			{
				cout << "BingGo memory is nullptr : binggoNumberCheck of *(temp + i) + j" << endl;
				exception.handle();
			}

			if (*(*(temp + i) + j) == input)
			{
				*(*(temp + i) + j) = 0;

				result = binggoLineCheck(temp, size, i, j);

				return result;
			}
		}
	}

	// 잘못 입력 처리
	cout << "* 이미 지워진 숫자 입니다." << endl << endl;

	return result;
}

int CommonFunctionManager::binggoLineCheck(int **temp, int size, int low, int column)
{
	int line = 0;
	int count = 0;

	if (temp == nullptr)
	{
		cout << "BingGo memory is nullptr : binggoLineCheck" << endl;
		exception.handle();
	}

	if (size < 3 || 30 < size)
	{
		cout << "BingGo size variable is no exist : binggoLineCheck" << endl;
		exception.handle();
	}

	if (low < 0 || size <= low)
	{
		cout << "BingGo low variable is no exist : binggoLineCheck" << endl;
		exception.handle();
	}

	if (column < 0 || size <= column)
	{
		cout << "BingGo column variable is no exist : binggoLineCheck" << endl;
		exception.handle();
	}

	for (int i = 0; i < size; i++)
	{
		if (*(temp + low) + i == nullptr)
		{
			cout << "BingGo memory is nullptr : binggoLineCheck of *(temp + low) + i" << endl;
			exception.handle();
		}

		if (*(*(temp + low) + i) == 0)
		{
			count++;

			if (count == size)
				line++;
		}
		else
			break;
	}

	count = 0;

	for (int i = 0; i < size; i++)
	{
		if (*(temp + i) + column == nullptr)
		{
			cout << "BingGo memory is nullptr : binggoLineCheck of *(temp + i) + column" << endl;
			exception.handle();
		}

		if (*(*(temp + i) + column) == 0)
		{
			count++;

			if (count == size)
				line++;
		}
		else
			break;
	}

	if (low == column)
	{
		count = 0;

		for (int i = 0; i < size; i++)
		{
			if (*(temp + i) + i == nullptr)
			{
				cout << "BingGo memory is nullptr : binggoLineCheck of *(temp + i) + i" << endl;
				exception.handle();
			}

			if (*(*(temp + i) + i) == 0)
			{
				count++;

				if (count == size)
					line++;
			}
			else
				break;
		}
	}

	if ((low + column) == (size - 1))
	{
		int lowCount = size - 1;

		count = 0;

		for (int i = size - 1; i >= 0; i--)
		{
			if (*(temp + (lowCount - i)) + i == nullptr)
			{
				cout << "BingGo memory is nullptr : binggoLineCheck of *(temp + (lowCount - i)) + i" << endl;
				exception.handle();
			}

			if (*(*(temp + (lowCount - i)) + i) == 0)
			{
				count++;

				if (count == size)
					line++;
			}
			else
				break;
		}
	}

	return line;
}

void CommonFunctionManager::freeBingGo(int **temp, int size)
{
	if (temp != nullptr)
	{
		if (size < 3 || 30 < size)
		{
			cout << "BingGo size variable is no exist : freeBingGo" << endl;
			exception.handle();
		}

		for (int i = 0; i < size; i++)
		{
			//free(*(temp + i));
			delete[] * (temp + i);
		}

		//free(temp);
		delete[] temp;
	}
	else
	{
		cout << "BingGo memory is nullptr : freeBingGo" << endl;
		exception.handle();
	}

	return;
}