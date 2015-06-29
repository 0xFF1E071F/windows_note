#include "StdAfx.h"
#include "InjectDllWithOutModule.h"

CInjectDllWithOutModule::CInjectDllWithOutModule(const char * pName)
: m_bReadOnly(FALSE)
, m_dwImageSize(0)
,m_pMem(NULL)
{
	if (pName != NULL && strlen(pName) < MAX_PATH)
	{//���汻ע��DLL������·��
		strcpy(m_cName, pName);
	}
	else
	{
		memset(m_cName, 0, MAX_PATH);
	}
}

CInjectDllWithOutModule::~CInjectDllWithOutModule()
{
	if (m_pMem != NULL)
	{
		if (m_bReadOnly)
		{
			UnmapViewOfFile(m_pMem);
		}
		else
		{
			GlobalFree(m_pMem);
		}
	}
	
}
//����DLL
bool CInjectDllWithOutModule::LoadDll(const char * pName, BOOL bReadOnly)
{
	char cBuffer[MAX_PATH] = {0};
	if (pName != NULL)
	{
		strcpy(cBuffer, pName);
	}
	else
	{
		strcpy(cBuffer, m_cName);
	}
	if (!IsDll(cBuffer)) //�ж��Ƿ��ǺϷ���DLL
	{
		return FALSE;
	}
	HANDLE hFile = CreateFile(cBuffer, GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, 0);
	if (hFile == INVALID_HANDLE_VALUE || 
		hFile == NULL)
	{
		int iError = GetLastError();
		return FALSE;
	}
	m_bReadOnly = bReadOnly;
	if (bReadOnly)
	{	//����PE���������ط�ʽӳ���ļ����ڴ�,����ڴ���ֻ����.
		HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE | SEC_IMAGE, 0, 0, NULL);
		if (hMap == NULL)
		{
			CloseHandle(hFile);
			return FALSE;
		}
		m_pMem = MapViewOfFile(hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, NULL);
		CloseHandle(hMap);
		CloseHandle(hFile);
		if (m_pMem == NULL)
		{
			return FALSE;
		}
	}
	else
	{
		DWORD dwFileSize = 0, dwReadInFactSize = 0;
		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize == 0xffffffff)
		{
			CloseHandle(hFile);
			return FALSE;
		}
		LPVOID pFile = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, dwFileSize);
		if (pFile == NULL)
		{
			CloseHandle(hFile);
			return FALSE;
		}
		BOOL bRead = ReadFile(hFile, pFile, dwFileSize, &dwReadInFactSize, NULL);
		if (!bRead || dwFileSize != dwReadInFactSize)
		{
			GlobalFree(pFile);
			CloseHandle(hFile);
			return FALSE;
		}
		PIMAGE_NT_HEADERS pPeHeader = (PIMAGE_NT_HEADERS)((PBYTE)pFile+((PIMAGE_DOS_HEADER)pFile)->e_lfanew);
		PIMAGE_SECTION_HEADER pSectionHeader = (IMAGE_SECTION_HEADER *)((char *)&pPeHeader->OptionalHeader + 
			pPeHeader->FileHeader.SizeOfOptionalHeader);
		//�õ���PE�ļ����ص��ڴ����ܴ�С
		m_dwImageSize = AlignmentNum(pPeHeader->OptionalHeader.ImageBase, 
			pPeHeader->OptionalHeader.SectionAlignment);
		m_pMem = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, m_dwImageSize);
		if (m_pMem == NULL)
		{
			GlobalFree(pFile);
			CloseHandle(hFile);
			return FALSE;
		}
		memcpy(m_pMem, pFile, pPeHeader->OptionalHeader.SizeOfHeaders);
		for (int i=0; i<pPeHeader->FileHeader.NumberOfSections; i++)
		{
			memcpy((PBYTE)m_pMem+pSectionHeader[i].VirtualAddress, 
				(PBYTE)pFile+pSectionHeader[i].PointerToRawData, pSectionHeader[i].SizeOfRawData);
		}
		GlobalFree(pFile);
		CloseHandle(hFile);
	}
	return TRUE;
}


