#include "stdafx.h"
#ifndef _SERVICE_H
#define _SERVICE_H

#include <Windows.h>

//��ȡ��������״̬
DWORD GetServiceStatus(LPCWSTR sServiceName);
//��������
BOOL StartServiceForUser(LPCWSTR sServiceName);
//ֹͣ����
BOOL StopServiceForUser(LPCWSTR sServiceName);
//ж�ط���
BOOL UninstallServiceFromSCM(LPCWSTR sServiceName);
//��װ����
BOOL InstallServiceToSCM(LPCWSTR sDriverFileFullName,LPCWSTR sServiceName,
										   LPCWSTR sDisplayName,DWORD dwStartType);

#endif

/*����ʾ��
int _tmain(int argc, _TCHAR* argv[])
{
		DWORD dwStartType = SERVICE_AUTO_START;//�Զ�����
		TCHAR DriverPath[MAX_PATH] = L"E:\\Test.sys";//����·��

		TCHAR *sDriverPath = DriverPath;
		TCHAR sServiceName[] = L"MyDriver";//������
		TCHAR sDlsplayName[] = L"MyDriver";//������

		InstallServiceToSCM(sDriverPath,sServiceName,sDlsplayName,dwStartType);//��װ����
		StartServiceForUser(sServiceName);//��������
		StopServiceForUser(sServiceName);//ֹͣ����
		UninstallServiceFromSCM(sServiceName);//ж�ط���

		return 0;
}
*/