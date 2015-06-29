#include <WINDOWS.H>
#include "AddExport.h"

/*
������szDllָ����Ҫ��ӽڵ�DLL�ļ�����dwLengthΪ��Ҫ������ݵĳ���
���ܣ�ΪszDll����һ���½ڣ�ֻ��
����ֵ������NULLΪ�������򷵻������ڵ�Rva��ַ
*/

DWORD WINAPI AddSection(PTCHAR lpszDll,DWORD dwLength)
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
	DWORD dwRet;

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
		MessageBox(NULL,TEXT ("����Ҫ�����ڵ�DLL�ļ�ʧ��!"),TEXT ("Error!"),MB_OK);
		return 0;
	}
	//�õ�������DLL�ļ���FileAlignment��ҳ����ֵ
	lpDosHead = lpMapFile;
	lpNTHead = (byte *)lpDosHead+lpDosHead->e_lfanew;
	lpOptionHead =	((byte *)lpNTHead+
					sizeof (DWORD)+
					sizeof (IMAGE_FILE_HEADER));
	dwFileAlignment = lpOptionHead->FileAlignment;
	dwSectionAlignment = lpOptionHead->SectionAlignment;
	//�õ�Ŀ���ļ�����Ŀ
	dwNumeOfSection = lpNTHead->FileHeader.NumberOfSections;

	//ͨ��FileAlignment������Ҫ�����Ľڴ�С
	dwAddLen = (dwLength+dwFileAlignment-1) & ~(dwFileAlignment-1);	//���������ڴ�С
	//�õ���һ���ڱ�ָ��
	lpSectionHead = (byte *)&(lpOptionHead->NumberOfRvaAndSizes)+
					sizeof (DWORD)+
					((lpOptionHead->NumberOfRvaAndSizes)*(sizeof (IMAGE_DATA_DIRECTORY)));
	//�ж����һ���ڱ������Ƿ��д��һ���µĽڱ�����
	i = dwNumeOfSection;
	if ((lpSectionHead[i].Characteristics != 0) ||
		(lpSectionHead[i].Misc.VirtualSize != 0) ||
		(lpSectionHead[i].Name[0] != 0) ||					//???
		(lpSectionHead[i].NumberOfLinenumbers != 0) ||
		(lpSectionHead[i].NumberOfRelocations != 0) ||
		(lpSectionHead[i].PointerToLinenumbers != 0) ||
		(lpSectionHead[i].PointerToRawData != 0) ||
		(lpSectionHead[i].PointerToRelocations != 0) ||
		(lpSectionHead[i].SizeOfRawData != 0) ||
		(lpSectionHead[i].VirtualAddress != 0))
	{
		return 0;
	}
	//Ϊ�����ڱ�ֵ����������Щֵ
	lpSectionHead[i].Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;
	lpSectionHead[i].Misc.VirtualSize = dwLength;
	lpSectionHead[i].VirtualAddress =	(lpSectionHead[i-1].VirtualAddress)+
										((lpSectionHead[i-1].Misc.VirtualSize+dwSectionAlignment-1) & ~(dwSectionAlignment-1));
	lpSectionHead[i].PointerToRawData =	(lpSectionHead[i-1].PointerToRawData)+
										((lpSectionHead[i-1].SizeOfRawData+dwFileAlignment-1) & ~(dwFileAlignment-1));
	lpSectionHead[i].SizeOfRawData = dwAddLen;

	//�޸�SizeOfImage��С
	lpNTHead->OptionalHeader.SizeOfImage += (dwLength+dwSectionAlignment-1) & ~(dwSectionAlignment-1);
	//���ӽ���Ŀ
	lpNTHead->FileHeader.NumberOfSections++;
	//�޸ĵ���������Ŀ¼ֵ��ʹ��ָ���½ڵ�ͷ��Rva
	lpOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = lpSectionHead[i].VirtualAddress;
	lpOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size = dwLength;

	//���÷���ֵ
	dwRet = lpSectionHead[i].VirtualAddress;
	//ͬ���޸�ֵ
	//FlushViewOfFile();
	//�ͷ��ڴ�
	FreeMap(lpMapFile,&stFileMap);

	//ΪĿ���ļ������½ڴ�С
	hFile = CreateFile(stFileMap.szDll,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if ((DWORD)hFile == -1)
		return 0;
	SetFilePointer(hFile,dwAddLen,NULL,FILE_END);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	//�����µĵ�����Rva
	return dwRet;

}
