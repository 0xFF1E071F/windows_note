// ������DLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <windows.h>

//1����dllʹ�þ�̬C����ʱ���������(CRT��C run-time library ).ʱ����Ҫʹ�øú�������Ϊ��ʱdll��ҪDLL_THREAD_ATTACH ��DLL_THREAD_DETATCH֪ͨ���ܷ����������á�
int __stdcall StartAddress(int a1)
{
	return MessageBoxA(0, "HOOK!", "Warning!", 0);
}

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	DWORD hObject;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//MessageBox(NULL,"DLLע��","DLLע��",MB_OK);
		//MessageBox(NULL,"+++++++++","++++++++++",MB_OK);

		DisableThreadLibraryCalls((HMODULE)hModule);
// ����ָ����DLL��DLL_THREAD_ATTACH��DLL_THREAD_DETACH֪ͨ���������Լ�СĳЩ����Ĺ�������С��
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAddress, 0, 0, &hObject);
	    CloseHandle((HANDLE)hObject);

		break;
// 	case DLL_PROCESS_DETACH:
// 		MessageBox(NULL,"+++++++++","++++++++++",MB_OK);
// 		//MessageBox(NULL,"DLLע�����","DLLע�����",MB_OK);
// 		break;
// 	case DLL_THREAD_ATTACH:
// 		
// 		break;
// 	case DLL_THREAD_DETACH:
// 		
// 		break;
	}
    return TRUE;
}

// 
// extern "C" void __declspec(dllexport)  FuncInDll (void)
// {
// 	MessageBox(NULL,"����","����",MB_OK);
// }
// extern "C" void __declspec(dllexport)  FuncInDll2 (void)
// {
// 	MessageBox(NULL,"DLL2","DLL2",MB_OK);
// }