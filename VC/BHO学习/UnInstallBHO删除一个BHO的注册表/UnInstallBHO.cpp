// UnInstallBHO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


bool UnInstallBHO(TCHAR *pCLSID, bool bDeleteFile = FALSE)
{
	HKEY 	hBhoMainKey = HKEY_LOCAL_MACHINE;
	HKEY 	hClsidMainKey = HKEY_CLASSES_ROOT;
	HKEY 	hBhoSubKey;
	HKEY 	hClsidSubKey;
	TCHAR   tcBuffer[256] = {0};
	TCHAR   tcSrcCLSID[24];
	_tcscpy(tcSrcCLSID, pCLSID);
	TCHAR   *pSrcCLSID = _tcsupr(tcSrcCLSID);
	TCHAR   *pDstCLSID = NULL;
	BOOL    bRet = FALSE;
	//��HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects������������ö��Ȩ�ޣ���ö����������Ӽ�
	LONG ReturnValue = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects",
		0L, KEY_ENUMERATE_SUB_KEYS, &hBhoSubKey);
	int i = 0, retCode = 0;
	DWORD dwMaxValueLen = 0, dwValueSize = 0, dwValueType = 0;
	if (ReturnValue == ERROR_SUCCESS) //����ɹ���
	{   
		TCHAR  tcKeyName[128];    //���ڱ���ÿ���Ӽ������ֵ��ڴ�ռ�
		DWORD dwKeyNameSize = 128; 
		while (1)
		{   //��ȡÿ���Ӽ�������
			dwKeyNameSize = 128;
			retCode = RegEnumKeyEx(hBhoSubKey, i, tcKeyName, &dwKeyNameSize, 
				NULL, NULL, NULL, NULL); 
			if (retCode == ERROR_NO_MORE_ITEMS)
			{   //�����ȡ����������ѭ��
				break;
			}
			pDstCLSID = _tcsupr(tcKeyName);
			if (_tcscmp(pSrcCLSID, pDstCLSID) != 0)
			{	//û�ҵ�Ҫɾ����BHO��CLSID������������һ��
				i++;
				continue;
			}
			_stprintf(tcBuffer, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\%s"), tcKeyName,
				pDstCLSID);
			ReturnValue = SHDeleteKey(hBhoMainKey, tcBuffer);
			if (ReturnValue != ERROR_SUCCESS)
			{
				break;
			}
			_stprintf(tcBuffer, _T("CLSID\\%s\\InprocServer32"), pDstCLSID);
			if (bDeleteFile)
			{
				ReturnValue = RegOpenKeyEx (hClsidMainKey, tcBuffer,
					0L, KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE, &hClsidSubKey);
				if (ReturnValue != ERROR_SUCCESS)
				{
					break;
				}
				//��ѯ���Ӽ��µ�ֵ�������������ڴ�ռ�
				ReturnValue = RegQueryInfoKey(hClsidSubKey, NULL, NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, &dwMaxValueLen, NULL, NULL);
				if (ReturnValue != ERROR_SUCCESS)
				{
					RegCloseKey(hClsidSubKey);
					break;
				}
				PBYTE pValueData = new BYTE[dwMaxValueLen];
				dwValueSize = dwMaxValueLen;
				ReturnValue = RegQueryValueEx(hClsidSubKey, NULL, NULL, 
					&dwValueType, pValueData, &dwValueSize); 
				if (ReturnValue != ERROR_SUCCESS)
				{
				}
				if (!DeleteFile((CHAR *)pValueData))
				{
					RegCloseKey(hClsidSubKey);
					delete []pValueData;
					break;
				}
				delete []pValueData;
			}
			ReturnValue = SHDeleteKey(hClsidMainKey, tcBuffer);
			if (ReturnValue == ERROR_SUCCESS)
			{
				bRet = TRUE;
			}
			break;
		}
		RegCloseKey(hBhoSubKey); //�ر��½��Ӽ��ľ���ͷ���Դ��	
	}
	return bRet;
}


int main(int argc, char* argv[])
{
	UnInstallBHO("{3FE8E6B8-6CDF-44B0-B250-0836B2320B7E}", true);
	return 0;
}

