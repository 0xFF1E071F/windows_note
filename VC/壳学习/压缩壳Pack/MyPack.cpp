// MyPack.cpp: implementation of the CMyPack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma warning(disable:4786)
#pragma warning(disable:4996)
#include "MyPack.h"
#include "Include/CopyResource.h"
#pragma comment(lib, "Lib/CopyResource.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///���캯��
CPack::CPack(BOOL bCompressResource)
: m_dwRelocationTableRva(0)
{
	//��Ա����������
	RtlZeroMemory(m_cFileName, 256);
	//��Աָ���ʼ��Ϊ��ֵ
	m_pMemPointer = NULL; //ָ��PE���غ��ָ��
	m_pSplithData = NULL; //ָ�򱣴渲�����ݵ�ָ��,�����,û�о�Ϊ��
	m_bCompressResource = bCompressResource;
	m_pComSec = NULL;
	m_bLoadSuccess = FALSE;
	m_iComSecNum = 0;
	m_dwRelocationTableRva = 0;
}

//���������ͷ��ڴ�
CPack::~CPack()
{
	if (m_pMemPointer  != NULL)
	{
		GlobalFree(m_pMemPointer  );
		m_pMemPointer  = NULL;
	}
	if (m_pSplithData != NULL)
	{
		GlobalFree(m_pSplithData);
		m_pSplithData = NULL;
	}
	for(int i=0; i<m_iComSecNum; i++)
	{
		free(m_pComSec->lpCompessData);
	}
	if (m_pComSec != NULL)
	{
		delete []m_pComSec;
		m_pComSec = NULL;
	}
}

//�ж��ļ��Ƿ����
BOOL CPack::IsFileExist(char * pFileName)
{
	bool bResult = false;
	WIN32_FIND_DATA findData;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	char *pTempPath = NULL;
	if (pFileName == NULL)
	{
		pTempPath = m_cFileName;
	}
	else
	{
		pTempPath = pFileName;
	}
	if ((hFile = FindFirstFile(pTempPath, &findData)) != INVALID_HANDLE_VALUE)
	{
		bResult = true;
	}
	else
	{	
		if (ERROR_FILE_NOT_FOUND == GetLastError())
		{
			bResult = false;
		}
	}
	FindClose(hFile);
	return bResult;
}

//�ж��ļ��Ƿ�ΪPE��ʽ
BOOL CPack::IsPeFile(TCHAR *pFileName)
{
	BOOL bRet = FALSE;
	char *pTempPath = NULL;
	if (pFileName == NULL)
	{
		pTempPath = m_cFileName;
	}
	else
	{
		pTempPath = pFileName;
	}
	//����ָ���ļ����
	HANDLE hFile = CreateFile(pTempPath, GENERIC_READ , FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{	
		return FALSE;
	}
	WORD dwTempRead, dwSize = 0;
	DWORD dwReadInFactSize;
	//��ȡ�ļ���ʼ���ֽ�,��MZͷ
	BOOL bRead = ReadFile(hFile, &dwTempRead, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		goto OVER;
	}
	if (dwTempRead != 0x5a4d)
	{
		goto OVER;
	}
	//�õ�e_lfanew ��Ա��IMAGE_DOS_HEADER �ṹ�е�ƫ��
	dwSize = offsetof(IMAGE_DOS_HEADER, e_lfanew);
	//���ļ�ָ���ƶ����õ�e_lfanew��Ա
	SetFilePointer(hFile, dwSize, NULL, FILE_BEGIN);
	//��ȡ�õ�e_lfanew��Ա������,Ҳ����PEͷ���ļ��е�ƫ��
	bRead = ReadFile(hFile, &dwTempRead, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		goto OVER;
	}
	//��ָ���ƶ���PEͷ
	SetFilePointer(hFile, dwTempRead, NULL, FILE_BEGIN);
	//��ȡPE��־
	bRead = ReadFile(hFile, &dwTempRead, sizeof(WORD), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(WORD) != dwReadInFactSize)
	{
		goto OVER;
	}
	if (dwTempRead != 0x4550)
	{
		goto OVER;
	}
	bRet = TRUE;
OVER:
	CloseHandle(hFile);
	//���ļ�����PE��ʽ,����TRUE.
	return bRet;	
}

//��dwAlignment ����dwOperateNum ֵ,Ҳ������dwOperateNumΪdwAlignment��������
DWORD CPack::AlignmentNum(DWORD dwOperateNum, DWORD dwAlignment)
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

//����Rva,����Rva�����ڵĽڱ�ָ��,������ڽ���,�򷵻�-1
IMAGE_SECTION_HEADER *CPack::GetSectionPtrByRva(DWORD dwRva)
{
	for (unsigned int i = 0; i < m_iSecNum; i++)
	{
		if (dwRva >= m_pPeSectionHeader[i].VirtualAddress && 
			dwRva < m_pPeSectionHeader[i].VirtualAddress + m_pPeSectionHeader[i].Misc.VirtualSize)
		{
			return &m_pPeSectionHeader[i];
		}
	}
	return NULL;
}

//�������Ŀ¼ĳ����Ϣ,����Ŀ¼��С,��������Ŀ¼RVA.
DWORD CPack::GetDataDirectoryInfo(DWORD dwDataDirectory, DWORD &dwDataDirectoryOffset)
{
	if (dwDataDirectory > 15)
	{
		return -1;
	}
	dwDataDirectoryOffset = m_pPeNtHeader->OptionalHeader.DataDirectory[dwDataDirectory].VirtualAddress;
	return m_pPeNtHeader->OptionalHeader.DataDirectory[dwDataDirectory].Size;
}

//����������
BOOL CPack::DoSpilthData(TCHAR *pFileName)
{
	BOOL bRet = FALSE;
	char *pTempPath = NULL;
	if (pFileName == NULL)
	{
		pTempPath = m_cFileName;
	}
	else
	{
		pTempPath = pFileName;
	}
	//����ָ���ļ����
	HANDLE hFile = CreateFile(pTempPath, GENERIC_READ , FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}		
	//�õ��ļ���ʵ�ʴ�С
	unsigned int iTempFileSize = GetFileSize(hFile, NULL);
	//�õ�PE�ļ�����Ч��С
	unsigned int iTempPeSize = m_pPeSectionHeader[m_iSecNum-1].PointerToRawData + 
		m_pPeSectionHeader[m_iSecNum-1].SizeOfRawData;
	//���PE�ļ���Ч��СС��ʵ�ʴ�С,��ô����������(overlay),Ҫ���渽������
	unsigned int iDescrepancy = iTempFileSize - iTempPeSize;
	if (iDescrepancy <= 0)
	{
		bRet = TRUE;
		goto OVER;
	}
	else
	{
		SetFilePointer(hFile, iTempPeSize, NULL, FILE_BEGIN);
		m_dwSplitDataSize = iDescrepancy;
		m_pSplithData = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iDescrepancy);
		if (m_pSplithData == NULL)
		{
			goto OVER;
		}
		DWORD dwInfactRead;
		BOOL bRead = ReadFile(hFile, m_pSplithData, iDescrepancy, &dwInfactRead, NULL);
		if (!bRead || iDescrepancy != dwInfactRead)
		{
			goto OVER;
		}
		bRet = TRUE;
	}
OVER:
	CloseHandle(hFile);
	return bRet;
}

