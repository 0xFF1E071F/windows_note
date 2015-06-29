// InstallProvider.cpp : Defines the entry point for the console application.
//

#define UNICODE
#define _UNICODE

#include "stdafx.h"
#include <RPC.H>
#include <Rpcdce.h>
#include <Ws2spi.h>
#include <Sporder.h>		// ������WSCWriteProviderOrder����
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Sporder.lib")	// ʵ����UuidCreate����
#pragma comment(lib, "Rpcrt4.lib")	// ʵ����UuidCreate����
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib


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
#define Safe_Delete(pPointer) if(pPointer != NULL) {delete []pPointer; pPointer=NULL;}
#define Safe_New(pPointer, Type, Size)  pPointer = new Type[Size]; if (pPointer == NULL) goto Exit;

BOOL InstallProvider(WCHAR * pProviderName, WCHAR * pwszPathName, int *pProtocols, int iNum)
{
	if (pwszPathName == NULL || pProviderName == NULL)
	{
		return FALSE;
	}
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	//����Ҫ��װ��Э������
	int nIndex = 0, nProtocols, nError, i=0, nArrayCount = 0, *pTemp = NULL;
	DWORD dwSize = 0, *pdwIds = NULL,*pdwOrigCatalogId = NULL;
	WSAPROTOCOL_INFOW *pOriginalProtocolInfo = NULL;
	BOOL bRet = FALSE;
	Safe_New(pTemp, int, iNum)
	//����һ��Ҫ��װ��Э����pTempָ����ڴ���
	memcpy(pTemp, pProtocols, iNum*sizeof(int));
	//Ϊÿ��Э����׼��WSAPROTOCOL_INFOW�ṹ��
	Safe_New(pOriginalProtocolInfo, WSAPROTOCOL_INFOW, iNum)
	//pdwOrigCatalogIdָ��������Ա���ڱ���ֲ�Э���²�Ļ���Э��Ŀ¼ID
	Safe_New(pdwOrigCatalogId, DWORD, iNum)
	DWORD dwLayeredCatalogId;// ���Ƿֲ�Э���Ŀ¼ID��
	// ö�����з�������ṩ��
	nProtocols = GetProvider(pProtoInfo);
	//������Ҫ��װ���ṩ�ߵ�Э����WinsockĿ¼���Ѵ��ڵ�λ��ǰ�˵��ṩ�ߵ�WSAPROTOCOL_INFOW��������
	for(; i<nProtocols; i++) 	//����WinsockĿ¼�������ṩ��
	{   
		for (int j=0; j<iNum; j++)
		{	//��iNum��Э��Ҫ��װ���ṩ�ߡ�
			//�жϵ�ǰ�ṩ�߶�Ӧ��Э���Ƿ���Ҫ��װ���ṩ�ߵ�Э��
			if(pProtoInfo[i].iAddressFamily == AF_INET && pProtoInfo[i].iProtocol == pTemp[j])
			{
				pTemp[j] = -1; //��������ͣ�ȷ����Ҫ���ҵ��ظ�Э����ṩ�ߣ�ֻҪ��ǰ�˵ġ�
				//�����Э������ʹ�õ��ṩ�ߵ���Ϣ
				memcpy(&pOriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				//�ֲ�Э����Ӧ��ȥ��XP1_IFS_HANDLES��־��
				pOriginalProtocolInfo[nArrayCount].dwServiceFlags1 = 
					pOriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				//����Э������ʹ�õ��ṩ�ߵ�Ŀ¼ID����������
				pdwOrigCatalogId[nArrayCount] = pProtoInfo[i].dwCatalogEntryId;
				nArrayCount++;
				break; //�ҵ��󲻱���ѭ������������
			}
		}
	}  
	// ��װ���ǵķֲ�Э�飬��ȡһ��Ŀ¼ID��
	// �����һ���²�Э��Ľṹ���ƹ�������
	WSAPROTOCOL_INFOW LayeredProtocolInfo;
	memcpy(&LayeredProtocolInfo, &pOriginalProtocolInfo[0], sizeof(WSAPROTOCOL_INFOW));
	// �޸�Э�����ƣ����ͣ�����PFL_HIDDEN��־
	wcscpy(LayeredProtocolInfo.szProtocol, pProviderName);
	LayeredProtocolInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL; // ���÷ֲ�Э���־;
	LayeredProtocolInfo.dwProviderFlags |= PFL_HIDDEN;
	
	// ��ȡһ��Guid����װ�ֲ�Э��
	GUID ProviderLayeredGuid;
	if(UuidCreate(&ProviderLayeredGuid) == RPC_S_OK)
	{
		if(WSCInstallProvider(&ProviderLayeredGuid, pwszPathName, &LayeredProtocolInfo, 1, &nError) == SOCKET_ERROR)
		{
			goto Exit;
		}
	}
	// ����ö��Э�飬��ȡ�ֲ�Э���Ŀ¼ID��
	GlobalFree(pProtoInfo);
	nProtocols = GetProvider(pProtoInfo);
	for(i=0; i<nProtocols; i++)
	{	//�����°�װ�ķֲ�Э���ṩ�ߵ�ID�����档
		if(memcmp(&pProtoInfo[i].ProviderId, &ProviderLayeredGuid, sizeof(ProviderLayeredGuid)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}
	// ��װЭ���������Э��������һ���ֲ�Э�顣�ж��ٸ�Э��Ҫ��װ���ṩ�߾�Ҫ��װ���ٸ�Э����
	// �޸�Э�����ƣ�����
	WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
	for(i=0; i<iNum; i++)
	{
		swprintf(wszChainName, L"%ws over %ws", pProviderName, pOriginalProtocolInfo[i].szProtocol);
		wcscpy(pOriginalProtocolInfo[i].szProtocol, wszChainName);
		if(pOriginalProtocolInfo[i].ProtocolChain.ChainLen == 1)
		{	//��ǰЭ������ʹ�õ��ṩ���ǻ����ṩ��,����ṩ�ߵ�IDλ���°�װ��Э�����еڶ���λ��
			pOriginalProtocolInfo[i].ProtocolChain.ChainEntries[1] = pdwOrigCatalogId[i];
		}
		else
		{
			//��ǰЭ������ʹ�õ��ṩ����Э�����ṩ��,�����Э�����������,Э������λ�÷ŷֲ�Э��
			for(int j = pOriginalProtocolInfo[i].ProtocolChain.ChainLen; j>0; j--)
			{
				pOriginalProtocolInfo[i].ProtocolChain.ChainEntries[j] 
				= pOriginalProtocolInfo[i].ProtocolChain.ChainEntries[j-1];
			}
		}
		//���հ�װ�ķֲ�Э����õ�Э����������.
		pOriginalProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogId;
		//Э�������°�װ��һ���ֲ�Э��,����ʹЭ������������Э��������1��
		pOriginalProtocolInfo[i].ProtocolChain.ChainLen++;	
	}
	// ��ȡһ��Guid����װЭ����
	GUID ProviderChainGuid;
	if(UuidCreate(&ProviderChainGuid) == RPC_S_OK)
	{
		if(WSCInstallProvider(&ProviderChainGuid,
			pwszPathName, pOriginalProtocolInfo, iNum, &nError) == SOCKET_ERROR)
		{
			goto Exit;	
		}
	}
	else
	{
		goto Exit;
	}
	// ��������WinsockĿ¼�������ǵ�Э������ǰ
	// ����ö�ٰ�װ��Э��
	GlobalFree(pProtoInfo);
	nProtocols = GetProvider(pProtoInfo);
	Safe_New(pdwIds, DWORD,nProtocols)
	// �������°�װ��Э�����ṩ��ID���õ�ID����ǰ��
	for(i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen > 1) &&
			(pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
		{	//�����°�װ������Э���������׶��Ƿֲ�Э��,�ݴ˲����°�װ��Э����
			pdwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}
	}
	// �������Э���ṩ��ID��ID������
	for(i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen <= 1) ||
			(pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId))
			pdwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
	}
	// ����ID������˳����������WinsockĿ¼
	if((nError = ::WSCWriteProviderOrder(pdwIds, nIndex)) != ERROR_SUCCESS)
	{
		goto Exit;
	}
	bRet = TRUE;   //�����ﰲװ�ɹ�
Exit:
	Safe_Delete(pdwIds)
	Safe_Delete(pOriginalProtocolInfo)
	Safe_Delete(pTemp)
	Safe_Delete(pdwOrigCatalogId)
	if (pProtoInfo != NULL)
	{
		GlobalFree(pProtoInfo);
		pProtoInfo = NULL;
	}
	return bRet;
}

BOOL UnInstall(GUID guidFilter)
{
	BOOL bRet = FALSE;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	DWORD dwLayeredCatalogId;
	// ����Guidȡ�÷ֲ�Э���Ŀ¼ID��
	int nProtocols = GetProvider(pProtoInfo);
	int nError, i=0;
	for(; i<nProtocols; i++)
	{
		if(memcmp(&guidFilter, &pProtoInfo[i].ProviderId, sizeof(guidFilter)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}
	if(i < nProtocols)
	{
		// �Ƴ�Э����
		for(i=0; i<nProtocols; i++)
		{
			if((pProtoInfo[i].ProtocolChain.ChainLen > 1) &&
				(pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
			{
				bRet = WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &nError);
			}
		}
		// �Ƴ��ֲ�Э��
		bRet = WSCDeinstallProvider(&guidFilter, &nError);
	}
	return bRet;
}

int main(int argc, char* argv[])
{
	int iArray[3] = {IPPROTO_UDP, IPPROTO_TCP, IPPROTO_IP};
	InstallProvider(L"SpiDll2", L"E:\\SpiDll2.dll", iArray, 3);
	return 0;
}

