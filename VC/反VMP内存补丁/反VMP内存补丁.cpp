// ��VMP�ڴ油��.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	//�Թ���ķ�ʽ��������
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&pi,0,sizeof(pi));
	memset(&si,0,sizeof(si));
	BOOL lRet= CreateProcess("5Star.exe",NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi);
	if (lRet==FALSE)
	{
		MessageBoxA(0,"Not Find File!",0,0);
		return 0;
	}
	BYTE command[]={0xEB,0xFE};//2���ֽ��ô���һֱjmp��ǰ��ַ

	HMODULE hModule=GetModuleHandle("user32.dll");

	FARPROC lApi = GetProcAddress(hModule,"CreateWindowExA");//��CreateWindowExAΪ��

	WriteProcessMemory(pi.hProcess,(LPVOID)lApi,&command,2,NULL);//��APIд��EBFE

	ResumeThread(pi.hThread);//�ָ��߳�
	BYTE OldApi[2]={0x8B,0xFF};

	CONTEXT context;
	context.ContextFlags=CONTEXT_FULL;
	while(1)//�ȴ�����CreateWindowExA
	{
		GetThreadContext(pi.hThread,&context);
		if (context.Eip==(UINT)lApi)
		{
			SuspendThread(pi.hThread);//������APIʱ���������
			break;
		}
	}
	//�˴�����Patch
	DWORD address = 0x00408EC2;
	BYTE WriteData[] = {0x74,0x0E,0x90,0x90,0x90,0x90};

	WriteProcessMemory(pi.hProcess,(LPVOID)address,&WriteData,6,NULL);
	WriteProcessMemory(pi.hProcess,(LPVOID)lApi,&OldApi,2,NULL);//�ָ�ԭָ��
  ResumeThread(pi.hThread);
return 0;
}