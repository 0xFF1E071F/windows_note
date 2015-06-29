// InstallBho.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <RPC.H>
#include <Rpcdce.h>
#pragma comment(lib, "Rpcrt4.lib")	// ʵ����UuidCreate����
#include <Windows.h>
#include <tchar.h>


#include <string>
using namespace std;

//�ú������ڽ�����������ת�����ַ�����������һ��string����
std::string HexToString(PBYTE pByte, long lLength)
{
	std::string strResult;
	const char cCharTable[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};	
	int iPos=28;
	std::string strTemp;	
	for(int i=0;i<lLength;i++)
	{
		iPos=28;
		strTemp.erase();
		while(!((pByte[i]>>iPos) & 0xf) && iPos>0)
		{
			iPos-=4;
		}		
		for(int j=1;j<2-iPos/4;j++)
		{
			strTemp+=_T('0');
		}		
		for(;iPos>=0;iPos-=4)
		{
			strTemp+=cCharTable[(pByte[i]>>iPos) & 0xf];
		}		
		strResult+=strTemp;
	}
	return strResult;
}

bool InstallBHO(TCHAR *pDllPath)
{
	bool bRet = FALSE;
	GUID ProviderChainGuid;
	//����һ��CLSID��
	if(UuidCreate(&ProviderChainGuid) != RPC_S_OK)
	{
		return bRet;
	}	
	HKEY 	hBhoMainKey = HKEY_LOCAL_MACHINE;
	HKEY 	hClsidMainKey = HKEY_CLASSES_ROOT;
	HKEY 	hBhoSubKey;
	HKEY 	hClsidSubKey;
	DWORD	dwOpenOrCreate = 0; //����ִ�н��:�Ǵ����Ļ��Ǵ򿪵�
	//�����ɵ�CLSIDת���ַ�����
	TCHAR tcBuffer[256] = {0};
	string strData4 = HexToString((PBYTE)ProviderChainGuid.Data4, 2);
	string strCLSID = HexToString((PBYTE)ProviderChainGuid.Data4+2, sizeof(ProviderChainGuid.Data4)-2);
	TCHAR tcCLSID[256] = {0};
	_stprintf(tcCLSID, _T("{%04X-%04X-%04X-%04s-%08s}"), ProviderChainGuid.Data1, ProviderChainGuid.Data2, 
		ProviderChainGuid.Data3, strData4.c_str(), strCLSID.c_str());
	//�����µ�BHO��
	_stprintf(tcBuffer, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\%s"),tcCLSID); 
	LONG ReturnValue = RegCreateKeyEx (hBhoMainKey, tcBuffer, 0L, NULL,
		REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, 
		&hBhoSubKey, &dwOpenOrCreate);
	if(ReturnValue != ERROR_SUCCESS)
	{
		return bRet;
	}
	RegCloseKey(hBhoSubKey); //�ر��½��Ӽ��ľ���ͷ���Դ��
	//Ϊ�´�����BHO�����ù�����DLL��
	_stprintf(tcBuffer, _T("CLSID\\%s\\InprocServer32"), tcCLSID);
	ReturnValue = ::RegCreateKeyEx (hClsidMainKey, tcBuffer, 0L, NULL,
		REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, 
		&hClsidSubKey, &dwOpenOrCreate);
	if(ReturnValue != ERROR_SUCCESS)
	{
		return bRet;
	}
	ReturnValue = RegSetValueEx(hClsidSubKey, NULL, 0, REG_SZ, (BYTE *)pDllPath, _tcslen(pDllPath));
	if(ReturnValue == ERROR_SUCCESS)
	{
		bRet = true;
	}
	RegCloseKey(hClsidSubKey); //�ر��½��Ӽ��ľ���ͷ���Դ��
	return bRet;
}





int main(int argc, char* argv[])
{
	InstallBHO("c:\\a.txt");
	return 0;
}

