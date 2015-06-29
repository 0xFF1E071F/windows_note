// SpiDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Ws2spi.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

// ����Ҫʹ��UNICODE�ַ���
#define UNICODE
#define _UNICODE

WSPPROC_TABLE g_NextProcTable;//�����²��ṩ������SPI��ַ��
wchar_t	g_szCurrentApp[MAX_PATH];	// ��ǰ���ñ�DLL�ĳ��������
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

int WSPAPI WSPSendTo(
					 SOCKET			s,
					 LPWSABUF		lpBuffers,
					 DWORD			dwBufferCount,
					 LPDWORD			lpNumberOfBytesSent,
					 DWORD			dwFlags,
					 const struct sockaddr FAR * lpTo,
					 int				iTolen,
					 LPWSAOVERLAPPED	lpOverlapped,
					 LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
					 LPWSATHREADID	lpThreadId,
					 LPINT			lpErrno
					 )
{	
	// �ܾ�����Ŀ�Ķ˿�Ϊ6688��UDP���
	SOCKADDR_IN sa = *(SOCKADDR_IN*)lpTo;
	if(sa.sin_port == htons(6688))
	{
		return SOCKET_ERROR;
	}
	return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo
		, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPStartup(WORD wVersionRequested,
					  LPWSPDATA lpWSPData,
					  LPWSAPROTOCOL_INFOW lpProtocolInfo,
					  WSPUPCALLTABLE UpcallTable,
					  LPWSPPROC_TABLE lpProcTable)
{
	//���Ǳ�д��DLL����Э�����У���������ǻ���Э���ֲ�Э��ʹ����ֱ�ӷ��ش���
	if(lpProtocolInfo->ProtocolChain.ChainLen <= 1)
	{	
		return WSAEPROVIDERFAILEDINIT;
	}
	//�����Ӧ�ó����������������أ���ѯ���Ƿ�����ͨ��
	wchar_t szName[512] = {0};
	wsprintfW(szName, L"Ӧ�ó���%s ���������������أ��Ƿ�����", g_szCurrentApp);
	if (MessageBoxW(0, szName, L"ϵͳ��ʾ", MB_YESNO|MB_ICONWARNING) == IDNO)
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
	//�����޸�ǰ��SPI������ַ���Ա����ǵĺ���ʹ��
	g_NextProcTable = *lpProcTable;
	// �޸�SPI������ַ��Hook����Ȥ�ĺ�����������Ϊʾ������Hook��WSPSendTo����
	// ��������Hook������������WSPSocket��WSPCloseSocket��WSPConnect��
 //	lpProcTable->lpWSPSendTo = WSPSendTo;
	GlobalFree(pProtoInfo);
	return nRet;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			// ȡ����ģ�������
			GetModuleFileNameW(NULL, g_szCurrentApp, MAX_PATH);
		}
		break;
	}
	return TRUE;
}

