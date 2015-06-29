// TLS.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <winnt.h>


void NTAPI tls_callback(PVOID h, DWORD reason, PVOID pv);//__stdcall

int PEB_BeingDebugged();

extern "C" PIMAGE_TLS_CALLBACK tls_callbacktbl[] = {tls_callback,0};


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.


	return 0;
}



void NTAPI tls_callback(PVOID h, DWORD reason, PVOID pv)//__stdcall
{
/*
���е�һ���͵����������������ڶ�������������������������µ���,
һ�����ĸ�ѡ��DLL_PROCESS_ATTACH��DLL_THREAD_ATTACH��DLL_THREAD_DETACH��DLL_PROCESS_DETACH��
DLL_PROCESS_ATTACH����ָ�½��̴���ʱ���ڳ�ʼ�����߳�ʱִ��
DLL_THREAD_ATTACH����ָ���½��̳�ʼ��ʱִ�У����ǲ��������߳�
DLL_THREAD_DETACH����ָ�����е��߳���ֹʱִ�У�����ͬ�����������߳�
DLL_PROCESS_DETACH����ָ�ڽ�����ֹʱִ�С�
*/
	if(reason==DLL_PROCESS_ATTACH)
	{
		if(PEB_BeingDebugged())
			MessageBox(NULL,"TLS CALLBACK FIND DEBUGER","TLS",MB_OK);
		else
			MessageBox(NULL,"TLS CALLBACK NOT FIND DEBUGER","TLS",MB_OK);
	}
	
	return;

};

int PEB_BeingDebugged()
{
	__asm
	{
		mov   eax , fs:[0x30]
		movzx eax, byte ptr [eax+2]
	}
	
}