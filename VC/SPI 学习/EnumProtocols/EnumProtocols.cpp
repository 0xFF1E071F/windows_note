// EnumProtocols.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
#include <windows.h>
#include <stdio.h>

LPWSAPROTOCOL_INFO GetProvider(LPINT lpnTotalProtocols)
{
	DWORD dwSize = 0;
	LPWSAPROTOCOL_INFO pProtoInfo = NULL;
	
	// ȡ����Ҫ�Ļ���������
	if(WSAEnumProtocols(NULL, pProtoInfo, &dwSize) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAENOBUFS) 
			return NULL;
	}
	// ���뻺�������ٴε���WSAEnumProtocols����
	pProtoInfo = (LPWSAPROTOCOL_INFO)GlobalAlloc(GPTR, dwSize);
	*lpnTotalProtocols = WSAEnumProtocols(NULL, pProtoInfo, &dwSize);
	return pProtoInfo;
}

void FreeProvider(LPWSAPROTOCOL_INFO pProtoInfo)
{
	::GlobalFree(pProtoInfo);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if(WSAStartup(sockVersion, &wsaData) != 0)
	{
		return -1;
	}
	int nTotalProtocols;
	LPWSAPROTOCOL_INFO pProtoInfo = GetProvider(&nTotalProtocols);
	if(pProtoInfo != NULL)
	{
		// ��ӡ�������ṩ�ߵ�Э����Ϣ
		for(int i=0; i<nTotalProtocols; i++)
		{
			printf(" Protocol: %s \n", pProtoInfo[i].szProtocol);
			printf(" CatalogEntryId: %d		ChainLen: %d \n\n", 
				pProtoInfo[i].dwCatalogEntryId, pProtoInfo[i].ProtocolChain.ChainLen);
		}
		FreeProvider(pProtoInfo);
	}
	WSACleanup();	
	return 0;
}

