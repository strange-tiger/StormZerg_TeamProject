#include "stdafx.h"					// Common.h�� ��ó��
#include "App.h"

int main(int argc, char* argv[])
{
	if (false == App_Init())		// App�� �ʱ�ȭ
	{
		return 1;
	}

	int32 appResult = App_Run();	// App ����

	return appResult;
}