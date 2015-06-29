// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

#include "..//dlltest//detour//detours.h"


PVOID g_pOldMessageBoxA=NULL;  
typedef int (WINAPI *PfuncMessageBoxA)(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);  
int WINAPI ZwNewMessageBoxA(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType)  
{  
	return ((PfuncMessageBoxA)g_pOldMessageBoxA)(hWnd, "Hook This!","My hook",uType);  
}  
//��װHook   
BOOL APIENTRY SetHook()  
{  
	//����� ��ʼ�����  
	DetourTransactionBegin();  
	//�����߳���Ϣ  
	DetourUpdateThread(GetCurrentThread());  
	//��ѯ���� ��ַ ��MessageBoxA ��ַ��������� ָ������  
	g_pOldMessageBoxA=DetourFindFunction("User32.dll","MessageBoxA");  
	//�����ǵ����غ��� ���ӵ� MessageBoxA ��ַ��  
	DetourAttach(&g_pOldMessageBoxA,ZwNewMessageBoxA);  
	//���hook  
	LONG ret=DetourTransactionCommit();  
	return ret==NO_ERROR;  
}  

//ж��Hook  
BOOL APIENTRY DropHook()  
{  
	DetourTransactionBegin();  
	DetourUpdateThread(GetCurrentThread());  
	DetourDetach(&g_pOldMessageBoxA, ZwNewMessageBoxA);  
	LONG ret=DetourTransactionCommit();  
	return ret==NO_ERROR;  
}  

static HMODULE s_hDll;  

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved )  
{  
	switch (ul_reason_for_call)  
	{  
	case DLL_PROCESS_ATTACH:  
		s_hDll = hModule;  
		DisableThreadLibraryCalls(hModule);  
		SetHook();  
		break;  
	case DLL_THREAD_ATTACH:  
		break;  
	case DLL_THREAD_DETACH:  
		break;  
	case DLL_PROCESS_DETACH:  
		DropHook();//UNHOOK  
		break;  
	}  
	return TRUE;  
}  

