// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <process.h>


void Install();
void UnInstall();
BOOL IsInstall();

void Run();
void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv);
void WINAPI Handler(DWORD fdwControl);

SERVICE_STATUS g_status;
SERVICE_STATUS_HANDLE service_status_handle;
SERVICE_DESCRIPTION description;

HANDLE g_hEvent = NULL;

#define lpServiceName _T("First Server")
#define lpDisplayName _T("First Server show!")

int main(int argc, char* argv[])
{

	g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_status.dwCurrentState = SERVICE_STOPPED;
	// ���÷������ʹ�õĿ���
	// ���ϣ��������������ֹͣ��ȥ��SERVICE_ACCEPT_STOP
	// SERVICE_ACCEPT_PAUSE_CONTINUE�Ƿ�����ԡ���ͣ/������
	
	g_status .dwCheckPoint = 0;
	g_status.dwControlsAccepted = \
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN |\
		SERVICE_ACCEPT_PAUSE_CONTINUE;
	g_status.dwServiceSpecificExitCode = 0;
	g_status.dwWaitHint = 0;
	g_status.dwWin32ExitCode = 0;

	//������ʼΪ���źŵ��ֶ��ں��¼���
	g_hEvent = CreateEvent(NULL,TRUE,TRUE,"Pause");
	
	if (argv[1] != NULL && _tcsicmp(argv[1],_T("Install")) == 0)
	{
		Install();
	}
	else if(argv[1] != NULL && _tcsicmp(argv[1],_T("UnInstall")) == 0)
	{
		UnInstall();
	}
	else
	{
		SERVICE_TABLE_ENTRY lpServiceTable[]=
		{
			{lpServiceName,ServiceMain},
			{NULL,NULL}
		};
		StartServiceCtrlDispatcher(lpServiceTable);
	}
	CloseHandle(g_hEvent);
//	system("pause");
	return 0;
}
BOOL IsInstall()
{
	BOOL bResult = FALSE;
	
	SC_HANDLE sc_handle = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (sc_handle != NULL)
	{
		SC_HANDLE sc_hService = OpenService(sc_handle,\
			lpServiceName,\
			SERVICE_QUERY_CONFIG);
		//Enables calling of the QueryServiceConfig function to query
		//the service configuration.
		
		if (sc_hService != NULL)
		{
			bResult = TRUE;
			CloseServiceHandle(sc_hService);
		}
		CloseServiceHandle(sc_handle);
	}
	return bResult;
}
void Install()
{
	if (IsInstall())
	{
		return ;
	}
	SC_HANDLE sc_managerhandle = OpenSCManager(NULL,NULL,\
		SC_MANAGER_ALL_ACCESS);
	if (sc_managerhandle == NULL)
	{
		_tprintf(_T("OpenSCManager����"));
		return;
	}
	TCHAR szFilePath[MAX_PATH];
    DWORD dwLength = GetModuleFileName(NULL,szFilePath,MAX_PATH);
	//װ��һ������ʵ���ľ��������ò���ΪNULL��
   //�ú������ظõ�ǰӦ�ó���ȫ·��	
	
  //�жϳ���·���Ƿ�����ո�,����������·����������.
   if (_tcschr(szFilePath, ' ') != NULL)
   {
	   dwLength += 3;
	   TCHAR *lpFilePath = new TCHAR[dwLength];
	   if (lpFilePath != NULL)
	   {
		   _stprintf(lpFilePath, _T("\"%s\""), szFilePath);
		   _tcscpy(szFilePath, lpFilePath);
		   delete []lpFilePath;
	   }
	}
	//����һ���ֶ������ķ��� 
	SC_HANDLE sc_handle = CreateService(sc_managerhandle,\
		lpServiceName,\
		lpDisplayName,\
		SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,SERVICE_ERROR_IGNORE,\
		szFilePath,NULL,NULL,NULL,NULL,NULL);
	if (sc_handle == NULL)
	{
		CloseServiceHandle(sc_managerhandle);
			_tprintf("CreateService����");

		//return ;
	}


	//����ʧ��
    if(sc_handle==NULL)
    {		
        DWORD dwErrorCode = GetLastError();
        if(dwErrorCode!=ERROR_SERVICE_EXISTS)
        {
            _tprintf("CreateService Error !\n");
            CloseServiceHandle(sc_handle);
			return ;
        }
		//����������
        else
        {
			//��������Ѵ�����򿪷���
            sc_handle=OpenService(sc_managerhandle,\
				lpServiceName,SERVICE_START); //������
			//��ʧ��
            if(sc_handle==NULL)
            {
				_tprintf("OpenService Error !\n");              
                CloseServiceHandle(sc_handle);
                return ;
            }
        }
    }
    else if (description.lpDescription!=NULL)
    {
        ChangeServiceConfig2W(sc_handle,SERVICE_CONFIG_DESCRIPTION,&description);
		//д��������Ϣ
    }
	
    //��������
    if(StartService(sc_handle,0,NULL)==0)
    {
        DWORD dwErrorCode=GetLastError();
		//������������
        if(dwErrorCode == ERROR_SERVICE_ALREADY_RUNNING)
        {
			_tprintf("StartService Error !\n");
			CloseServiceHandle(sc_managerhandle);  
			CloseServiceHandle(sc_handle);
			return ;
        }
    } 
	//��ѯ����״̬
    while(QueryServiceStatus(sc_handle,&g_status)!=0)           
    {   //�����Ƿ��ڳ�ʼ���׶�
        if(g_status.dwCurrentState==SERVICE_START_PENDING)
        {
            Sleep(100);
        }
        else
        {
            break;
        }
    }
	//��ѯ����״̬������û�������ɹ�
    if(g_status.dwCurrentState!=SERVICE_RUNNING)
    {
        _tprintf("Install service Failed\n");
    }
    else
    {
		_tprintf("Install service Successed\n");
    }

	CloseServiceHandle(sc_handle);
	CloseServiceHandle(sc_managerhandle);
	//system("pause");
	return ;
}
void UnInstall()
{
	if (!IsInstall())
	{
		_tprintf(_T("UnInstall����"));
		return ;
	}

	SC_HANDLE sc_handle = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (sc_handle == NULL)
	{
		_tprintf("UnInstall OpenSCManager ʧ��" );
		return ;
	}
    SC_HANDLE sc_service = OpenService(sc_handle,lpServiceName,DELETE | \
		SERVICE_STOP);
	if (sc_service == NULL)
	{
		CloseServiceHandle(sc_handle);
		return ;
	}
	SERVICE_STATUS status;
	//����ֹͣ����,ȷ�������ܹ�������ɾ��.
	ControlService(sc_service,SERVICE_CONTROL_STOP,&status);

	BOOL bResult = DeleteService(sc_service);
    CloseServiceHandle(sc_handle);
    CloseServiceHandle(sc_service);
	if (!bResult)
	{
				_tprintf(_T("DeleteService����"));
	}
//	system("pause");
}
void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv)
{
	service_status_handle = RegisterServiceCtrlHandler(lpServiceName,Handler);
	if (service_status_handle == NULL)
	{
		return ;
	}
	g_status.dwCurrentState = SERVICE_START_PENDING;
	SetServiceStatus(service_status_handle,&g_status);
	g_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(service_status_handle,&g_status);
	
	Run();
	
	g_status.dwWin32ExitCode = S_OK;
	g_status.dwCheckPoint = 0;
	g_status.dwWaitHint = 0;
	g_status.dwCurrentState = SERVICE_STOPPED;//not running
	
	//���÷���״̬Ϊֹͣ���Ӷ��˳�����
	SetServiceStatus(service_status_handle,&g_status);
}
void WINAPI Handler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		//Notifies a service that it should stop. 
		{
			g_status.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(service_status_handle,&g_status);
			
			g_status.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(service_status_handle,&g_status);
		}
		break;
	case SERVICE_CONTROL_PAUSE:
		//Notifies a service that it should pause. 
		{
			g_status.dwCurrentState = SERVICE_CONTINUE_PENDING;
			SetServiceStatus(service_status_handle,&g_status);
			ResetEvent(g_hEvent); //��Ϊ���ź�//֪ͨRUN������ʼ�ȴ�
			g_status.dwCurrentState = SERVICE_PAUSED;
			SetServiceStatus(service_status_handle,&g_status);
		}	
		break;
	case SERVICE_CONTROL_CONTINUE:
		//Notifies a paused service that it should resume.
		{
			g_status.dwCurrentState = SERVICE_CONTINUE_PENDING;
			SetServiceStatus(service_status_handle,&g_status);
			SetEvent(g_hEvent);//֪ͨRUN��������ִ��
			g_status.dwCurrentState = SERVICE_RUNNING;
			SetServiceStatus(service_status_handle,&g_status);
		}
		break;
	case SERVICE_CONTROL_INTERROGATE:
		//Notifies a service that it should report its current 
		//status information to the service control manager.
		{
			break;
		}
	case SERVICE_CONTROL_SHUTDOWN:
		//Notifies a service that the system is shutting down 
		//so the service can perform cleanup tasks. 
		{
			g_status.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(service_status_handle,&g_status);
			
			g_status.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(service_status_handle,&g_status);
		}
		break;
	default:
		break;
	}
	
}
void Run()
{
	while (1)   //ѭ��ɨ��ϵͳ����
	{
		TCHAR tcBuffer[4] = {0}; //׼����С�ڴ�,����̽��ʵ����Ҫ���ڴ��С��
		DWORD dwNeededSize = GetLogicalDriveStrings(4, tcBuffer); //��һ��ִ�У����ʵ����Ҫ�ڴ档
		if (dwNeededSize > 4)
		{   //����ֵ����ָ���ڴ��С,��Ҫ����׼���㹻�ڴ��ٴ�ִ�С�
			TCHAR *pBuffer = new TCHAR[dwNeededSize];  //׼���㹻����ڴ�
			dwNeededSize = GetLogicalDriveStrings(dwNeededSize, pBuffer); //����߼��������ַ���
			TCHAR *pDrive = pBuffer;
			int iPreLength = 0;
			while (1)
			{
				pDrive = pBuffer+iPreLength;  //�����һ��������
				if (pDrive[0] == 0) //��ȡ����������
				{
					break; //�˳�ѭ��
				}
				DWORD dwDriveType = GetDriveType(pDrive); //��ȡ����������
				if (dwDriveType == DRIVE_FIXED)
				{   //������ƶ�������
					TCHAR tcFile[MAX_PATH] = {0};
					_stprintf(tcFile, _T("%sa.txt"), pDrive);
					//���Ѵ��ڵ�a.txt�ļ�
					HANDLE hFile = CreateFile(tcFile, \
						GENERIC_WRITE|GENERIC_READ, \
						FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						//��ʧ���򴴽�һ����
						hFile = CreateFile(tcFile, GENERIC_WRITE|GENERIC_READ,\
							FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						DWORD dwWrite = 0;
						WriteFile(hFile, _T("Hello"), 5, &dwWrite, NULL);
					}
					CloseHandle(hFile);
				}
				iPreLength += _tcslen(pDrive);  //������ǰ������
				iPreLength += sizeof(TCHAR);    //����'\0'��
			}
			if (pBuffer != NULL)
			{
				delete []pBuffer;  //�ͷ��ڴ�
				pBuffer = NULL;
			}
		}
		Sleep(500);  //��ͣ500��������ɨ��
		//���g_hEvent���ź�����ִͣ��
		WaitForSingleObject(g_hEvent, INFINITE);
	}
}