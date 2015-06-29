#include <windows.h>
#include <iostream.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <iostream.h>

struct RemoteStruct //Զ�̽ṹ��
{
	char MyText[255];		//����
	char MyCaption[255];	//����
	DWORD Address;			//����ض�λ�Ժ��MessageBox��ַ
};

typedef int (WINAPI*MyMessageBox)(HWND,LPCTSTR,LPCTSTR,UINT); //����ԭ��

void WINAPI MyThread(void *MyArg) //Զ�̽���
{
	RemoteStruct *MyRemoteStruct=(RemoteStruct*)MyArg;
	MyMessageBox My;
	My=(MyMessageBox)MyRemoteStruct->Address; //ָ��MessageBox��ַ
	My(NULL,MyRemoteStruct->MyText,MyRemoteStruct->MyCaption,MB_OK); //���ú���
}

void ProcessList()
{
	//�г����������Ӧ��Pid
	HANDLE SnapshotHandle=NULL; //���վ��
	SnapshotHandle=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); //�������̿���
	if(SnapshotHandle==NULL)
	{
		system("cls");
		cout<<"����"<<endl;
		cout<<"�������̿���ʧ�ܡ�"<<endl;
		system("pause");
		exit(1);
	}
	cout<<"ProcessName:                   PID:"<<endl;
	BOOL All=FALSE; //�Ƿ�ȫ������������
	PROCESSENTRY32W Mylppe;
	Mylppe.dwSize=sizeof(Mylppe); //���ô�С

	All=::Process32FirstW(SnapshotHandle,&Mylppe); //ȡ�õ�һ�����̿���
	while(All) //ѭ��ȡ��
	{
		for (int i = 0;Mylppe.szExeFile[i]!='\0';i++)
		{
			printf("%c",Mylppe.szExeFile[i]);
		}
		cout<<"                      "<<Mylppe.th32ProcessID<<endl;
		All=::Process32NextW(SnapshotHandle,&Mylppe); 
		//ȡ���¸�
	}
	CloseHandle(SnapshotHandle); //�ͷſ��վ��
}

int main()
{
	DWORD Pid=0;
	char FunctionText[255]={NULL}; //����
	char FunctionCaption[255]={NULL}; //����
	
	LPVOID lpArgBuf=NULL; //����Ŀռ�(����)
	LPVOID lpFunBuf=NULL; //����Ŀռ�(����)
	
	DWORD WriteByteArg=0; //����ʵ��д���ֽ���
	DWORD WriteByteFun=0; //����ʵ��д���ֽ���
	
	RemoteStruct MyRemoteStruct;
	while(1)
	{
		cout<<"------------------------------------"<<endl;
		cout<<"�����ǽ��̣���ѡ����Ҫע��Ľ��̣�����������Pid��"<<endl;
		ProcessList(); //��������
		cout<<endl; //����
		cout<<"������Ҫ��Ľ���ID��(Pid)��"<<endl;
		cin>>Pid;
		if(Pid==0)
		{
			system("cls");
			cout<<"����"<<endl;
			cout<<"��ȷ���������Pid����ȷ�ģ�"<<endl;
			system("pause");
			exit(1);
		}
		//�򿪽���
		HANDLE ProcessHandle=NULL;
		ProcessHandle=::OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid); //�򿪽���
		if(ProcessHandle == NULL) //ʧ��
		{
			system("cls");
			cout<<"����"<<endl;
			cout<<"���ܴ򿪽��̣���ȷ���������Pid����ȷ�ģ�"<<endl;
			system("pause");
			exit(1);
		}
		//ȡ��MessageBox�Ĳ���
		cout<<"�򿪽��̳ɹ���������Ҫ��������ݣ�"<<endl;
		cin>>FunctionText;
		cout<<"������Ҫ����ı��⣺"<<endl;
		cin>>FunctionCaption;
		//��ָ������������ռ��������������
		strcpy(MyRemoteStruct.MyText,FunctionText);
		strcpy(MyRemoteStruct.MyCaption,FunctionCaption);

		MyRemoteStruct.Address=(DWORD)MessageBox; //ȡ�ú�����ַ

		lpArgBuf=::VirtualAllocEx(ProcessHandle,0,1024,MEM_COMMIT,PAGE_EXECUTE_READWRITE); //����ռ䲢��¼�ռ�
		if(lpArgBuf==NULL) //ʧ��
		{
			system("cls");
			cout<<"����"<<endl;
			cout<<"�����ڴ˽���������ռ�(����)��"<<endl;
			system("pause");
			exit(1);
		}
if(!WriteProcessMemory(ProcessHandle,lpArgBuf,&MyRemoteStruct,sizeof(MyRemoteStruct),&WriteByteArg))
 //д�����
		{
			system("cls");
			cout<<"����"<<endl;
			cout<<"����д�������"<<endl;
			system("pause");
			exit(1);
		}
		//�����ڴ�д���̺߳���
		lpFunBuf=::VirtualAllocEx(ProcessHandle,0,4096,MEM_COMMIT,PAGE_EXECUTE_READWRITE); //������ַ
		if(lpFunBuf==NULL)
		{
			system("cls");
			cout<<"����"<<endl;
			cout<<"�����ڴ˽���������ռ�(����)��"<<endl;
			system("pause");
			exit(1);
		}
		//д�뺯��(��СΪ4096)
		if(!WriteProcessMemory(ProcessHandle,lpFunBuf,&MyThread,4096,&WriteByteFun))
		{
			system("cls");
			cout<<"����"<<endl;
			cout<<"����д�뺯����"<<endl;
			system("pause");
			exit(1);
		}
		//Զ��ִ�к���(�߳�)
		DWORD ThreadId=0; 
		//Զ���߳�Id
		HANDLE hThread=::CreateRemoteThread(ProcessHandle,0,0,\
			(LPTHREAD_START_ROUTINE)lpFunBuf,lpArgBuf,0,&ThreadId);
		//ִ�к���
		::WaitForSingleObject(hThread,INFINITE); //�ȴ�Զ�̺���ִ�����
		VirtualFreeEx(ProcessHandle,lpFunBuf,4096,MEM_COMMIT); //�ͷź����ռ�
		VirtualFreeEx(ProcessHandle,lpArgBuf,1024,MEM_COMMIT); //�ͷŲ����ռ�
		CloseHandle(ProcessHandle);
		cout<<"ע��ɹ���"<<endl;
		system("pause");
		system("cls");
	}
	return 0;
}