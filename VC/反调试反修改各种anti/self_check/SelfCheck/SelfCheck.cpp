#include "SelfCheck.h"
#include <tchar.h>

SelfCheck::SelfCheck(void)
{
}

SelfCheck::~SelfCheck(void)
{
}

//ԭ������������һ���ļ���С���ٻ�ȡ�ļ���С�����������ȣ����ļ�û�б��޸�
//ͨ���������ȼ�һ���ǣ��õ��ļ���С������dwRealSize��С���ٻ�ȡ�ļ���С
//��ʱ�������������Σ���С�϶��ᷢ���仯����ͽ���У��
BOOL SelfCheck::IsFileModified_By_FileSize()
{
	BOOL bRet = FALSE;
	DWORD dwSize = 0;

	//�ļ���ǰ��С
	DWORD dwCurrentFileSize = 0;

	//�ļ�ʵ�ʴ�С��һ���ǼӿǺ�Ĵ�С��
	DWORD dwRealSize = 5632;
	TCHAR szFileName[MAX_PATH];

	//��ȡ�����ļ���
	GetModuleFileName(NULL, szFileName, MAX_PATH);

	//���ļ�
	HANDLE hFile = CreateFile(
					szFileName, 
					GENERIC_READ, 
					FILE_SHARE_READ, 
					NULL, 
					OPEN_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	//��ȡ�ļ���С
	dwCurrentFileSize = GetFileSize(hFile, NULL);

	if (dwCurrentFileSize == dwRealSize)
	{
		bRet = FALSE;
	}
	else
	{
		bRet = TRUE;
	}
	return bRet;
}



BOOL IsFileModified_By_CRC();
DWORD CRC32(BYTE*,DWORD);

// ԭ������������add2crc32.exe���߽�CRC32У��ֵд�뵽�ļ���PEͷ��ǰ�ĸ��ֽ�
// ���žͶ�ȡPEͷ��ǰ�ĸ��ֽڣ�������߲���Ⱦͱ�ʾ�ļ����޸��ˣ����ַ����Ƚ�׼ȷ�㣬������ȻҲ�ǱȽϺù��ġ�
BOOL SelfCheck::IsFileModified_By_CRC32()
{
	TCHAR szFileName[MAX_PATH] = {0};

	// ��õ�ǰ�ļ���
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader;

	// ���ļ�
	HANDLE hFile = CreateFile(
		szFileName, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//��ȡ�ļ���С
	DWORD dwCurrentFileSize = 0;
	dwCurrentFileSize = GetFileSize(hFile, NULL);
	if (dwCurrentFileSize == 0xFFFFFFFF)
	{
		return FALSE;
	}
		
	
	DWORD dwReaded = 0;
	TCHAR *pBuffer = new TCHAR[dwCurrentFileSize];
	DWORD dwOriginalCRC32 = 0; // ԭʼCRC32ֵ

	//��ȡӲ���������ļ�
	ReadFile(hFile, pBuffer, dwCurrentFileSize, &dwReaded, NULL);

	pDosHeader = (PIMAGE_DOS_HEADER)pBuffer;
	pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	//��ȡCRCֵ�������Զ�����NtͷҲ���ǡ�PE00��ǰ�ĸ��ֽ�
	dwOriginalCRC32 = *(DWORD*)((DWORD)pNtHeader - 4);
	
	//�ļ���С��ȥNtͷǰ��Ĵ�С��Ҳ���Ǵ�Nt��ʼ
	dwCurrentFileSize = dwCurrentFileSize - pDosHeader->e_lfanew;
	if (dwOriginalCRC32 == CRC32((BYTE*)pNtHeader, dwCurrentFileSize))
	{
		return FALSE;	//û�б��޸�
	}
	else
	{
		return TRUE;	//�Ѿ����޸�
	}
	CloseHandle(hFile);
	delete[] pBuffer;
	


}
////////////////////////////////////////////////////////////////
// �����ַ�����CRC32ֵ
// ������������CRC32ֵ�ַ������׵�ַ�ʹ�С
// ����ֵ: ����CRC32ֵ

DWORD CRC32(BYTE* ptr,DWORD Size)
{

	DWORD crcTable[256],crcTmp1;

	//��̬����CRC-32��
	for (int i=0; i<256; i++)
	{
		crcTmp1 = i;
		for (int j=8; j>0; j--)
		{
			if (crcTmp1&1) crcTmp1 = (crcTmp1 >> 1) ^ 0xEDB88320L;
			else crcTmp1 >>= 1;
		}

		crcTable[i] = crcTmp1;
	}
	//����CRC32ֵ
	DWORD crcTmp2= 0xFFFFFFFF;
	while(Size--)
	{
		crcTmp2 = ((crcTmp2>>8) & 0x00FFFFFF) ^ crcTable[ (crcTmp2^(*ptr)) & 0xFF ];
		ptr++;
	}

	return (crcTmp2^0xFFFFFFFF);
}