//����PE���������صķ�ʽ����������ڴ�
BOOL CPack::ReadFileToMem(TCHAR *pFileName)
{
	BOOL bRet = FALSE;
	unsigned int i = 0;
	char *pTempPath = NULL;
	DWORD dwTempSize = 0;
	long lTempSize = 0;
	IMAGE_NT_HEADERS TempNtHeader = {0};
	if (pFileName == NULL)
	{
		pTempPath = m_cFileName;
	}
	else
	{
		pTempPath = pFileName;
	}
	HANDLE hFile = CreateFile(pTempPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//ָ��PE�ṹ�е�Dosͷ��e_lfanew��,�Ӷ��õ�Ntͷ���ļ�ƫ�ơ�
	SetFilePointer(hFile, offsetof(IMAGE_DOS_HEADER, e_lfanew), NULL, FILE_BEGIN);
	DWORD dwTempRead = 0, dwReadInFactSize = 0;
	//��ȡ�õ�Ntͷ�����ļ��е�ƫ�ơ�
	BOOL bRead = ReadFile(hFile, &dwTempRead, sizeof(DWORD), &dwReadInFactSize, NULL);
	m_iDosSize = dwTempRead;
	if (!bRead || sizeof(DWORD) != dwReadInFactSize)
	{
		goto OVER;
	}
	//�ƶ��ļ�ָ�뵽PE�ṹ��Ntͷ��
	SetFilePointer(hFile, dwTempRead, NULL, FILE_BEGIN);
	//��ȡ�õ�Ntͷ������
	bRead = ReadFile(hFile, &TempNtHeader, sizeof(IMAGE_NT_HEADERS), &dwReadInFactSize, NULL);
	if (!bRead || sizeof(IMAGE_NT_HEADERS) != dwReadInFactSize)
	{
		goto OVER;
	}
	//�ڴ��������
	m_iMemAlignment = TempNtHeader.OptionalHeader.SectionAlignment;
	//�ļ���������
	m_iFileAlignment = TempNtHeader.OptionalHeader.FileAlignment;
	//�õ���PE�ļ����ص��ڴ����ܴ�С
	lTempSize = AlignmentNum(TempNtHeader.OptionalHeader.ImageBase, m_iMemAlignment);
	//׼���ڴ棬���水��Windows���ط�ʽ���ص�PE����
	m_pMemPointer = (char *)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, lTempSize);
	if (NULL == m_pMemPointer)
	{ 
		goto OVER;
	}
	//���ļ�ָ��ָ���ļ���
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	//�ڵĸ���
	m_iSecNum = TempNtHeader.FileHeader.NumberOfSections;
	//�ڱ�ͷ�Ĵ�С
	m_iSecSize = sizeof(IMAGE_SECTION_HEADER) * m_iSecNum;
	//NTͷ�Ĵ�С
	m_iNtSize = 4 + sizeof(IMAGE_FILE_HEADER) +
		TempNtHeader.FileHeader.SizeOfOptionalHeader;
	//PEͷ�ܴ�С=Dosͷ+Ntͷ+�ڱ�ͷ
	m_iPeSize = m_iDosSize /*Dos ͷ�Ĵ�С*/ + 
		m_iNtSize  /*Nt ͷ�Ĵ�С*/ + 
		m_iSecSize /*SectionTable ͷ�Ĵ�С*/ ;
	//��ȡPEͷ
	bRead = ReadFile(hFile, m_pMemPointer, m_iPeSize, &dwReadInFactSize, NULL);
	if (!bRead || m_iPeSize != dwReadInFactSize)
	{
		goto OVER;
	}

	//m_pPeNtHeader ָ��PE�ṹ��NTͷ
	m_pPeNtHeader = (PIMAGE_NT_HEADERS)((BYTE *)m_pMemPointer + m_iDosSize);
	//m_pSectionHeader ָ��PE�ṹ�Ľڱ�ͷ
	m_pPeSectionHeader = (PIMAGE_SECTION_HEADER)((BYTE *)m_pMemPointer + m_iDosSize + m_iNtSize);
	m_pPeSectionHeader = (PIMAGE_SECTION_HEADER)((BYTE *)&m_pPeNtHeader->OptionalHeader
		+ m_pPeNtHeader->FileHeader.SizeOfOptionalHeader);
	//ѭ����ȡ������
	//���������нڼ��غ�������ڴ��С
	for (i = 0; i < m_iSecNum; i++)
	{
		//���ļ�ָ��ָ��ڵĿ�ʼ��
		SetFilePointer(hFile, m_pPeSectionHeader[i].PointerToRawData , NULL, FILE_BEGIN);
		//��ȡ�����ڵ�����
		ReadFile(hFile, (BYTE *)m_pMemPointer + AlignmentNum(m_pPeSectionHeader[i].VirtualAddress, m_iMemAlignment),
			m_pPeSectionHeader[i].SizeOfRawData, &dwReadInFactSize, NULL);
		dwTempSize += AlignmentNum(m_pPeSectionHeader[i].Misc.VirtualSize, m_iMemAlignment);
	}
	//������Դ��
	m_Resource = GetAppointSection(IMAGE_DIRECTORY_ENTRY_RESOURCE);
	m_iAllSecMemSize = dwTempSize;
	bRet = TRUE;
OVER:
	CloseHandle(hFile);
	return bRet;
}

//��װ�����ļ����ڴ�ĺ���
ERRORINFO CPack::LoadPE(TCHAR *pFileName)
{
	if (pFileName == NULL)
	{
		return PARAMETER_ERROR;
	}
	//�����ļ���
	_tcscpy(m_cFileName, pFileName);
	//�ж��ļ��Ƿ����
	if (!IsFileExist())
	{
		return FILE_NOT_EXIST;
	}
	//�ж��Ƿ�ΪPE�ļ�,�������,���򷵻ؼ�
	if (!IsPeFile())
	{
		return NOT_PE;
	}
	//��ȡ�ļ����ڴ�
	if (!ReadFileToMem())
	{
		return LOAD_TO_MEM_ERROR;
	}
	//����������
	if (!DoSpilthData())
	{
		return DEAL_SPILTH_ERROR;
	}
	m_bLoadSuccess = TRUE;
	return SUCCESS;
}

