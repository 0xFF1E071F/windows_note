// LpkDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>

HANDLE g_hModule = NULL;   //�����Լ���ģ����
extern "C" __declspec(dllexport) void Test()
{
	TCHAR tcModulePath[MAX_PATH] = {0};
	//��ȡ���������·��
	GetModuleFileName((HMODULE)g_hModule, tcModulePath, MAX_PATH);
	MessageBox(0, tcModulePath, _T("LpkDll�ļ���·��"), 0);
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{
	g_hModule = hModule; //����ģ����ؾ��
	return TRUE;
}