BOOL CInjectDllWithOutModule::IsDll(const char * pName)
{
	char cBuffer[MAX_PATH] = {0};
	if (pName != NULL)
	{
		strcpy(cBuffer, pName);
	}
	else
	{
		strcpy(cBuffer, m_cName);
	}
	//����ָ���ļ����
	HANDLE hFile = CreateFile(cBuffer, GENERIC_READ , FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE || 
		hFile == NULL)
	{	
		return FALSE;
	}
	WORD dwDosFlag = 0;
	DWORD dwReadInFactSize = 0;
	//��ȡ�ļ���ʼ���ֽ�,��MZͷ
	BOOL bRead = ReadFile(hFile, &dwDosFlag, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
	    CloseHandle(hFile);
		return FALSE;
	}
	if (dwDosFlag != 0x5a4d)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	//�õ�e_lfanew ��Ա��IMAGE_DOS_HEADER �ṹ�е�ƫ��
	DWORD dwSize = offsetof(IMAGE_DOS_HEADER, e_lfanew);
	//���ļ�ָ���ƶ����õ�e_lfanew��Ա
	SetFilePointer(hFile, dwSize, NULL, FILE_BEGIN);
	WORD dwOffsetPe = 0;
	//��ȡ�õ�e_lfanew��Ա������,Ҳ����PEͷ���ļ��е�ƫ��
	bRead = ReadFile(hFile, &dwOffsetPe, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	//��ָ���ƶ���PEͷ
	SetFilePointer(hFile, dwOffsetPe, NULL, FILE_BEGIN);
	WORD dwPeFlag = 0;
	//��ȡPE��־
	bRead = ReadFile(hFile, &dwPeFlag, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	if (dwPeFlag != 0x4550)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	//���ļ�����PE��ʽ��
	//��ָ���ƶ���PEͷ
	SetFilePointer(hFile, dwOffsetPe, NULL, FILE_BEGIN);
	IMAGE_NT_HEADERS igmNtHeader;
	//��ȡPEͷ
	bRead = ReadFile(hFile, &igmNtHeader, sizeof(IMAGE_NT_HEADERS), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(IMAGE_NT_HEADERS) != dwReadInFactSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	return IMAGE_FILE_DLL & igmNtHeader.FileHeader.Characteristics ? TRUE : FALSE;
}

//��dwAlignment ����dwOperateNum ֵ,Ҳ������dwOperateNumΪdwAlignment��������
DWORD CInjectDllWithOutModule::AlignmentNum(DWORD dwOperateNum, DWORD dwAlignment)
{
	if (dwAlignment == 0)
	{
		return dwOperateNum;
	}
	int iTemp = dwOperateNum % dwAlignment;
	if (iTemp)
	{
		return dwOperateNum + dwAlignment - iTemp;
	}
	return dwOperateNum;
}

void CInjectDllWithOutModule::RelocateImage(DWORD dwRelocOffset)
{
	assert(m_pMem != NULL);  //���ô˺���ʱm_pMem����Ϊ��
	PIMAGE_NT_HEADERS pPeHeader = (PIMAGE_NT_HEADERS)((PBYTE)m_pMem+((PIMAGE_DOS_HEADER)m_pMem)->e_lfanew);
	DWORD dwRelocTableRVA = pPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	PIMAGE_BASE_RELOCATION pRelocateTable = (PIMAGE_BASE_RELOCATION)((PBYTE)m_pMem+dwRelocTableRVA);
	PWORD parrOffset = NULL;
	DWORD dwRvaCount = 0, dwRva = 0;
	while(pRelocateTable->VirtualAddress != NULL)
	{
		parrOffset = (WORD*)((PBYTE)pRelocateTable + sizeof(IMAGE_BASE_RELOCATION));
		dwRvaCount = (pRelocateTable->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
		for(DWORD i=0; i<dwRvaCount; i++)
		{
			dwRva = parrOffset[i];
			if((dwRva & 0xf000) != 0x3000)
			{
				continue;
			}
			dwRva &= 0x0fff;
			dwRva = (DWORD)((PBYTE)m_pMem + pRelocateTable->VirtualAddress + dwRva);
			*(DWORD*)dwRva += dwRelocOffset;
		}
		pRelocateTable = (PIMAGE_BASE_RELOCATION)((PBYTE)pRelocateTable + pRelocateTable->SizeOfBlock);
	}
}

BOOL CInjectDllWithOutModule::InjectDllAndRun(DWORD dwProcessID, const char * pName)
{
	LPBYTE pInjectPos = NULL;
	HANDLE hProcess = NULL;
	__try
	{
		if((hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_WRITE|PROCESS_VM_OPERATION,
			FALSE, dwProcessID)) == NULL)
		{
			return FALSE;
		}
		if((pInjectPos = (LPBYTE)VirtualAllocEx(hProcess, NULL, m_dwImageSize, 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
		{
			CloseHandle(hProcess);
			return FALSE;
		}
		DWORD dwRva = GetExportFuncAddr(pName);
		if(dwRva == 0)
		{
			VirtualFreeEx(hProcess, pInjectPos, 0, MEM_RELEASE);
			CloseHandle(hProcess);
			return FALSE;
		}
		PIMAGE_NT_HEADERS pPeHeader = (PIMAGE_NT_HEADERS)((PBYTE)m_pMem+((PIMAGE_DOS_HEADER)m_pMem)->e_lfanew);
		DWORD dwRelocOffset = (DWORD)pInjectPos - pPeHeader->OptionalHeader.ImageBase;
		RelocateImage(dwRelocOffset);
		DWORD dwRelocTableRVA = pPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
		//�������ض�λ���ݺ��ض�λ������Ҫ����������������Զ���̺߳�����Ҫ�Ĳ���.
		_InjectData *pInjectData = (_InjectData *)((PBYTE)m_pMem+dwRelocTableRVA);
		pInjectData->dwImageBase = (DWORD)pInjectPos;
		strcpy(pInjectData->cLoad, "LoadLibraryA");
		strcpy(pInjectData->cGetAddr, "GetProcAddress");
		strcpy(pInjectData->cKernel32, "kernel32.dll");
		wcscpy(pInjectData->wcKernel32, L"kernel32.dll");
		if(!WriteProcessMemory(hProcess, pInjectPos, m_pMem, m_dwImageSize, NULL))
			return FALSE;

 		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, 
			(LPTHREAD_START_ROUTINE)((PBYTE)pInjectPos+dwRva), (PBYTE)pInjectPos+dwRelocTableRVA, 0, NULL);
 		if(hThread == NULL)
		{
			int iError = GetLastError();
			if (pInjectPos != NULL && hProcess != NULL)
			{
				if (!VirtualFreeEx(hProcess, pInjectPos, 0, MEM_RELEASE))
				{
					iError = GetLastError();
				}
				
			}
			return FALSE;
		}
		WaitForSingleObject(hThread, 50000);
		if (!VirtualFreeEx(hProcess, pInjectPos, 0, MEM_RELEASE))
		{
			int iError = GetLastError();
		}
	}
	__finally
	{
		if (pInjectPos != NULL && hProcess != NULL)
		{
			if (!VirtualFreeEx(hProcess, pInjectPos, 0, MEM_RELEASE))
			{
				int iError = GetLastError();
			}
		}
	}
	return TRUE;
}

DWORD CInjectDllWithOutModule::GetExportFuncAddr(const char *pFuncName)
{
	assert(m_pMem != NULL);
	assert(pFuncName != NULL);
	PIMAGE_NT_HEADERS pPeHeader = (PIMAGE_NT_HEADERS)((PBYTE)m_pMem+((PIMAGE_DOS_HEADER)m_pMem)->e_lfanew);
	DWORD dwExportTableRVA = pPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	IMAGE_EXPORT_DIRECTORY *pExportDescriptor = (IMAGE_EXPORT_DIRECTORY *)((PBYTE)m_pMem+dwExportTableRVA);
	int iFuncCount = pExportDescriptor->NumberOfNames;
	PDWORD pNameAddrTable = (PDWORD)((PBYTE)m_pMem+pExportDescriptor->AddressOfNames);
	char *pExportName = NULL;
	int i=0;
	for (; i<iFuncCount; i++)
	{
		pExportName = (PCHAR)((PBYTE)m_pMem+pNameAddrTable[i]);
		if (strcmp(pFuncName, pExportName) == 0)
		{
			break;
		}
	}
	DWORD dwRes = 0;
	if (i<iFuncCount)
	{
		DWORD pNameOrdinalsAddrTable = (DWORD)((PBYTE)m_pMem+pExportDescriptor->AddressOfNameOrdinals);
		WORD wOrdinal = ((WORD *)pNameOrdinalsAddrTable)[i];
		PDWORD pExportAddrTable = (PDWORD)((PBYTE)m_pMem+pExportDescriptor->AddressOfFunctions);
		dwRes = pExportAddrTable[wOrdinal];
	}
	return dwRes;
}
