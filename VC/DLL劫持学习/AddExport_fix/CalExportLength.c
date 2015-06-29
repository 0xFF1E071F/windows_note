#include <WINDOWS.H>
#include "AddExport.h"

#define NFIND_ORD 0x0;
#define FIND_ORD 0x1;

/*
������
lpFileָ���ڴ�ӳ���ļ�
dwTransitNameLenΪ��תDLL���ֵĳ���
���ܣ�ͨ�������lpFile����DLL�ļ�����������Ҫ�������ַ����ȣ�������Ҫ����ת�����ַ���Ŀ
����ֵ������NULLΪ�������򷵻����ļ��������Ҫ�����Ľڴ�С��0x200���룩
*/

DWORD CalExportLength(LPVOID lpFile,DWORD dwTransitNameLen)
{

	PIMAGE_DOS_HEADER lpDosHead;
	PIMAGE_NT_HEADERS32 lpNTHead;
	PIMAGE_OPTIONAL_HEADER32 lpOptionHead;
	PIMAGE_DATA_DIRECTORY lpDataDir;
	PIMAGE_EXPORT_DIRECTORY lpExport;
	DWORD lpName;
	DWORD dwNameLen;
	DWORD dwAddrLen;	//������ڵ�ַRVA+��������ַRVA+���������ܳ���
	DWORD *lpNameRva;
	DWORD dwNameStringLen;
	DWORD dwTemp;
	WORD *lpOrdinals;
	int i;
	int n;
	int m;
	int f = NFIND_ORD;	//�Ƿ��ҵ�������ŵı�־

	lpDosHead = (PIMAGE_DOS_HEADER)lpFile;
	lpNTHead = (PIMAGE_NT_HEADERS32)((byte *)lpDosHead+lpDosHead->e_lfanew);
	lpOptionHead = (PIMAGE_OPTIONAL_HEADER32)((byte *)lpNTHead+
					sizeof (DWORD)+
					sizeof (IMAGE_FILE_HEADER));
	//�õ�����Ŀ¼�ĵ�һ����Ա��ַ��Ҳ���ǵ�����Ŀ¼
	lpDataDir = (PIMAGE_DATA_DIRECTORY)(&lpOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	//�õ����������ڴ��еĵ�ַ
	lpExport = (PIMAGE_EXPORT_DIRECTORY)RvaToOffset(lpDosHead,lpDataDir->VirtualAddress);
	//��ʼ������������������Ҫ�õ����ڴ泤�ȣ�������ӵ���ת�������ڴ泤��
	lpName = (DWORD)RvaToOffset(lpDosHead,lpExport->Name);
	dwNameLen = lstrlenA(lpName);
	dwNameLen++;
	dwAddrLen = ((lpExport->NumberOfFunctions)*sizeof(DWORD)+	//��ź�����ڵ�ַ����Ҫ���ܳ���
				(lpExport->NumberOfNames)*sizeof(DWORD)+		//��ź�������ַ����Ҫ���ܳ���
				(lpExport->NumberOfNames)*sizeof(WORD));		//��ź�����������Ҫ���ܳ���
	//���������㺯�����ַ�������Ҫ���ܳ���+��ת�����ַ�������

	dwNameStringLen = 0;
	dwTemp = 0;
	
	lpNameRva = (DWORD *)RvaToOffset(lpDosHead,lpExport->AddressOfNames);

	for (n = lpExport->NumberOfNames,i = 0;n != 0;n--,i++)
	{
		dwTemp = lstrlenA((DWORD)RvaToOffset(lpDosHead,lpNameRva[i]));
		dwTemp++;	//ÿ���ַ�����0��β�����ټ�1
		dwTemp *= 2;	//��ת������Ҫ��ͬ��С�������ַ���
		//dwTemp += sizeof TEXT("Bommon.");	//��ת������Ҫ��ÿ���ַ���ǰ�����DLL����
		dwTemp += dwTransitNameLen;
		dwTemp++;	//����.�ĳ���
		dwNameStringLen += dwTemp;
	}
	//����ֻ������Ž��е����ĺ�������ת�����ַ������ȣ�DLLName.#19
	n = lpExport->NumberOfFunctions;
	n -= lpExport->NumberOfNames;
	lpOrdinals = (WORD *)RvaToOffset(lpDosHead,lpExport->AddressOfNameOrdinals);

	if (!n)
	{
		dwNameStringLen += dwAddrLen;
		dwNameStringLen += dwNameLen;
		dwNameStringLen += 0x1FF;
		dwNameStringLen &= 0xFFFFFE00;
		return dwNameStringLen;
	}

	for (i=0;n!=0;i++)
	{
		for (m=0;m < lpExport->NumberOfNames;m++)
		{
			if (i==lpOrdinals[m])
			{
				f = FIND_ORD;
				break;
			}
		}
		if (!f)
		{
			//dwNameStringLen += sizeof TEXT("Bommon.#");
			dwNameStringLen += (dwTransitNameLen+2);
			dwNameStringLen += sizeof (DWORD64);
			dwNameStringLen++;
			n--;
		}
		f = NFIND_ORD;
	}
	dwNameStringLen += dwAddrLen;
	dwNameStringLen += dwNameLen;
	dwNameStringLen += 0x1FF;
	dwNameStringLen &= 0xFFFFFE00;
	return dwNameStringLen;

}
