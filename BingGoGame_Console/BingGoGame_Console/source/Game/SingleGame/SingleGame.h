
#pragma once

#include "../CommonFunction.h"
#include "../../ExceptionHandling.h"
#include <iostream>

class SingleGameManager
{
public:
	SingleGameManager();
	~SingleGameManager();

	void run();

	void singleBingGoPrint(int **, int);

	void singleGameStart(int **, int);
	
private:

	int binggoSize = 0;

	ExceptionHandlingManager exception;

	CommonFunctionManager *commonFunction = nullptr;
};