#include <ws2spi.h>
#include <sporder.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"sporder.lib")

//
// Ҫ��װ��LSP��Ӳ���룬���Ƴ���ʱ��Ҫʹ����
//
GUID  ProviderGuid = {0x9d6c9dd7,0xa201,0x42f5,{0xa8,0xbc,0x03,0xf0,0x1f,0x41,0x72,0xc6}};

//
// ö��Э����
//
LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
{
	DWORD dwSize = 0;
	int nError;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	
	// ȡ����Ҫ�Ļ�������С
	if(::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
	{
		if(nError != WSAENOBUFS)
			return NULL;
	}
	
	// ���仺����
	pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
	
	*lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
	return pProtoInfo;
}

//
// �ͷŻ�����
//
void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
{
	::GlobalFree(pProtoInfo);
}

//
// ��װLSP
//
BOOL InstallProvider(char *pszPathName)
{
	WCHAR wszLSPName[] = L"CuitWlgcxLSP";
	LPWSAPROTOCOL_INFOW pProtoInfo;
	WSAPROTOCOL_INFOW	OriginalProtocolInfo[3];
	DWORD				dwOrigCatalogId[3];
	DWORD				dwLayeredCatalogId;		// ���ǵķֲ�Э��Ŀ¼ID
	int		nProtocols;	
	int		nArrayCount = 0;
	int		nError;
	WCHAR	wszPathName[256];
	
	memset(wszPathName, 0, sizeof(wszPathName));
	if(!::MultiByteToWideChar(CP_ACP, 0, pszPathName, -1, wszPathName, 256))
	{
		printf("MultiByteToWideChar Failed!(%d)\n", ::GetLastError());
		return FALSE;
	}
	// --���LSP DLL·��
	printf("LSP PathName: %s\n", pszPathName);
	
	// �ҵ����ǵ��²�Э�飬����Ϣ����������
	pProtoInfo	  = GetProvider(&nProtocols);
	BOOL bFindUdp = FALSE;
	BOOL bFindTcp = FALSE;
	BOOL bFindRaw = FALSE;
	for(int i=0; i<nProtocols; i++)
	{
		if(pProtoInfo[i].iAddressFamily == AF_INET)
		{
			if(!bFindTcp && pProtoInfo[i].iProtocol == IPPROTO_TCP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				// ȥ��XP1_IFS_HANDLES��־
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				// ����ԭ�������ID
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
				
				bFindTcp = TRUE;
			} 
			if(!bFindUdp && pProtoInfo[i].iProtocol == IPPROTO_UDP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
				bFindUdp = TRUE;
			}
			if(!bFindRaw && pProtoInfo[i].iProtocol == IPPROTO_IP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
				bFindRaw = TRUE;
			}
		}
	}  
	
	// ��װ���ǵķֲ�Э�飬��ȡһ��dwLayeredCatalogId
	// �����һ���²�Э��Ľṹ���ƹ�������
	WSAPROTOCOL_INFOW LayeredProtocolInfo;
	memcpy(&LayeredProtocolInfo, &OriginalProtocolInfo[0], sizeof(WSAPROTOCOL_INFOW));
	// �޸�Э�����ƣ����ͣ�����PFL_HIDDEN��־
	wcscpy(LayeredProtocolInfo.szProtocol, wszLSPName);
	LayeredProtocolInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;
	LayeredProtocolInfo.dwProviderFlags	      |= PFL_HIDDEN;
	// ��ʽ��װ
	if(::WSCInstallProvider(&ProviderGuid, wszPathName, &LayeredProtocolInfo, 1, &nError) == SOCKET_ERROR)
	{
		return FALSE;
	}
	// ����ö��Э�飬��ȡ�ֲ�Э���Ŀ¼ID��
	FreeProvider(pProtoInfo);
	pProtoInfo = GetProvider(&nProtocols);
	for(i=0; i<nProtocols; i++)
	{
		if(memcmp(&pProtoInfo[i].ProviderId, &ProviderGuid, sizeof(ProviderGuid)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}
	
	// ��װЭ����
	// �޸�Э�����ƣ�����
	WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
	for(i=0; i<nArrayCount; i++)
	{
		swprintf(wszChainName, L"%ws over %ws", wszLSPName, OriginalProtocolInfo[i].szProtocol);
		wcscpy(OriginalProtocolInfo[i].szProtocol, wszChainName);
		if(OriginalProtocolInfo[i].ProtocolChain.ChainLen == 1)
		{
			OriginalProtocolInfo[i].ProtocolChain.ChainEntries[1] = dwOrigCatalogId[i];
		}
		else
		{
			for(int j = OriginalProtocolInfo[i].ProtocolChain.ChainLen; j>0; j--)
			{
				OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j] = OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j-1];
			}
		}
		OriginalProtocolInfo[i].ProtocolChain.ChainLen ++;
		OriginalProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogId;	
	}
	// ��ȡһ��Guid����װ֮
	GUID ProviderChainGuid;
	if(::UuidCreate(&ProviderChainGuid) == RPC_S_OK)
	{
		if(::WSCInstallProvider(&ProviderChainGuid, wszPathName, OriginalProtocolInfo, nArrayCount, &nError) == SOCKET_ERROR)
		{
			return FALSE;	
		}
	}
	else
		return FALSE;
	
	// ��������WinsockĿ¼�������ǵ�Э������ǰ
	// ����ö�ٰ�װ��Э��
	FreeProvider(pProtoInfo);
	pProtoInfo = GetProvider(&nProtocols);
	
	DWORD dwIds[20];
	int nIndex = 0;
	// ������ǵ�Э����
	for(i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen > 1) && (pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
		{
			dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}
	}
	// �������Э��
	for(i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen <= 1) || (pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId))
		{
			dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}
	}
	// ��������WinsockĿ¼
	if((nError = ::WSCWriteProviderOrder(dwIds, nIndex)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	FreeProvider(pProtoInfo);
	
	return TRUE;
}

//
// ж��LSP
//
BOOL RemoveProvider()
{
	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols;
	DWORD dwLayeredCatalogId;
	
	// ����Guidȡ�÷ֲ�Э���Ŀ¼ID��
	pProtoInfo = GetProvider(&nProtocols);
	int nError;
	for(int i=0; i<nProtocols; i++)
	{
		if(memcmp(&ProviderGuid, &pProtoInfo[i].ProviderId, sizeof(ProviderGuid)) == 0)
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
			if((pProtoInfo[i].ProtocolChain.ChainLen > 1) && (pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
			{
				::WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &nError);
			}
		}
		// �Ƴ��ֲ�Э��
		::WSCDeinstallProvider(&ProviderGuid, &nError);
	}
	
	return TRUE;
}


void main(int argc, char *argv[])
{
	char szFileName[256];
	char szPathName[256];
	char* p;
	
	if(argc == 3)
	{
		strcpy(szFileName,argv[2]);
		if(strcmp(argv[1], "-install")==0)   
		{
			if(::GetFileAttributes(szFileName) == -1)
			{
				printf("Error:Can't Find File!\n");
				return;
			}
			
			if(::GetFullPathName(szFileName, 256, szPathName, &p) != 0)
			{
				if(InstallProvider(szPathName))
				{
					printf("\nInstall Successully!\n");
					return;
				}
			}
			printf("\nInstall Failed -> %d\n",::GetLastError());
			return;
		}
		else if(strcmp(argv[1],"-remove")==0)  
		{
			if(RemoveProvider())
				printf("\nRemove Successully!\n");
			else
				printf("\nRemove Failed -> %d\n",::GetLastError());
			return;
		}
	}
	printf(" Usage: test.exe [ -install �� -remove ] DllName\n");
	printf("\n\teg:test.exe -install LSP.dll\n");
	printf("\tor:test.exe -remove  LSP.dll\n");
	printf("\n");
}