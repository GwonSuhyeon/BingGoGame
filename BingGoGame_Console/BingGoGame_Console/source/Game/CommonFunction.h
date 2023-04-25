
#pragma once

#include "../ExceptionHandling.h"
#include <iostream>
#include <stdlib.h>

class CommonFunctionManager
{
public:
	CommonFunctionManager();
	~CommonFunctionManager();

	int **userBingGo = nullptr;
	int **computerBingGo = nullptr;

	int** makeBingGo(int);

	void binggoSetting(int **, int);

	int binggoNumberCheck(int **, int, int);

	int binggoLineCheck(int **, int, int, int);

	void freeBingGo(int **, int);

private:

	ExceptionHandlingManager exception;
};