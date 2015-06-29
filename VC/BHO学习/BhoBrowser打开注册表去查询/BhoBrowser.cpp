// BhoBrowser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

int main(int argc, char* argv[])
{
	HKEY 	hBhoMainKey = HKEY_LOCAL_MACHINE;
	HKEY 	hClsidMainKey = HKEY_CLASSES_ROOT;
	HKEY 	hBhoSubKey;
	HKEY 	hClsidSubKey;
	TCHAR   tcBuffer[256] = {0};
	DWORD   dwMaxValueLen = 0, dwValueType = 0, dwValueSize = 0;
	//��HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects������������ö��Ȩ�ޣ���ö����������Ӽ�
	LONG ReturnValue = RegOpenKeyEx (HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects",
		0L, KEY_ENUMERATE_SUB_KEYS, &hBhoSubKey);
	int i = 0, retCode = 0;
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
			printf("%s\n", tcKeyName);
			//��HKEY_CLASSES_ROOT\CLSID\xxxxx\InprocServer32�����������ò�ѯȨ�ޣ��Բ�ѯ��Ĭ���Ӽ�ֵ��
			_stprintf(tcBuffer, _T("CLSID\\%s\\InprocServer32"), tcKeyName);
			ReturnValue = RegOpenKeyEx (hClsidMainKey, tcBuffer,
				0L, KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE, &hClsidSubKey);
			if (ReturnValue != ERROR_SUCCESS)
			{
				i++;
				continue;
			}
			//��ѯ���Ӽ��µ�ֵ�������������ڴ�ռ�
			ReturnValue = RegQueryInfoKey(hClsidSubKey, NULL, NULL, NULL, NULL, 
				NULL, NULL, NULL, NULL, &dwMaxValueLen, NULL, NULL);
			if (ReturnValue != ERROR_SUCCESS)
			{
				RegCloseKey(hClsidSubKey);
				i++;
				continue;
			}
			PBYTE pValueData = new BYTE[dwMaxValueLen];
			dwValueSize = dwMaxValueLen;
			ReturnValue = RegQueryValueEx(hClsidSubKey, NULL, NULL, 
				&dwValueType, pValueData, &dwValueSize); 
			if (ReturnValue != ERROR_SUCCESS)
			{
				RegCloseKey(hClsidSubKey);
				i++;
				delete []pValueData;
				continue;
			}
			printf("%s\n\n", pValueData);
			delete []pValueData;
			RegCloseKey(hClsidSubKey);
			i++;
		}
		RegCloseKey(hBhoSubKey);
	}
	return 0;
}

