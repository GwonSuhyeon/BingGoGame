
#pragma once

#include "../CommonFunction.h"
#include "../../ExceptionHandling.h"
#include <iostream>

class MultiGameManager
{
public:
	MultiGameManager();
	~MultiGameManager();

	bool firstRun = true;

	void run();

	void multiBingGoPrint(int **, int **, int);

	void multiGameStart(int **, int **, int);

private:

	int binggoSize = 0;

	ExceptionHandlingManager exception;

	CommonFunctionManager *commonFunction = nullptr;
};