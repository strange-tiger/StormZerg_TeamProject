#include "stdafx.h"					// Common.h의 전처리
#include "App.h"

int main(int argc, char* argv[])
{
	if (false == App_Init())		// App의 초기화
	{
		return 1;
	}

	int32 appResult = App_Run();	// App 실행

	return appResult;
}