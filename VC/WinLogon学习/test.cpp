// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string.h>
#include "resource.h"
#include <process.h>


BOOL EnablePrivilege()//������ǰ����Ȩ��
{
	HANDLE hToken = NULL;
	BOOL bFlag = FALSE;

	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))//��ȡ���̵�������Ϣ
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		if (!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid))//
		{
			CloseHandle(hToken);
			return FALSE;
		}
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken ,FALSE ,&tp, sizeof(tp), NULL, NULL))
		{
			MessageBox(NULL,"�޸�Ȩ��ʧ��","�޸�Ȩ��ʧ��",MB_OK);
			return FALSE;
		}
	}
	CloseHandle(hToken);
	return TRUE;
}

int main(int argc, char* argv[])
{
	char szSysDir[100];

	EnablePrivilege();
	GetSystemDirectory(szSysDir,100);
	strcat(szSysDir,"\\WLN.dll");

	HANDLE hFile = CreateFile(szSysDir,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	HRSRC hrsrc = FindResource(NULL,MAKEINTRESOURCE(IDR_DLL1),"dll");

	HGLOBAL hGlobal = LoadResource(NULL,hrsrc);
	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon
	int size = SizeofResource(NULL,hrsrc);

	DWORD dwWrited;
	WriteFile(hFile,(LPVOID)hGlobal,size,&dwWrited,NULL);

	HKEY hKey; 
	DWORD dwDisp;

	LONG ret = 	RegCreateKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify\\WLNTest",\
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,\
		KEY_ALL_ACCESS,\
		NULL,
		&hKey,
		&dwDisp);
	if (ERROR_SUCCESS != ret)
	{
		printf("ʧ��!\n");
		return 0;
	}
	char szDllname[] = "WLN.dll";
	long ret2 = RegSetValueExA(hKey,"DLLName",0,REG_SZ,(unsigned char*)szDllname,sizeof(szDllname)+1);
	
	if (ERROR_SUCCESS != ret2)
	{
		printf("RegSetValueExA1ʧ��!\n");
		return 0;
	}
	char szName1[] = "fnStartSS";
	long ret3 = RegSetValueExA(hKey,"StartScreenSaver",0,REG_SZ,(unsigned char*)szName1,sizeof(szName1)+1);
	if (ERROR_SUCCESS != ret3)
	{
		printf("RegSetValueExA2ʧ��!\n");
		return 0;
	}

	char szName2[] = "fnStopSS";
	long ret4 = RegSetValueExA(hKey,"StopScreenSaver",0,REG_SZ,(unsigned char*)szName2,sizeof(szName1)+1);
	if (ERROR_SUCCESS != ret4)
	{
		printf("RegSetValueExA3ʧ��\n!");
		return 0;
	}
	system("shutdown -l");
	return 0;
}

