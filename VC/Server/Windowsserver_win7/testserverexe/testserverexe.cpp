// testserverexe.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE hManage =  OpenSCManager(NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE|GENERIC_EXECUTE);

	SC_HANDLE hService =  OpenService(hManage,L"WinService",SERVICE_ALL_ACCESS);
	SERVICE_STATUS State;
	BOOL ret = 	ControlService(hService ,129,&State);
	CloseServiceHandle(hService);
	CloseServiceHandle(hManage);
	return 0;
}

