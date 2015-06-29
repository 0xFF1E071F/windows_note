// Lpkdll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>

typedef void  (* FUNTEST)();
FUNTEST  g_funTest = NULL;

// extern "C"  __declspec(dllexport) void Test()
// {
// 	g_funTest();
// }
extern "C"  __declspec(naked) __declspec(dllexport) void Test()
{
	__asm jmp dword ptr [g_funTest]
}

BOOL APIENTRY DllMain( HMODULE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{
	//���̼���ʱ����DLLMainʱ
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{	//���ý������̴߳������˳�������DllMain��ں���
		DisableThreadLibraryCalls(hModule);
		//��ȡԭʼLpkDll.dll�����ĺ�����ַ������.
		char cDllPath[MAX_PATH]={0};
		GetSystemDirectoryA((LPSTR)cDllPath,MAX_PATH);
		strcat(cDllPath,"\\LpkDll.dll");
		HMODULE hDll = LoadLibraryA(cDllPath);
		g_funTest = (FUNTEST)GetProcAddress(hDll, "Test");
		MessageBox(NULL,"dll�ٳ���!","dll�ٳ���!",MB_OK);
	}
    return TRUE;
}

