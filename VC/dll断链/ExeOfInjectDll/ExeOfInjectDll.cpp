// ExeOfInjectDll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

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
            //AfxMessageBox("�޸�Ȩ��ʧ��");
            return FALSE;
        }
    }
    CloseHandle(hToken);
    return TRUE;
}

int main(int argc, char* argv[])//����DLL
{
	EnablePrivilege();
	char szDllPath[] = "g:\\beiyong\\biancheng\\test\\dll����\\injectdll����\\debug\\injectdll����.dll";
						//"injectdll����.dll";
	HMODULE handle = LoadLibrary(szDllPath);
	
	
	return 0;
}

