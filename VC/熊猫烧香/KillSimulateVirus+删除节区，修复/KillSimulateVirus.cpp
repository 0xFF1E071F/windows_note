// KillSimulateVirus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>
#include <Windows.h>
#include <stddef.h>


//ɨ�赽�ļ�ʱ���õĻص���������
enum VIRUS_DEAL_TYPE{NOT_PE, PE_NOT_VIRUS, INFECT_HEADSTREAM_DELETED, CARRY_VIRUS_CLEARED, VIRUS_FAILED};
typedef VIRUS_DEAL_TYPE (*ScanFileCallBack)(TCHAR *pFileName, PVOID pThis);

void ScanDirectory(TCHAR *pStrDirectory, ScanFileCallBack funcDoSomething, PVOID pThis = NULL)
{
	if(pStrDirectory == NULL)
	{
		return ;
	}
	WIN32_FIND_DATA fData = {0};
	TCHAR tcDirectory[MAX_PATH] = {0}, tcScanFileType[MAX_PATH] = {0};
	int iLength = _tcslen(pStrDirectory);
	if (iLength > MAX_PATH) //���Ŀ¼·��������ʧ��
	{
		return ;
	}
	_tcscpy(tcScanFileType, pStrDirectory);  //����ɨ���ļ���
	
	if (_T('\\') == tcScanFileType[iLength - 1])
		_tcscat(tcScanFileType, _T("*.*"));
	else
		_tcscat(tcScanFileType, _T("\\*.*")); //����ɨ���ļ�������
	HANDLE hFindHandle = FindFirstFile(tcScanFileType, &fData);
	if (hFindHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do 
	{
		if (_T('.') == fData.cFileName[0])
		{   //�����ϲ�Ŀ¼
			continue;
		}
		_tcscpy(tcDirectory, pStrDirectory);  //����ɨ���ļ���
		if ( '\\' != tcDirectory[iLength - 1])
			_tcscat(tcDirectory, _T("\\"));
		
		if (iLength + strlen(fData.cFileName) > MAX_PATH) //�ļ���������ʧ��
		{
			return;
		}
		strcat(tcDirectory, fData.cFileName); //���ļ��������ļ�����
		fData.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY;
		if (FILE_ATTRIBUTE_DIRECTORY == fData.dwFileAttributes)//�ж��Ƿ���Ŀ¼
		{
			ScanDirectory(tcDirectory, funcDoSomething, pThis);  //ö����Ŀ¼
		}
		else
		{
			VIRUS_DEAL_TYPE virusDealRet = funcDoSomething(tcDirectory, pThis);   //Ŀ¼�е��ļ�,ִ����Ӧ������
			TCHAR tcBuffer[128] = {0};
			switch (virusDealRet)
			{
			case NOT_PE:
				_tcscpy(tcBuffer, "��PE�ļ�");
				break;
			case PE_NOT_VIRUS:
				_tcscpy(tcBuffer, "���ǲ���");
				break;
			case INFECT_HEADSTREAM_DELETED:
				_tcscpy(tcBuffer, "��ȾԴ��ɾ���ɹ�");
				break;
			case CARRY_VIRUS_CLEARED:
				_tcscpy(tcBuffer, "����Я���ߣ�����ɹ�");
				break;
			case VIRUS_FAILED:
				_tcscpy(tcBuffer, "������ɱ��ʧ��");
				break;
			}
			printf("%-40s ɨ����: %s\n", tcDirectory, tcBuffer);
		}
	} while (FindNextFile(hFindHandle, &fData));
	FindClose(hFindHandle);
}

//�ж��Ƿ�ΪPE�ṹ
BOOL IsPeFile(TCHAR* pFileName)
{
	if (pFileName == NULL)
	{
		return FALSE;
	}
	//���ָ���ļ����
	HANDLE hFile = CreateFile(pFileName, GENERIC_READ , FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{	
		return FALSE;
	}
	WORD dwTempRead;
	DWORD dwReadInFactSize;
	//��ȡ�ļ���ʼ���ֽ�,��MZͷ
	BOOL bRead = ReadFile(hFile, &dwTempRead, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (dwTempRead != 0x5a4d)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	//�õ�e_lfanew ��Ա��IMAGE_DOS_HEADER �ṹ�е�ƫ��
	DWORD dwSize = offsetof(IMAGE_DOS_HEADER, e_lfanew);
	//���ļ�ָ���ƶ����õ�e_lfanew��Ա
	SetFilePointer(hFile, dwSize, NULL, FILE_BEGIN);
	//��ȡ�õ�e_lfanew��Ա������,Ҳ����PEͷ���ļ��е�ƫ��
	bRead = ReadFile(hFile, &dwTempRead, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	//��ָ���ƶ���PEͷ
	SetFilePointer(hFile, dwTempRead, NULL, FILE_BEGIN);
	//��ȡPE��־
	IMAGE_NT_HEADERS NtHeader = {0};
	bRead = ReadFile(hFile, &NtHeader, sizeof(NtHeader), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(NtHeader) != dwReadInFactSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (NtHeader.Signature != 0x4550)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	//���ļ�����PE��ʽ,����TRUE.
	return TRUE;
}

//��ָ���ļ���ȡ���ڴ�
BOOL GetFileContent(TCHAR *pPath, DWORD &dwSize, PBYTE pMem = NULL)
{
	if (pPath == NULL)
	{
		return FALSE;
	}
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	dwSize = GetFileSize(hFile, NULL);
	if (pMem != NULL)
	{
		DWORD dwRead = 0;
		BOOL bRet = ReadFile(hFile, pMem, dwSize, &dwRead, NULL);
		if (!bRet || dwRead != dwSize)
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}
	CloseHandle(hFile);
	return TRUE;
}

//��ָ���ڴ�����д�뵽�ļ�
BOOL ContentToFile(TCHAR *pPath, PBYTE pMem, DWORD dwSize)
{
	if (pMem == NULL || pPath == NULL)
	{
		return FALSE;
	}
	HANDLE hFile = CreateFile(pPath, GENERIC_WRITE, FILE_SHARE_READ, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	DWORD dwWrite = 0;
	BOOL bRet = WriteFile(hFile, pMem, dwSize, &dwWrite, NULL);
	if (!bRet || dwWrite != dwSize)
	{
		int i = GetLastError();
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

enum VIRUS_TYPE{NOT_VIRUS, INFECT_HEADSTREAM, CARRY_VIRUS_APPEND, 
CARRY_VIRUS_RESOURCE, CARRY_VIRUS_ADDSECTION};

VIRUS_TYPE JudgeFile(TCHAR *pFileName)
{
	VIRUS_TYPE virusType = NOT_VIRUS;
	DWORD dwSize = 0, dwSectionNum = 0, dwSizePe = 0;
	PBYTE pMem = NULL;
	IMAGE_DOS_HEADER *pPeDosHeader = NULL;
	IMAGE_NT_HEADERS *pPeHeader = NULL;
	IMAGE_SECTION_HEADER * pPeSectionHeader = NULL;
	//1����ȡĿ���ļ����ڴ�
	BOOL bRet = GetFileContent(pFileName, dwSize);
	if (!bRet)
		goto Exit ;
	pMem = new BYTE[dwSize];
	if (pMem == NULL)
		goto Exit ;
	bRet = GetFileContent(pFileName, dwSize, pMem);
	if (!bRet)
		goto Exit ;
	
	//PE�ļ���Dosͷ
	pPeDosHeader = (IMAGE_DOS_HEADER *)pMem;
	//PEͷ���ļ��е�ƫ�ƣ�m_pPeDosHeader->e_lfanew
	pPeHeader = (IMAGE_NT_HEADERS *)((BYTE *)pMem + ((IMAGE_DOS_HEADER *)pMem)->e_lfanew);
	//PE�ļ��нڵĸ���
	dwSectionNum = pPeHeader->FileHeader.NumberOfSections;
	//PE�ļ��ڱ�ͷ���ļ��е�ƫ��
	pPeSectionHeader = (IMAGE_SECTION_HEADER *)((char *)&pPeHeader->OptionalHeader + 
		pPeHeader->FileHeader.SizeOfOptionalHeader);
	//�����ЧPE�ĳ���
	dwSizePe = pPeSectionHeader[dwSectionNum-1].SizeOfRawData+
		pPeSectionHeader[dwSectionNum-1].PointerToRawData;
	//2.�����ж�,������и�Ⱦ���,���ǲ���
	if (pPeDosHeader->e_cblp == 0x8F8F && pPeDosHeader->e_cs == 0x3e3e)
	{
		HMODULE hExe = LoadLibrary(pFileName);
		if (hExe == NULL)
		{
			goto Exit;
		}
		HRSRC hRsrc = FindResource(hExe, (LPCTSTR)"EXE", (LPCSTR)"BINARYDATA");
		if (hRsrc != NULL)
		{    //�ҵ���Դ����Ϊ��Դ����ʽЯ����
			virusType = CARRY_VIRUS_RESOURCE;
		}
		else if (dwSize > dwSizePe)
		{//����ļ�ʵ�ʴ�С����PE�ļ���Ч��С�����Ǹ�����������ʽЯ���ߡ�
			virusType = CARRY_VIRUS_RESOURCE;
		}
		else
		{	//�Ǹ�ȾԴ
			virusType = INFECT_HEADSTREAM;
		}
		FreeLibrary(hExe);
	}
	else if (pPeDosHeader->e_ip == 0xfeff && pPeDosHeader->e_cs == 0xfeff)
	{	//������һ������.boy
		if (strcmp((char *)pPeSectionHeader[dwSectionNum-1].Name, ".boy") == 0)
		{
			virusType = CARRY_VIRUS_ADDSECTION;
		}
		else
		{	//�Ǹ�ȾԴ
			virusType = INFECT_HEADSTREAM;
		}
	}
Exit:
	if (pMem != NULL)
	{
		delete []pMem;
		pMem = NULL;
	}
	return virusType;
}

BOOL KillVirus(TCHAR *pFileName, VIRUS_TYPE virusType)
{
	BOOL bRet = FALSE;
	switch (virusType)
	{
	case INFECT_HEADSTREAM:
		{
			bRet = DeleteFile(pFileName);
		}
		break;
	case CARRY_VIRUS_APPEND:
		{	//������Я���߶�ȡ���ڴ�
			DWORD dwSize = 0;
			bRet = GetFileContent(pFileName, dwSize);
			if (!bRet)
				break;
			PBYTE pMem = new BYTE[dwSize];
			if (pMem == NULL)
				break;
			bRet = GetFileContent(pFileName, dwSize, pMem);
			if (!bRet)
			{
				delete []pMem;
				break;
			}
			//PE�ļ���Dosͷ
			IMAGE_DOS_HEADER * pPeDosHeader = (IMAGE_DOS_HEADER *)pMem;
			//PEͷ���ļ��е�ƫ�ƣ�m_pPeDosHeader->e_lfanew
			IMAGE_NT_HEADERS * pPeHeader = (IMAGE_NT_HEADERS *)((BYTE *)pMem + ((IMAGE_DOS_HEADER *)pMem)->e_lfanew);
			//PE�ļ��нڵĸ���
			DWORD dwSectionNum = pPeHeader->FileHeader.NumberOfSections;
			//PE�ļ��ڱ�ͷ���ļ��е�ƫ��
			IMAGE_SECTION_HEADER * pPeSectionHeader = (IMAGE_SECTION_HEADER *)((char *)&pPeHeader->OptionalHeader + 
				pPeHeader->FileHeader.SizeOfOptionalHeader);
			//�����ЧPE�ĳ���
			DWORD dwSizePe = pPeSectionHeader[dwSectionNum-1].SizeOfRawData+
			pPeSectionHeader[dwSectionNum-1].PointerToRawData;
			bRet = DeleteFile(pFileName);
			if (!bRet)
			{
				delete []pMem;
				break;
			}
			//������Я���߸��������е��ļ��ͷų���
			bRet = ContentToFile(pFileName, pMem+dwSizePe, dwSize-dwSizePe);
		}
		break;
	case CARRY_VIRUS_RESOURCE:
		{
			HMODULE hExe = LoadLibrary(pFileName);
			if (hExe == NULL)
			{
				return bRet;
			}
			HRSRC hRsrc = FindResource(hExe, (LPCTSTR)"EXE", (LPCSTR)"BINARYDATA");
			int iError = 0;
			if (hRsrc == NULL)
			{
				return bRet; 
			}
			HGLOBAL hData = LoadResource(hExe, hRsrc);
			if (hData == NULL)
				return bRet;
			LPVOID pMem = LockResource(hData);
			if (pMem == NULL)
				return bRet;
			int iSize = SizeofResource(hExe, hRsrc);
			PBYTE pTemp = new BYTE[iSize];
			if (pTemp == NULL)
				return bRet;
			memcpy(pTemp, pMem, iSize);
			FreeLibrary(hExe);
			bRet = DeleteFile(pFileName);
			//������Я���߸��������е��ļ��ͷų���
			bRet = ContentToFile(pFileName, (PBYTE)pTemp, iSize);
			delete []pTemp;
		}
		break;
	case CARRY_VIRUS_ADDSECTION:
		{
			//������Я���߶�ȡ���ڴ�
			DWORD dwSize = 0;
			bRet = GetFileContent(pFileName, dwSize);
			if (!bRet)
				break;
			PBYTE pMem = new BYTE[dwSize];
			if (pMem == NULL)
				break;
			bRet = GetFileContent(pFileName, dwSize, pMem);
			if (!bRet)
			{
				delete []pMem;
				break;
			}
			//PE�ļ���Dosͷ
			IMAGE_DOS_HEADER * pPeDosHeader = (IMAGE_DOS_HEADER *)pMem;
			//PEͷ���ļ��е�ƫ�ƣ�m_pPeDosHeader->e_lfanew
			IMAGE_NT_HEADERS * pPeHeader = (IMAGE_NT_HEADERS *)((BYTE *)pMem + ((IMAGE_DOS_HEADER *)pMem)->e_lfanew);
			//PE�ļ��нڵĸ���
			DWORD dwSectionNum = pPeHeader->FileHeader.NumberOfSections;
			//PE�ļ��ڱ�ͷ���ļ��е�ƫ��
			IMAGE_SECTION_HEADER * pPeSectionHeader = (IMAGE_SECTION_HEADER *)((char *)&pPeHeader->OptionalHeader + 
				pPeHeader->FileHeader.SizeOfOptionalHeader);
			//�����ЧPE�ĳ���
			DWORD dwSizePe = pPeSectionHeader[dwSectionNum-1].SizeOfRawData+
			pPeSectionHeader[dwSectionNum-1].PointerToRawData;
			int i = 0;
			//������һ�������ڴ��ַ
			PBYTE pLastSecPtr = NULL;
			for (; i < dwSectionNum; i++)
			{
				if (strcmp((char *)pPeSectionHeader[i].Name, ".boy") == 0)
				{
					break;
				}
			}
			if (i != 0)
			{
				pLastSecPtr = (PBYTE)pMem + pPeSectionHeader[i].PointerToRawData;
			}
			//ȡ����ƫ��4�ֽں󱣴��ԭʼ��ڵ�ַ���õ���RVAֵ��
			DWORD dwOep = *(PDWORD((PDWORD)pLastSecPtr+1))-pPeHeader->OptionalHeader.ImageBase;
			//�����Ⱦ���
			*(DWORD *)(pMem + 20) = 0x00000000;
			//�ָ�OEP
			pPeHeader->OptionalHeader.AddressOfEntryPoint = dwOep;
			//���������С��
			pPeHeader->OptionalHeader.SizeOfImage -= pPeSectionHeader[dwSectionNum-1].Misc.VirtualSize;;
			//ɾ�����һ����
			pPeHeader->FileHeader.NumberOfSections--;
			memset(&pPeSectionHeader[dwSectionNum-1], 0, sizeof(IMAGE_SECTION_HEADER));
			//ɾ������Я����
			bRet = DeleteFile(pFileName);
			//����ԭ����ļ�����
			bRet = ContentToFile(pFileName, pMem, dwSize-pPeSectionHeader[dwSectionNum-1].SizeOfRawData);
		}
		break;
	}
	return bRet;
}
VIRUS_DEAL_TYPE DealWith(TCHAR *pFileName, PVOID pPara)
{
	VIRUS_DEAL_TYPE virusDealType = PE_NOT_VIRUS;
	if (IsPeFile(pFileName)) //�����ж��Ƿ�ΪPE�ļ�
	{
		VIRUS_TYPE virusType = JudgeFile(pFileName); //�жϲ�������
		if (virusType != NOT_VIRUS)
		{	//����ǲ��������ɱ��
			BOOL bRet = KillVirus(pFileName, virusType);
			if (bRet && virusType == INFECT_HEADSTREAM)
			{	//�ɹ�ɾ����ȾԴ
				virusDealType = INFECT_HEADSTREAM_DELETED;
			} 
			else if (bRet && (virusType == CARRY_VIRUS_APPEND || 
				virusType == CARRY_VIRUS_RESOURCE || virusType == CARRY_VIRUS_ADDSECTION))
			{	//�ɹ����Я�����еĲ���
				virusDealType = CARRY_VIRUS_CLEARED;
			} 
			else
			{	//ɱ��ʧ��
				virusDealType = VIRUS_FAILED;
			}
		}
	}
	else
	{	//������Ч��PE�ļ�
		virusDealType = NOT_PE;
	}
	return virusDealType;
}

int main(int argc, char* argv[])
{
	ScanDirectory(argv[1], DealWith, NULL);
	return 0;
}