//�ж��Ƿ�ΪDLL
BOOL CPack::IsDll()
{
	return IMAGE_FILE_DLL & m_pPeNtHeader->FileHeader.Characteristics
		? TRUE : FALSE;
}


//����ض�λ��Ϣ
void CPack::ClearReloc()
{
	if (!IsDll())  //�����EXE����,����Ҫ����ض�λ����.
	{
		DWORD dwRelocDirectoryOffset;//�����ض�λ����Ϣ��RVA,
		DWORD dwRelocDirectorySize;  //�����ض�λ����Ϣ�Ĵ�С
		//����ض�λĿ¼��Ϣ
		dwRelocDirectorySize = GetDataDirectoryInfo(IMAGE_DIRECTORY_ENTRY_BASERELOC, 
			dwRelocDirectoryOffset);
		//���Ϊ��,��˵���������ض�λ��Ϣ,ֱ�ӷ���
		if (dwRelocDirectoryOffset == NULL || dwRelocDirectorySize == 0)
		{
			return;
		}
		else //�����ض�λ��Ϣ�����
		{
			//��������Ŀ¼��RVA�õ�����Ŀ¼�����ڱ��ָ��
			DWORD dwSec = (DWORD)GetSectionPtrByRva(m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
			//�ض�λĿ¼�����ɽ�
			if (((PIMAGE_SECTION_HEADER)dwSec)->VirtualAddress == dwRelocDirectoryOffset)
			{
				m_iSecNum--;  //�ܽ�����һ
				//���н���ռ�ڴ���Ҫ��ȥ�ض�λ����ռ�ڴ�
				m_iAllSecMemSize -= AlignmentNum(((PIMAGE_SECTION_HEADER)dwSec)->Misc.VirtualSize, m_iMemAlignment);
				//���ض�λ�ڱ�ͷ���
				RtlZeroMemory((BYTE *)dwSec, sizeof(IMAGE_SECTION_HEADER));
			}
			else
			{
				//�������ض�λ������0
				RtlZeroMemory((BYTE *)m_pMemPointer+dwRelocDirectoryOffset, dwRelocDirectorySize);
			}
		}
	}
	//�����ض�λ���RVA
	m_dwRelocationTableRva = m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	//���ض�λĿ¼��С��ƫ����Ϊ0.
	m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = 0;
	m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = 0;
	return;
}

//���ĳĿ¼��Ϣ�����ڱ�,��������ڸ�Ŀ¼���򷵻ؿս�
IMAGE_SECTION_HEADER CPack::GetAppointSection(DWORD dwDataDirectory)
{
	IMAGE_SECTION_HEADER temp = {0};
	DWORD dwResourceDirectoryOffset;//����Ŀ¼��Ϣ��RVA,
	//���Ŀ¼��Ϣ��Rva
	GetDataDirectoryInfo(dwDataDirectory, dwResourceDirectoryOffset);
	//��ø�Rva�����ڵĽڱ�ָ��
	DWORD dwResourcePointer = (DWORD)GetSectionPtrByRva(dwResourceDirectoryOffset);
	for (unsigned int i = 0; i < m_iSecNum; i++)
	{
		//�����ǰ�ڱ�ָ�����Rva�����ڱ�ָ��,��ô�ҵ�
		if (dwResourcePointer == (unsigned long)&m_pPeSectionHeader[i])
		{
			return m_pPeSectionHeader[i];
		}
	}
	return temp;
}

//��APlibѹ�������������ѹ��
PVOID CPack::CompressData(PVOID pSource, long lInLength, OUT long &lOutLenght)
{
	BYTE *packed, *workmem;
	/* allocate memory */
	if ((packed  = (BYTE *) malloc(aP_max_packed_size(lInLength))) == NULL ||
		(workmem = (BYTE *) malloc(aP_workmem_size(lInLength)))    == NULL)
	{
		return NULL;
	}
	lOutLenght = aP_pack(pSource, packed, lInLength, workmem, NULL, NULL);
	if (lOutLenght == APLIB_ERROR)
	{
		return NULL;
	}
	if (NULL != workmem)
	{
		free(workmem);
		workmem = NULL;
	}
	return packed;
}

//���ĳĿ¼��Ϣ�����ڱ���������,��������ڸ�Ŀ¼���򷵻�-1
int CPack::GetSectionPos(DWORD dwDataDirectory)
{
	DWORD dwResourceDirectoryOffset;//����ĳ��Ϣ��RVA,
	//���ĳ��Ϣ��Rva
	GetDataDirectoryInfo(dwDataDirectory, dwResourceDirectoryOffset);
	//��ø�Rva�����ڵĽڱ�ָ��
	DWORD dwResourcePointer = (DWORD)GetSectionPtrByRva(dwResourceDirectoryOffset);
	unsigned int i = 0;
	for (; i < m_iSecNum; i++)
	{
		//�����ǰ�ڱ�ָ�����Rva�����ڱ�ָ��,��ô�ҵ�
		if (dwResourcePointer == (unsigned long)&m_pPeSectionHeader[i])
		{
			break;
		}
	}
	if (i == m_iSecNum)
	{
		return -1;
	}
	return i;
}

//ѹ����
void CPack::CompressSections()
{
	int iCompressRva  = 0;
	//�õ����һ���ڵĵ�ַ,��Ϊѹ����Ҫ�ŵ������ں���.
	int iTempLastSecRva = m_pPeSectionHeader[m_iSecNum-1].VirtualAddress;
	//�õ����һ���ڵĴ�С����Ҫ�ڴ����
	int iTempLastSecSize = AlignmentNum(m_pPeSectionHeader[m_iSecNum-1].Misc.VirtualSize, m_iMemAlignment);
	//ѹ���ڵ���ʼ��ַ
	iCompressRva = iTempLastSecRva + iTempLastSecSize;	


	int iCompressSecNum = m_iSecNum;
	//�õ���Դ���ڽڵ�����
	int iResourcePos = GetSectionPos(IMAGE_DIRECTORY_ENTRY_RESOURCE);
	//�������Դ�����Ҳ�Ҫ��ѹ����Դ�ڣ���ѹ���ڵĸ���Ҫ��һ��
	if (iResourcePos != -1 && !m_bCompressResource)  
	{
		iCompressSecNum -= 1;
	}
	//Ϊ�����ڵ�ѹ����Ϣ׼���ڴ�
	m_pComSec = new CompessSection[iCompressSecNum];
	//�ҵ���Դ������
	int iPos = 0;
	int j = 0;
	for (unsigned int i = 0; i < m_iSecNum; i++)
	{
		//�������Դ������Ҫ��ѹ����ֱ������
		if (i == iResourcePos && !m_bCompressResource)
		{
			iPos++;
			continue;
		}
		else  
		{
			if (m_pPeSectionHeader[iPos].SizeOfRawData == 0)
			{   //����ǿս�������.
				iPos++;
				continue;
			}
			long lCompressSize = 0;
			PVOID pCompressData;
			PVOID pInData = (BYTE *)m_pMemPointer + m_pPeSectionHeader[iPos].VirtualAddress;
			pCompressData = CompressData(pInData, 
				m_pPeSectionHeader[iPos].Misc.VirtualSize,
				lCompressSize);
			//ѹ������ָ��
			m_pComSec[j].lpCompessData = pCompressData;
			//��ѹ����ڴ��ַ
			m_pComSec[j].VA = m_pPeSectionHeader[iPos].VirtualAddress;
			//	pComSec[i].Size = m_pPeSectionHeader[iPos].Misc.VirtualSize;
			//	int iTempComSize = m_pe->AlignmentNum(iCompressRva, m_pe->GetMemAlignment());
			//ѹ�����ݼ��غ���ڴ��ַ
			m_pComSec[j].CompessVA = iCompressRva;
			//ѹ�����ݵĴ�С
			m_pComSec[j].CompessSize = lCompressSize;
			//�õ���һ����ѹ�����ݼ��غ���ڴ��ַ
			iCompressRva += AlignmentNum(lCompressSize, m_iMemAlignment);
			//�ͷ�ѹ�������ڷ�����ڴ�
			iPos++;
			j++;
		}
	}
}


//��������ָ��������Ҫ����ĺ������쵼���
void CPack::BuildImportTable(DWORD dwRva, pImportInfo pImport, PBYTE pMem)
{ ////biaozhi rva ,pImportInfo ,chushihua
	//��������Ա,��һ��:��������ַ����ĵ�ַ
	//���ĸ�dll���ĵ�ַ,�����IAT�ĵ�ַ
	//�ֱ��������ֵ:
	//1.�����Ĵ�С
	DWORD ImportSize = 0;
	//�����ṹ,���뼸��dll���м�+1����Ա��ÿ����Ա5��DWORD
	//��ΪҪ��ȫ���β��
	ImportSize = (pImport->lDllNum+1) * 5 * sizeof(DWORD);

	//IAT�Ĵ�С���ж��ٸ��������ж���+1(ȫ���β)��IAT��Ա��ÿ����Ա��һ��DWORD
	int iTempIAT = 0;
	//2.IAT�Ĵ�С
	DWORD IatSize = 0;
	int i = 0;
	for (; i < pImport->lDllNum; i++)
	{
		iTempIAT += pImport->pDll_info[i].lFuncNum + 1;
	}
	//IAT�Ĵ�С���ڸ���*������Ա��С
	IatSize = iTempIAT * sizeof(DWORD);
	//3.��������ַ�����С����IAT��С
	//���濪ʼ�������������塣
/*
�����ṹ��
Iat1s 00000000 Iat2s 00000000 ...
ImprotTable
Func1Addrs 00000000 Func2Addrs 00000000 ...
Func1Str 00 Func2Str 00 Dll1Str 00 Func1Str 00 Func2Str 00 Dll2Str ......
*/
	DWORD IatRva = dwRva;

	//the address of Func1Addrs Destination:to write IID
	DWORD FuncNameTableRva = dwRva + IatSize + ImportSize;

//there is memory so use these to write
	DWORD DllNameRva = dwRva + 2*IatSize + ImportSize;
	DWORD FuncsStrRva = dwRva + 2*IatSize + ImportSize;

//the following is the Temporary buffer
	BYTE * dwStringPos = pMem + 2*IatSize + ImportSize;//there need to write
	BYTE * dwIatPos = pMem;
	BYTE * dwImportTablePos = pMem + IatSize;
	BYTE * dwFuncNamesRvaPos = pMem + IatSize + ImportSize;
	for (i = 0; i < pImport->lDllNum; i++)
	{
		int j = 0;
		for (; j < pImport->pDll_info[i].lFuncNum; j++)
		{
			dwStringPos += 2;   //�������ú�����ŵ�λ��
			int iTemp = strlen(pImport->pDll_info[i].pFunctionInfo[j].cFunctionName);
			//д�뺯����
			memcpy(dwStringPos, pImport->pDll_info[i].pFunctionInfo[j].cFunctionName, iTemp);
			//д��IAT�ͺ�������ַ����
			memcpy(dwIatPos, &FuncsStrRva, sizeof(DWORD));//д��Iat1s �� ֵΪ Func1Str�ĵ�ַ
			memcpy(dwFuncNamesRvaPos, &FuncsStrRva, sizeof(DWORD));//д��Func1Addrs �� ֵΪ Func1Str�ĵ�ַ
			//��һ��������IAT ->Iat2s
			dwIatPos += sizeof(DWORD);	
			//��һ���������ֵ�Rva ->Func2Addrs
			dwFuncNamesRvaPos += sizeof(DWORD);	
			//��һ���������ֵ�Rva ->Func2Str
			FuncsStrRva += iTemp + 1/*(00��β)*/+ 2/*���*/;
			//��һ��������λ��
			dwStringPos += iTemp + 1/*(00��β)*/;

			DllNameRva += iTemp + 1/*(00��β)*/+ 2/*���*/;
		}
		dwIatPos += sizeof(DWORD);  //һ��ȫ���β
		dwFuncNamesRvaPos += sizeof(DWORD);	//һ��ȫ���β	
		int iTemp = strlen(pImport->pDll_info[i].cDllName) + 1/*(00��β)*/;
		//д��DLL��
		memcpy(dwStringPos, pImport->pDll_info[i].cDllName, iTemp);
		dwStringPos += iTemp; //Խ��DLL��
		FuncsStrRva += iTemp;
		//д�뵼���
		//д�뺯����������ַ
		memcpy(dwImportTablePos, &FuncNameTableRva, sizeof(DWORD));
		//FuncNameTableRva += (j+1) * sizeof(DWORD);
		dwImportTablePos += 3 * sizeof(DWORD);//qian 3 tiao guo
		memcpy(dwImportTablePos, &DllNameRva, sizeof(DWORD));
		//DllNameRva += iTemp;
		dwImportTablePos += sizeof(DWORD);
		memcpy(dwImportTablePos, &IatRva, sizeof(DWORD));//the address of Iat1s write to FirstThunk
		//dwImportTablePos += sizeof(DWORD);
		//IatRva += (j+1) * sizeof(DWORD);
	}
}

//���㵼���Ĵ�С
DWORD CPack::ImportTableSize(pImportInfo pImport)
{
	DWORD ImportSize = 0;
	//�����ṹ,���뼸��dll���м�+1����Ա��ÿ����Ա��DWORD
	//��ΪҪ��ȫ���β��
	ImportSize = (pImport->lDllNum+1) * 5 * sizeof(DWORD);
	//IAT�Ĵ�С���ж��ٸ��������ж���+1(ȫ���β)��IAT��Ա��ÿ����Ա��һ��DWORD
	int iTempIAT = 0;
	int iTempString = 0;
	for (int i = 0; i < pImport->lDllNum; i++)
	{
		iTempIAT += pImport->pDll_info[i].lFuncNum + 1;
		iTempString += strlen(pImport->pDll_info[i].cDllName) + 1/*(00��β)*/;
		for (int j = 0; j < pImport->pDll_info[i].lFuncNum; j++)
		{
			iTempString += strlen(pImport->pDll_info[i].pFunctionInfo[j].cFunctionName) + 1/*(00��β)*/ + 2/*���*/;
		}
	}
	ImportSize += 2 *iTempIAT * sizeof(DWORD) + iTempString;
	//����ַ����Ŀռ��С
	return ImportSize;
}

//��ѹ��������
BYTE decode[] = 
"\x60\x8B\x74\x24\x24\x8B\x7C\x24\x28\xFC\xB2\x80\x8A\x06\x83\xC6\x01\x88\x07\x83\xC7\x01\xBB\x02\x00\x00\x00\x00\xD2\x75\x05\x8A"
"\x16\x46\x10\xD2\x73\xE6\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x73\x4F\x31\xC0\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x0F\x83\xDB\x00"
"\x00\x00\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x11\xC0\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x11\xC0\x00\xD2\x75\x05\x8A\x16\x46\x10"
"\xD2\x11\xC0\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x11\xC0\x74\x06\x89\xFB\x29\xC3\x8A\x03\x88\x07\x47\xBB\x02\x00\x00\x00\xEB\x9B"
"\xB8\x01\x00\x00\x00\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x11\xC0\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x72\xEA\x29\xD8\xBB\x01\x00"
"\x00\x00\x75\x28\xB9\x01\x00\x00\x00\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x11\xC9\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x72\xEA\x56"
"\x89\xFE\x29\xEE\xF3\xA4\x5E\xE9\x4F\xFF\xFF\xFF\x48\xC1\xE0\x08\x8A\x06\x46\x89\xC5\xB9\x01\x00\x00\x00\x00\xD2\x75\x05\x8A\x16"
"\x46\x10\xD2\x11\xC9\x00\xD2\x75\x05\x8A\x16\x46\x10\xD2\x72\xEA\x3D\x00\x7D\x00\x00\x83\xD9\xFF\x3D\x00\x05\x00\x00\x83\xD9\xFF"
"\x3D\x80\x00\x00\x00\x83\xD1\x00\x3D\x80\x00\x00\x00\x83\xD1\x00\x56\x89\xFE\x29\xC6\xF3\xA4\x5E\xE9\xFE\xFE\xFF\xFF\x8A\x06\x46"
"\x31\xC9\xC0\xE8\x01\x74\x17\x83\xD1\x02\x89\xC5\x56\x89\xFE\x29\xC6\xF3\xA4\x5E\xBB\x01\x00\x00\x00\xE9\xDD\xFE\xFF\xFF\x2B\x7C"
"\x24\x28\x89\x7C\x24\x1C\x61\xC3";

//��Ǵ�������
BYTE code[] = 
"\x53\x50\x60\xE8\x00\x00\x00\x00\x58\x8B\xEC\x81\xEC\x00\x01\x00\x00\x89\x45\xF8\x8B\x50\xEC\x85\xD2\x74"
"\x1E\x8B\x4D\x2C\x89\x4D\x98\x83\xFA\x01\x75\x09\xC7\x40\xEC\x02\x00\x00\x00\xEB\x17\x83\xFA\x02\x53\x0F"
"\x84\x9C\x01\x00\x00\x64\x8B\x1D\x30\x00\x00\x00\x8B\x5B\x08\x89\x5D\x98\x8B\x40\xE0\x83\xC0\x20\x8B\x75"
"\xF8\x2B\xF0\x36\x8B\x0E\x85\xC9\x74\x21\x8B\x45\x98\x36\x03\x06\x50\x8B\x45\x98\x36\x03\x46\x04\x50\x8B"
"\x45\xF8\x2D\xEF\x01\x00\x00\xFF\xD0\x83\xC4\x08\x83\xC6\x08\xEB\xD8\x83\xC6\x04\x8B\x06\x89\x45\xA4\x83"
"\xC6\x04\x8B\x45\x98\x8B\x1E\x03\xD8\x8B\x7D\x98\x53\x83\xC3\x0C\x8B\x1B\x85\xDB\x74\x5A\x03\xDF\x53\x8B"
"\x45\xF8\xFF\x90\x5A\xFF\xFF\xFF\x8B\xF0\x5B\x53\x8B\x03\x85\xC0\x75\x05\x8B\x4B\x10\xEB\x02\x8B\x0B\x8B"
"\x5B\x10\x03\xCF\x03\xDF\x8B\x01\x85\xC0\x74\x2A\xA9\x00\x00\x00\x80\x75\x07\x03\xC7\x83\xC0\x02\xEB\x05"
"\x25\xFF\xFF\xFF\x7F\x51\x50\x56\x8B\x45\xF8\xFF\x90\x5E\xFF\xFF\xFF\x89\x03\x59\x83\xC1\x04\x83\xC3\x04"
"\xEB\xD0\x5B\x83\xC3\x14\xEB\x9C\x8D\x04\x24\x50\x6A\x04\x68\x00\x20\x00\x00\x57\x8B\x45\xF8\xFF\x90\x62"
"\xFF\xFF\xFF\x8B\xC7\x8B\x58\x3C\x03\xC3\x8B\x4D\xF8\x8B\x59\xE4\x89\x98\x88\x00\x00\x00\x8B\x59\xE8\x89"
"\x98\x8C\x00\x00\x00\x8B\x4D\xF8\x8B\x59\xF0\x8B\x45\x98\x03\xD8\x89\x5D\xEC\x8B\x45\xA4\x2B\xF8\x85\xFF"
"\x0F\x84\x97\x00\x00\x00\x89\x7D\xD4\x8B\x45\xEC\x83\x38\x00\x0F\x84\x88\x00\x00\x00\x8B\x45\xEC\x83\xC0"
"\x08\x89\x45\xC8\x8B\x45\xEC\x8B\x48\x04\x83\xE9\x08\xD1\xE9\x89\x4D\xBC\xC7\x45\x8C\x00\x00\x00\x00\xEB"
"\x09\x8B\x45\x8C\x83\xC0\x01\x89\x45\x8C\x8B\x45\x8C\x3B\x45\xBC\x73\x46\x8B\x45\x8C\x8B\x4D\xC8\x0F\xB7"
"\x14\x41\x89\x55\xB0\x8B\x45\xB0\x25\x00\xF0\x00\x00\x3D\x00\x30\x00\x00\x74\x02\xEB\xD1\x8B\x45\xB0\x25"
"\xFF\x0F\x00\x00\x89\x45\xB0\x8B\x45\xEC\x8B\x4D\x98\x03\x08\x03\x4D\xB0\x89\x4D\xB0\x8B\x45\xB0\x8B\x08"
"\x03\x4D\xD4\x8B\x55\xB0\x89\x0A\xEB\xA9\x8B\x45\xEC\x8B\x4D\xEC\x03\x48\x04\x89\x4D\xEC\xE9\x6C\xFF\xFF"
"\xFF\x5B\x8B\x45\xF8\x89\x45\x20\x8B\x5D\x98\x89\x5D\x24\x81\xC4\x00\x01\x00\x00\x61\x58\x5B\x03\x58\xF4"
"\xFF\xE3";
//���������ѹ�����Ľṹ��
typedef struct Data 
{
	PBYTE pOut; //��ѹ���ŵ�λ��
	PBYTE pIn;  //��ѹ��������λ��
}SECDATA;

//���ɼӿ��ļ�
ERRORINFO CPack::GetFile(TCHAR *pPathName)
{
	//һ���Ƿ�������ɼӿ��ļ�
	if(!m_bLoadSuccess)
	{
		return NOT_LOAD;
	}
	//�ж��ļ��Ƿ��Ѿ��ӿ�
	if (*(PBYTE)((PBYTE)m_pMemPointer + 3) == (BYTE)0x1)
	{
		return ALREADY_PACKED;
	}
	else
	{
		//д��ӿǱ��
		*(PBYTE)((PBYTE)m_pMemPointer + 3) = 0x1;
	}
	//���������ض�λ���ݣ�EXE����Ҫ���������DLL��Ҫ����ض�λĿ¼���Լ������ض�λ��
	ClearReloc();
	//����ѹ�������ڡ�
	CompressSections();
	//�ġ�����Ŀ���ļ�����д����ʱPEͷ��
	HANDLE hFile = CreateFile(pPathName, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{
		return CREATE_TARGET_FILE_ERROR;
	}
	//�õ�PEͷ�Ĵ�С
	DWORD dwPeHeaderSize = 0;
	dwPeHeaderSize = AlignmentNum(m_iPeSize, m_iMemAlignment);
	DWORD dwWrite = 0;
	//д��PEͷ
	WriteFile(hFile, m_pMemPointer, dwPeHeaderSize, &dwWrite, NULL);
	//���ڼ���ӿǺ��ļ��ľ����С�ı���
	DWORD dwImageSize = 0;
	//��PEͷ�Ĵ�С���뵽���㾵���С�ı�����
	dwImageSize += dwPeHeaderSize;
	//��.�����ո�д���PEͷ�еĽڱ�ͷ
	//׼��ռλ����Ϣ
	char *pSectionName = "WBCPACK";
	IMAGE_SECTION_HEADER tempSec = {0};
	strcpy((char *)tempSec.Name, pSectionName);
	//ռλ�ڵ��ڴ���ʼRVA,����PEͷ֮��
	tempSec.VirtualAddress = dwPeHeaderSize;
	if (m_Resource.PointerToRawData == NULL || m_bCompressResource)
	{
		tempSec.Misc.VirtualSize = m_iAllSecMemSize;
	}
	else
	{
		tempSec.Misc.VirtualSize = m_iAllSecMemSize - 
			AlignmentNum(m_Resource.Misc.VirtualSize, m_iMemAlignment);
	}
	//��ռλ�ڵ��ڴ��С���뵽���㾵���С�ı�����
	dwImageSize += tempSec.Misc.VirtualSize;
	tempSec.PointerToRawData = dwPeHeaderSize;
	tempSec.SizeOfRawData = 0;
	tempSec.Characteristics = 0x80000000; //������Ϊ��д
	//���ļ�ָ���ƶ����ڱ�ͷ����ʼ��,��PE�ṹNTͷ��β��
	DWORD dwMove = 0;
	dwMove = SetFilePointer(hFile, m_iNtSize + m_iDosSize, NULL, FILE_BEGIN);
	//д��ڱ�ͷ�ĵ�һ����ͷ,��ռλ��ͷ 
	WriteFile(hFile, &tempSec, sizeof(tempSec), &dwWrite, NULL);
	DWORD dwFileTemp = 0; //���ڼ�¼��һ���ڵ��ļ�ƫ�Ƶı���
	dwFileTemp = dwPeHeaderSize; 
	int iPressSecNum =  m_iSecNum; //��¼ʵ�ʱ�ѹ���Ľڵĸ���
	//���������Դ�����Ҳ�Ҫ��ѹ����Դ�ڣ������Դ��д�ڵڶ�����
	if (m_Resource.VirtualAddress != 0 && !m_bCompressResource)  
	{
		//д��ڱ�ͷ�ĵڶ�����ͷ,����Դͷ
		strcpy((char *)tempSec.Name, (char *)m_Resource.Name);
		tempSec.VirtualAddress = m_Resource.VirtualAddress;
		tempSec.Misc.VirtualSize = m_Resource.Misc.VirtualSize;
		tempSec.PointerToRawData = dwPeHeaderSize;
		tempSec.SizeOfRawData = m_Resource.SizeOfRawData;
		tempSec.Characteristics = m_Resource.Characteristics;
		dwImageSize += AlignmentNum(tempSec.Misc.VirtualSize,m_iMemAlignment);
		//д����Դ��ͷ
		WriteFile(hFile, &tempSec, sizeof(tempSec), &dwWrite, NULL);
		dwFileTemp += AlignmentNum(m_Resource.SizeOfRawData, m_iFileAlignment);
		iPressSecNum -= 1;
	}
	//д�����ѹ���ڵĽ�ͷ
	int i = 0;
	for (; i < iPressSecNum; i++)
	{
		strcpy((char *)tempSec.Name, pSectionName);
		tempSec.VirtualAddress = m_pComSec[i].CompessVA;
		tempSec.Misc.VirtualSize = m_pComSec[i].CompessSize;
		tempSec.PointerToRawData = dwFileTemp;
		tempSec.SizeOfRawData = AlignmentNum(m_pComSec[i].CompessSize, 
			m_iFileAlignment);
		tempSec.Characteristics = 0x80000000; //������Ϊ��д
		dwImageSize += AlignmentNum(tempSec.Misc.VirtualSize,m_iMemAlignment);
		WriteFile(hFile, &tempSec, sizeof(tempSec), &dwWrite, NULL);
		dwFileTemp += AlignmentNum(m_pComSec[i].CompessSize, m_iFileAlignment);
	}
	//��.д��������Ϣ�ڵĽ�ͷ
	/*
	���ýڵĳ��ȣ�
	1.��ѹ����		  PressSecNum * 8 + 4  �ֽ�
	2.��ַ			  4 �ֽ�
	3.�����RVA       4 �ֽ�
	4.��ѹ����        0x149 �ֽ�           DecompressCodeSize
	5.�����		  0x**  �ֽ�           ImportTableSize
	6.��Գ���        4 �ֽ�               RelativeNum
	7.��Դ��Ϣ        8 �ֽ�               ResourceInfoSize 
	8.DLL��Ϣ         4�ֽ�
	9.�ض�λ��RVA     4�ֽ�
	10.oep(RVA)       4 �ֽ�               OepSize
	11.��Ǵ���	      0x** �ֽ�						
	*/
	//ͳ�����ýڵĳ���
	DWORD dwConfigSize = 0;
	//������ǵ����
	pImportInfo pInfo = new IMPORT_INFO;
	pInfo->lDllNum = 1;
	pInfo->pDll_info = new DLL_INFO[1];
	pInfo->pDll_info[0].pFunctionInfo = new FUNC_INFO[3];
	strcpy(pInfo->pDll_info[0].cDllName, "kernel32.dll");
	pInfo->pDll_info[0].lFuncNum = 3;
	strcpy(pInfo->pDll_info[0].pFunctionInfo[0].cFunctionName, "LoadLibraryA");
	strcpy(pInfo->pDll_info[0].pFunctionInfo[1].cFunctionName, "GetProcAddress");
	strcpy(pInfo->pDll_info[0].pFunctionInfo[2].cFunctionName, "VirtualProtect");
	DWORD dwRva = (iPressSecNum + 1) * m_iMemAlignment;
	//д�����ÿռ��ͷ
	//�õ����һ��ѹ����Rva
	DWORD dwLastCompessVA = m_pComSec[iPressSecNum-1].CompessVA;
	//���һ��ѹ����Ĵ�С
	DWORD dwLastCompessSize = m_pComSec[iPressSecNum-1].CompessSize;
	DWORD dwLastSectionMemSize = dwLastCompessVA + AlignmentNum(dwLastCompessSize, 
		m_iMemAlignment);
	DWORD dwConfigSecStart = dwLastSectionMemSize;
	DWORD dwSecParamSize = sizeof(SECDATA)*(iPressSecNum);
	//���㵼���Ĵ�С
	int iImportTableSize = ImportTableSize(pInfo);	
	dwConfigSize = dwSecParamSize + 4/*��ѹ����������־*/ + 4/*��ַ*/ + 
		4/*�����RVA*/ + sizeof(decode)/*��ѹ�����С*/ + iImportTableSize
		+ 4/*���ֵ*/ + 8/*��Դ��Ϣ*/+ 4/*DLL��Ϣ*/ + 4/*�ض�λ��RVA*/+ 4/*OEP*/+
		sizeof(code)/*��Ǵ����С*/;
	strcpy((char *)tempSec.Name, pSectionName);
	tempSec.VirtualAddress = dwLastSectionMemSize;
	tempSec.Misc.VirtualSize = dwConfigSize;  //?
	tempSec.PointerToRawData = dwFileTemp; 
	tempSec.SizeOfRawData = dwConfigSize;    //��Ϊ�����һ���ڣ����Բ��ö���
	tempSec.Characteristics = 0xE0000020;   //���ÿռ��д
	dwImageSize += AlignmentNum(tempSec.Misc.VirtualSize,m_iMemAlignment);
	WriteFile(hFile, &tempSec, sizeof(tempSec), &dwWrite, NULL);
	//��.д����������ݡ�
	//���ļ�ָ���ƶ����ڿ�ʼ��
	dwMove = SetFilePointer(hFile, dwPeHeaderSize, NULL, FILE_BEGIN);

	//���������Դ��,������Դ��ûҪ��ѹ��,��ô����Դ������д��PEͷ��
	DWORD dwMoveFact = dwPeHeaderSize;
	if (m_Resource.SizeOfRawData != NULL && !m_bCompressResource)
	{
		//д����Դ��
		WriteFile(hFile, (PVOID)((BYTE *)m_pMemPointer + m_Resource.VirtualAddress), 
			m_Resource.SizeOfRawData, &dwWrite, NULL);
		//��Դ�ڵ��ļ������С
		DWORD dwResourceSize = AlignmentNum(m_Resource.SizeOfRawData,
			m_iFileAlignment);
		dwMove = SetFilePointer(hFile, dwPeHeaderSize + dwResourceSize, NULL, FILE_BEGIN);
		//�ļ�ͷ+��Դ�ڵ��ļ������С
		dwMoveFact += dwResourceSize;
	}
	//д�����ѹ��������
	for (i = 0; i < iPressSecNum; i++)
	{	
		WriteFile(hFile, m_pComSec[i].lpCompessData, 
			m_pComSec[i].CompessSize, &dwWrite, NULL);
		dwMoveFact += AlignmentNum(m_pComSec[i].CompessSize,
			m_iFileAlignment);
		dwMove = SetFilePointer(hFile, dwMoveFact, NULL, FILE_BEGIN);
	}
	//
	//��.������һ����Ҳ�������ý�
	/*
	���ýڵĳ��ȣ�
	1.��ѹ����		  PressSecNum * 8 + 4  �ֽ�
	2.��ַ			  4 �ֽ�
	3.�����RVA       4 �ֽ�
	4.��ѹ����        0x149 �ֽ�           DecompressCodeSize
	5.�����		  0x**  �ֽ�           ImportTableSize
	6.��Գ���        4 �ֽ�               RelativeNum
	7.��Դ��Ϣ        8 �ֽ�               ResourceInfoSize 
	8.DLL��Ϣ         4�ֽ�
	9.�ض�λ��RVA     4�ֽ�
	10.oep(RVA)       4 �ֽ�               OepSize
	11.��Ǵ���	      0x** �ֽ�						
	*/
	//���ڼ�¼���ý�ǰ�岿�ֵ��ܳ��ȵı���
	DWORD dwSize = 0;
	//Ϊ��ѹ���������ڴ�,���ҽ��и�ֵ.
	SECDATA *pSecDatas = new SECDATA[iPressSecNum];
	for (i = 0; i < iPressSecNum; i++)
	{
		pSecDatas[i].pIn = (PBYTE)m_pComSec[i].CompessVA;
		pSecDatas[i].pOut = (PBYTE)m_pComSec[i].VA;
	}
	//д���ѹʱ��Ҫ������ָ�뼰��ŵ�ַ
	WriteFile(hFile, pSecDatas, 
		sizeof(SECDATA)*(iPressSecNum), &dwWrite, NULL);
	if (pSecDatas)
	{
		delete []pSecDatas;
	}
	dwSize += sizeof(SECDATA)*(iPressSecNum);
	//д���ѹ����������־
	DWORD dwOverFlat = 0;
	WriteFile(hFile, &dwOverFlat, 4, &dwWrite, NULL);
	dwSize += 4;
	DWORD dwImageBase = m_pPeNtHeader->OptionalHeader.ImageBase;
	//д�����ȼ��ػ�ַ
	WriteFile(hFile, &dwImageBase, 4, &dwWrite, NULL);
	dwSize += 4;
	DWORD dwImportVa = m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	//д�뵼����ַ
	WriteFile(hFile, &dwImportVa, 4, &dwWrite, NULL);
	dwSize += 4;
	//д���ѹ������
	WriteFile(hFile, &decode, sizeof(decode), &dwWrite, NULL);
	dwSize += sizeof(decode);

	//������ǳ���ʹ�õĵ����
	BYTE *pImportTable = new BYTE[iImportTableSize];
	memset(pImportTable, 0, iImportTableSize);
	DWORD dwImportRVA = dwConfigSecStart  + dwSecParamSize + 4/*��ѹ����������־*/ + 4/*��ַ*/ + 
		4/*�����RVA*/ + sizeof(decode)/*��ѹ�����С*/;
	BuildImportTable(dwImportRVA ,pInfo, pImportTable);//biaozhi rva ,pImportInfo ,chushihua
	//д�뵼��� 
	WriteFile(hFile, pImportTable, iImportTableSize, &dwWrite, NULL);
	dwSize+= iImportTableSize;//���ڼ�¼���ý�ǰ�岿�ֵ��ܳ��ȵı���
	delete []pImportTable;
	//д����Գ���
	WriteFile(hFile, &dwSize, 4, &dwWrite, NULL);
	//д����Դ��Ϣ
	//********************* new д��ԭʼ��ԴĿ¼RVA����ԴĿ¼��С **************
	DWORD dwResRva = m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	WriteFile(hFile, &dwResRva, 4, &dwWrite, NULL);
	DWORD dwResSize = m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size;
	WriteFile(hFile, &dwResSize, 4, &dwWrite, NULL);
	//********************* new д��ԭʼ��ԴĿ¼RVA����ԴĿ¼��С **************
	//д��DLL��־
	DWORD dwDllFlag = 0;
	if(IsDll())
	{
		dwDllFlag = 1;
	}
	WriteFile(hFile, &dwDllFlag, 4, &dwWrite, NULL);
	//д���ض�λĿ¼RVA
	WriteFile(hFile, &m_dwRelocationTableRva, 4, &dwWrite, NULL);
	//дoep
	DWORD dwOep = m_pPeNtHeader->OptionalHeader.AddressOfEntryPoint;
	WriteFile(hFile, &dwOep, 4, &dwWrite, NULL);
	//д����Ǵ���
	WriteFile(hFile, &code, sizeof(code), &dwWrite, NULL);
	//��.����peͷ
	//�õ��ӿǺ�����
	DWORD dwEntryPoint = dwConfigSecStart +  dwSecParamSize + 4/*��ѹ����������־*/ + 4/*��ַ*/ + 
		4/*�����RVA*/ + sizeof(decode)/*��ѹ�����С*/ + iImportTableSize
		+ 4/*���ֵ*/ + 8/*��Դ��Ϣ*/+ 4/*DLL��Ϣ*/ + 4/*�ض�λ��RVA*/ + 4/*OEP*/;
	//��������ڸ�Ϊ��Ǵ�������
	m_pPeNtHeader->OptionalHeader.AddressOfEntryPoint = dwEntryPoint;
	//�õ��µ�����RVA��ַ
	int iIatSize = 0;
	for (i=0; i < pInfo->lDllNum; i++)
	{
		iIatSize += pInfo->pDll_info[i].lFuncNum + 1;
	}
	DWORD dwImportAdd = dwConfigSecStart  + dwSecParamSize + 4/*��ѹ����������־*/ + 4/*��ַ*/ + 
		4/*�����RVA*/ + sizeof(decode)/*��ѹ�����С*/+ iIatSize * sizeof(DWORD);//IID rva
	//���¼ӿǺ����ĵ�����ַ
	m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = dwImportAdd;
	m_pPeNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = iImportTableSize;

	//���½ڸ���
	m_pPeNtHeader->FileHeader.NumberOfSections = m_iSecNum + 2;
	//���¾����С
	m_pPeNtHeader->OptionalHeader.SizeOfImage = dwImageSize;
	dwMove = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	//�����º��PEͷд���ļ�
	WriteFile(hFile, m_pMemPointer, m_iDosSize + m_iNtSize, &dwWrite, NULL);
	CloseHandle(hFile);
	if(m_bCompressResource && !IsDll())
	{
		UpdateIconAndVersion(m_cFileName, pPathName);
	}
	//д�븽������
	if (m_pSplithData)
	{
		hFile = CreateFile(pPathName, GENERIC_WRITE, 0, NULL, 
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		dwMove = SetFilePointer(hFile, 0, NULL, FILE_END);
		WriteFile(hFile, m_pSplithData, m_dwSplitDataSize, &dwWrite, NULL);
		CloseHandle(hFile);
	}
	for (i=0; i<pInfo->lDllNum; i++)
	{
		delete []pInfo->pDll_info[i].pFunctionInfo;
	}
	delete []pInfo->pDll_info;
	delete []pInfo;
	return SUCCESS;
}
