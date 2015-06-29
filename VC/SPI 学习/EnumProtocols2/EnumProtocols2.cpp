///////////////////////////////////////////////////////////
// WSCEnumProtocols.cpp�ļ�

#include "StdAfx.h"
#include <locale.h>
#include <Ws2spi.h>		//  SPI����������Ws2spi.h�ļ���
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

void main()
{
	int nProtocols;
	int nError;
	DWORD dwSize = 0;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	//  �״ε��ã�pProtoInfo����NULL��ȡ����Ҫ�Ļ���������
	if(WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
	{
		if(nError != WSAENOBUFS)
		{
			return ;
		}
	}
	// �����㹻�������ڴ档
	pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
	if (pProtoInfo == NULL)
	{
		return ;
	}
	//�ٴε���WSCEnumProtocols����
	nProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
	wchar_t szWPath[MAX_PATH] = {0};
	int iLen = MAX_PATH;
	//ʹ����̨֧�����ı���////////////////////////////////////////////////////////////////////
	setlocale(LC_ALL, "chs");
	wprintf(L"%-10s%-25s%s\n\n", L"�ṩ��ID", L"Э������", L"DLL·��");
	wchar_t *szWType[3] = {L"�ֲ�Э��", L"����Э��", L"Э����"};
	int iIndex = 0;
	for(int i=0; i<nProtocols; i++)
	{	//��ӡ��������ṩ��ID
		wprintf(L"%-13d", pProtoInfo[i].dwCatalogEntryId);
		//��ӡ��������ṩ��Э������
		iIndex = (pProtoInfo[i].ProtocolChain.ChainLen>1?2:pProtoInfo[i].ProtocolChain.ChainLen);
		wprintf(L"%-15s", szWType[iIndex]);
		//��ӡ��������ṩ������ӦDLL��·��
		WSCGetProviderPath(&pProtoInfo[i].ProviderId, szWPath, &iLen, NULL);
		wprintf(L"%-15s\n", szWPath);
	}
	wprintf(L"���� %d ����������ṩ��\n", nProtocols);
	//�ͷ��ڴ�
	GlobalFree(pProtoInfo);
	//�ָ�����̨����//////////////////////////////////////////////////////////////////////////
	setlocale(LC_ALL, NULL);
}
