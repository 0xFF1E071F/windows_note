// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>


int main(int argc, char* argv[])
{
	HMODULE handle = LoadLibrary("HOOK.dll");
	
	while (1)
	{
		MessageBox(NULL,"����","����",MB_OK);
	}
	return 0;
}

