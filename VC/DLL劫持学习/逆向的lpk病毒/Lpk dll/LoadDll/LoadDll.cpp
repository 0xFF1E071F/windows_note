// LoadDll.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE  hFile = LoadLibrary(L"lpk1.dll");

	while (1)
	{
	}
	FreeLibrary(hFile);
	return 0;
}

