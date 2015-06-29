// TLS_test.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "TLS_test.h"
#include <windows.h>
#include <winnt.h>

#pragma comment(linker, "/INCLUDE:__tls_used")
/*����PIMAGE_TLS_CALLBACK()������ԭ�ͣ����е�һ���͵����������������ڶ��������������������������*/
void NTAPI my_tls_callback1(PVOID h, DWORD reason, PVOID pv)
{
	/*һ�����ĸ�ѡ��DLL_PROCESS_ATTACH��DLL_THREAD_ATTACH��DLL_THREAD_DETACH��DLL_PROCESS_DETACH�����΢�����ġ�Microsoft Portable Executable and Common Object File Format Specification v8��*/
	//���ڽ��̳�ʼ���������߳�ʱִ�еĴ���
	if( reason == DLL_PROCESS_ATTACH ){
		MessageBox(NULL,"hi,this is tls callback","title",MB_OK);
	}
	return;
}
 //.CRT������ʹ��C RunTime����,$�����XLA��X��ʾ����ı�ʶ,L��ʾ��TLS callback section,A�������

#pragma data_seg(".CRT$XLB")
/*���Ҫ������TLS_CallBack���������԰��������д�ɣ�
PIMAGE_TLS_CALLBACK p_thread_callback [] = {tls_callback_A, tls_callback_B, tls_callback_C,0};
����tls_callback_B��tls_callback_CӦ�����㶨��õ�����TLS_callBack����
*/
extern "C" PIMAGE_TLS_CALLBACK my_tls_callbacktbl[] = {TlsCallBackFunction1, 0};
#pragma data_seg()

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    MessageBox(NULL, TEXT("hello world"), TEXT("Tls test"), MB_OK);
	return 0;
}

void NTAPI TlsCallBackFunction1(PVOID h, DWORD reason, PVOID pv)
{
	/*һ�����ĸ�ѡ��DLL_PROCESS_ATTACH��DLL_THREAD_ATTACH��DLL_THREAD_DETACH��DLL_PROCESS_DETACH�����΢�����ġ�Microsoft Portable Executable and Common Object File Format Specification v8��*/
	//���ڽ��̳�ʼ���������߳�ʱִ�еĴ���
	if( reason == DLL_PROCESS_ATTACH ){
		MessageBox(NULL,"It's my test","tls test",MB_OK);
	}
	return;
}
