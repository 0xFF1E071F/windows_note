#include <WINDOWS.H>
#include "AddExport.h"

/*
������szDllָ����Ҫ�������һ���ڵ�DLL�ļ�����dwLengthΪ��Ҫ������ݵĳ���
���ܣ�ΪszDll�����һ��������dwLength��С����
����ֵ������NULLΪ�������򷵻���������һ���ڵ��������ֵ�Rva��ַ
*/

DWORD WINAPI ExpandLastSection(PTCHAR lpszDll,DWORD dwLength)
{
	STFILEMAP stFileMap;
	LPVOID lpMapFile;
	DWORD dwFileAlignment;	//��Ҫ�����ڵ�Ŀ���ļ�����ֵ
	DWORD dwSectionAlignment;	//��Ҫ������Ŀ���ļ�ҳ����ֵ
	DWORD dwAddLen;	//����FileAlignment�����������ڴ�С
	PIMAGE_DOS_HEADER lpDosHead;	//ָ��Dosͷָ��
	PIMAGE_NT_HEADERS32 lpNTHead;	//ָ��NTͷָ��
	DWORD dwNumeOfSection;			//����Ŀ
	PIMAGE_OPTIONAL_HEADER32 lpOptionHead;	//ָ���ѡͷ
	PIMAGE_SECTION_HEADER lpSectionHead;	//ָ��ڱ�ͷ
	int i;
	HANDLE hFile;
	DWORD dwOldVirtualSize;
	DWORD dwOldSizeOfRawData;
	DWORD dwRet;
	DWORD dwRealAddLen;	//DLL�ļ���ʵ���ӵĴ�С

	//��ʼ���ṹ
	stFileMap.szDll = lpszDll;
	stFileMap.hFile = 0;
	stFileMap.hFileMap =0;
	stFileMap.dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
	stFileMap.dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	stFileMap.dwCreationDisposition = OPEN_EXISTING;
	stFileMap.flProtect = PAGE_READWRITE;
	stFileMap.dwDesiredAccessMap = FILE_MAP_READ | FILE_MAP_WRITE;
	lpMapFile = MapFile(&stFileMap);
	if (!lpMapFile)
	{
		MessageBox(NULL,TEXT ("����Ҫ����ڵ�DLL�ļ�ʧ��!"),TEXT ("Error!"),MB_OK);
		return 0;
	}
	//�õ��������ݵ�DLL�ļ���FileAlignment��ҳ����ֵ
	lpDosHead = lpMapFile;
	lpNTHead = (byte *)lpDosHead+lpDosHead->e_lfanew;
	lpOptionHead =	((byte *)lpNTHead+
					sizeof (DWORD)+
					sizeof (IMAGE_FILE_HEADER));
	dwFileAlignment = lpOptionHead->FileAlignment;
	dwSectionAlignment = lpOptionHead->SectionAlignment;
	//�õ�Ŀ���ļ�����Ŀ
	dwNumeOfSection = lpNTHead->FileHeader.NumberOfSections;

	//ͨ��FileAlignment������Ҫ���������ݴ�С
	dwAddLen = (dwLength+dwFileAlignment-1) & ~(dwFileAlignment-1);
	//�õ���һ���ڱ�ָ��
	lpSectionHead = (byte *)&(lpOptionHead->NumberOfRvaAndSizes)+
					sizeof (DWORD)+
					((lpOptionHead->NumberOfRvaAndSizes)*(sizeof (IMAGE_DATA_DIRECTORY)));

	//�������Ŀ
	i = dwNumeOfSection;
	//����ԭʼVirtualSize��SizeOfRawData
	dwOldVirtualSize = lpSectionHead[i-1].Misc.VirtualSize;
	dwOldSizeOfRawData = lpSectionHead[i-1].SizeOfRawData;
	//��չ���һ���ڵĴ�С���� ֱ���ں�׷�����ݣ���Ϊ���ӵ�����ӳ�䵽�ڴ�ռ����ܻᱻ�������ݸ�д
	//Ӧ�����������һ���ڵĴ�С��VirtualSize��ͬ
	//�Ƚ�VirtualSize��������û�ȥ
	lpSectionHead[i-1].Misc.VirtualSize = ((dwOldVirtualSize+dwSectionAlignment-1) & ~(dwSectionAlignment-1));
	lpSectionHead[i-1].SizeOfRawData = lpSectionHead[i-1].Misc.VirtualSize;

	//���㷵��ֵ
	dwRet = lpSectionHead[i-1].VirtualAddress + lpSectionHead[i-1].Misc.VirtualSize;
	//�������һ��������Ϊ��Ҫ���ӵ����ݳ���(�ڶ���ֵ)
	lpSectionHead[i-1].Misc.VirtualSize += dwAddLen;
	lpSectionHead[i-1].SizeOfRawData += dwAddLen;

	//�޸�SizeOfImage��С
	lpNTHead->OptionalHeader.SizeOfImage += (dwAddLen+dwSectionAlignment-1) & ~(dwSectionAlignment-1);

	//�޸ĵ���������Ŀ¼ֵ��ʹ��ָ������������ͷ��Rva
	lpOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = dwRet;
	lpOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size = dwLength;
	//�õ���ʵ���ӵ����ݴ�С
	dwRealAddLen = lpSectionHead[i-1].SizeOfRawData - dwOldSizeOfRawData;
	//�ͷ��ڴ�
	FreeMap(lpMapFile,&stFileMap);

	//ΪĿ���ļ����Ӵ�С
	hFile = CreateFile(stFileMap.szDll,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if ((DWORD)hFile == -1)
		return 0;

	SetFilePointer(hFile,dwRealAddLen,NULL,FILE_END);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	//�����µĵ�����Rva
	return dwRet;

}
