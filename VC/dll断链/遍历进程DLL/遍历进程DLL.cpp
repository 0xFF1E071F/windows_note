// ��������DLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Tlhelp32.h>

int main(int argc, char* argv[])//����ģ��
{
	PROCESSENTRY32 processentry32;
	MODULEENTRY32 moduleentry32;
	DWORD PID;
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	Process32First(handle,&processentry32);
	do 
	{
		if (!strcmp(processentry32.szExeFile,"dll����.exe"))
		{
			PID = processentry32.th32ProcessID;
		}
	} while (Process32Next(handle,&processentry32));
	
	
	HANDLE handle2 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,PID);

	Module32First(handle2,&moduleentry32);

	do 
	{
		printf("ģ�����ƣ�%s\r\n",moduleentry32.szModule);
		printf("ģ��·����%s\r\n\n",moduleentry32.szExePath);
		

	} while (Module32Next(handle2,&moduleentry32));
	return 0;
}

