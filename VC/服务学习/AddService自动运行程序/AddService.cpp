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
main()
{
	char name[100];
	char info[200];
	char path[300];
	printf("�����������\n\n");
	scanf ("%s",&name);
	printf("�������������\n\n");
	scanf ("%s",&info);
	printf("���������·��\n\n");
	scanf ("%s",&path);
	SC_HANDLE manager=NULL;
	SC_HANDLE service=NULL;
	if((manager=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE))==NULL)
	{
		printf("OpenSCManager Error");
	}
	service=CreateService(
			manager,name,info, 
			SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, 
			SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
			path, 0, 0, 0, 0, 0 );
	if(service)
	printf("���񴴽��ɹ�\n\n");
	else
	printf("���񴴽�ʧ��\n\n");
	CloseServiceHandle(service);
	CloseServiceHandle(manager);
}