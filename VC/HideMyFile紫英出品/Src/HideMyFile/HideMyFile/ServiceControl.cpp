#include "stdafx.h"
#include "ServiceControl.h"
#include <winsvc.h>
//////////Code By Sonsie~~^_^ 2012-06-22////////////////

/////////////////////////////////////////////////////////////////
//������GetServiceStatus
//������sServiceName - ��������
//˵������ȡ��������״̬
//////////////////////////////////////////////////////////////////
DWORD GetServiceStatus(LPCWSTR sServiceName)
{
	SC_HANDLE hSCManager = NULL;//������������
	SC_HANDLE hService = NULL;//������
	SERVICE_STATUS SS = {0};//״ֵ̬
	DWORD dwErrorStaus  = SERVICE_STOPPED;//��������

	//�򿪷��������
	hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager) goto Err;
	//�򿪷���
	hService = OpenService(hSCManager,sServiceName,SERVICE_ALL_ACCESS);
	if (NULL == hService) goto Err;
	//��ȡ����״̬
	if (ControlService(hService,SERVICE_CONTROL_INTERROGATE,&SS) == FALSE)  goto Err;
	//�رվ��
	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hService);
	//����״̬��
	return SS.dwCurrentState;
Err:
	if (NULL != hSCManager) CloseServiceHandle(hSCManager);
	if (NULL != hService) CloseServiceHandle(hService);
	return FALSE;
}

/////////////////////////////////////////////////////////////////
//������StartServiceForUser
//������sServiceName - ��������
//˵������������
//////////////////////////////////////////////////////////////////
BOOL StartServiceForUser(LPCWSTR sServiceName)
{
	SC_HANDLE hSCManager = NULL;//������������
	SC_HANDLE hService = NULL;//������
	//����������״̬�򷵻�
	if (GetServiceStatus(sServiceName) == SERVICE_RUNNING) return TRUE;
	//�򿪷��������
	hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager) goto Err;
	//�򿪷���
	hService = OpenService(hSCManager,sServiceName,SERVICE_ALL_ACCESS);
	if (NULL == hService) goto Err;
	//��������
	if (StartService(hService,0,NULL) == FALSE)  goto Err;
	//�رվ��
	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hService);
	return TRUE;
Err:
	if (NULL != hSCManager) CloseServiceHandle(hSCManager);
	if (NULL != hService) CloseServiceHandle(hService);
	return FALSE;
}
/////////////////////////////////////////////////////////////////
//������StopServiceForUser
//������sServiceName - ��������
//˵����ֹͣ����
//////////////////////////////////////////////////////////////////
BOOL StopServiceForUser(LPCWSTR sServiceName)
{
	SC_HANDLE hSCManager = NULL;//������������
	SC_HANDLE hService = NULL;//������
	SERVICE_STATUS SS = {0};//״ֵ̬
	
	//�򿪷��������
	hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager) goto Err;
	//�򿪷���
	hService = OpenService(hSCManager,sServiceName,SERVICE_ALL_ACCESS);
	if (NULL == hService) goto Err;
	//���Ʒ���
	if (ControlService(hService,SERVICE_CONTROL_STOP,&SS) == FALSE)  goto Err;
	//�رվ��
	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hService);
	return TRUE;
Err:
	if (NULL != hSCManager) CloseServiceHandle(hSCManager);
	if (NULL != hService) CloseServiceHandle(hService);
	return FALSE;
}
/////////////////////////////////////////////////////////////////
//������UninstallServiceFromSCM
//������sServiceName - ��������
//˵����ж�ط���
//////////////////////////////////////////////////////////////////
BOOL UninstallServiceFromSCM(LPCWSTR sServiceName)
{
	SC_HANDLE hSCManager = NULL;//������������
	SC_HANDLE hService = NULL;//������
	//ж��ǰ��ֹͣ����
	StopServiceForUser(sServiceName);
	//�򿪷��������
	hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager) goto Err;
	//�򿪷���
	hService = OpenService(hSCManager,sServiceName,SERVICE_ALL_ACCESS);
	if (NULL == hService) goto Err;
	//ɾ������
	if (DeleteService(hService) == FALSE) goto Err;
	//�رվ��
	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hService);
	return TRUE;
Err:
		if (NULL != hSCManager) CloseServiceHandle(hSCManager);
		if (NULL != hService) CloseServiceHandle(hService);
		return FALSE;
}
/////////////////////////////////////////////////////////////////
//������UninstallServiceFromSCM
//������sDriverFileFullName - �����ļ�·��
//			 sServiceName - ��������
//		     sDisplayName - ������ʾ������
//			 dwStartType - ������ʽ
//˵����ж�ط���
//////////////////////////////////////////////////////////////////
BOOL InstallServiceToSCM(LPCWSTR sDriverFileFullName,LPCWSTR sServiceName,LPCWSTR sDisplayName,DWORD dwStartType)
{
	SC_HANDLE hSCManager = NULL;//������������
	SC_HANDLE hService = NULL;//������
	//��װǰ��ж�ط���
	UninstallServiceFromSCM(sServiceName);
	//�򿪷��������
	hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager) goto Err;
	//��������
	hService = CreateService(hSCManager,sServiceName,	sDisplayName,SERVICE_ALL_ACCESS,SERVICE_KERNEL_DRIVER, dwStartType,
											SERVICE_ERROR_NORMAL,sDriverFileFullName,NULL,NULL,NULL,NULL,NULL);
	if (NULL == hService) goto Err;

	//�رվ��
	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hService);
	return TRUE;
Err:
	if (NULL != hSCManager) CloseServiceHandle(hSCManager);
	if (NULL != hService) CloseServiceHandle(hService);
	return FALSE;
}

