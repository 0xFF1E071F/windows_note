//////////////////////////////////////////////////////////////////////////////////////
//																					//
// ����:	2007/08/10																//
// ����:	���																	//
// ˵��:	��������� ���дmainfun����															//
// ��ϵ:	QQ 121121606 											//
//////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <winsvc.h>
#include <stdio.h>
void main()
{
	char name[100];
	SC_HANDLE scm;
        SC_HANDLE service;
	SERVICE_STATUS status;
	printf("������Ҫɾ���ķ�����\n\n");
	scanf ("%s",&name);

	if((scm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE))==NULL)
	{
		printf("OpenSCManager Error\n");
	}
	service=OpenService(scm,name,SERVICE_ALL_ACCESS|DELETE);
	if (!service)
	{
		printf("OpenService error!\n");
		return;
	}
        BOOL isSuccess=QueryServiceStatus(service,&status);
	if (!isSuccess)
	{
		printf("QueryServiceStatus error!\n");
		return;
	}
       if ( status.dwCurrentState!=SERVICE_STOPPED )
	{
	    isSuccess=ControlService(service,SERVICE_CONTROL_STOP,&status);
	    if (!isSuccess )
		printf("Stop Service error!\n");
	    Sleep( 500 );
	}
    isSuccess=DeleteService(service);
    if (!isSuccess)
         printf("ɾ������ʧ�ܣ�\n");
    else
         printf("ɾ������ɹ���\n");
    CloseServiceHandle(service );
    CloseServiceHandle(scm);
}