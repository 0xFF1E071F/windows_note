#include <windows.h>
#include "AddExport.h"

/*
������lpFileָ��PE�ļ��ڴ�ӳ��ͷ����Rvaָ�������ַ
���ܣ�ͨ�������ַѭ���жϸ������ַ�����ĸ�section�У�������Rva��Ӧ����lpFileָ���PE�ڴ�ӳ���ַ
����ֵ������NULLΪ����
*/

VOID * WINAPI RvaToOffset (IMAGE_DOS_HEADER *lpFile,VOID *Rva)
{
	PIMAGE_DOS_HEADER lpDosHead;	//ָ��Dosͷָ��
	PIMAGE_NT_HEADERS32 lpNTHead;	//ָ��NTͷָ��
	DWORD dwNumeOfSection;			//����Ŀ
	PIMAGE_OPTIONAL_HEADER32 lpOptionHead;	//ָ���ѡͷ
	PIMAGE_SECTION_HEADER lpSectionHead;	//ָ��ڱ�ͷ
	int i;
	
	lpDosHead = lpFile;
	lpNTHead = (PIMAGE_NT_HEADERS32)((byte *)lpDosHead+lpDosHead->e_lfanew);
	dwNumeOfSection = lpNTHead->FileHeader.NumberOfSections;
	lpOptionHead = (PIMAGE_OPTIONAL_HEADER32)((byte *)lpNTHead+
					sizeof (DWORD)+
					sizeof (IMAGE_FILE_HEADER));
	lpSectionHead = (byte *)&(lpOptionHead->NumberOfRvaAndSizes)+
					sizeof (DWORD)+
					((lpOptionHead->NumberOfRvaAndSizes)*(sizeof (IMAGE_DATA_DIRECTORY)));
	for	(i=0;dwNumeOfSection != 0;dwNumeOfSection--,i++)	//ѭ������Rva�����ĸ�����
	{
		if ((DWORD)Rva >= lpSectionHead[i].VirtualAddress && 
			(DWORD)Rva <= (lpSectionHead[i].VirtualAddress+lpSectionHead[i].Misc.VirtualSize))
			//���Rva����ĳ���ڵ�Rva����
			return (VOID *)((DWORD)Rva-lpSectionHead[i].VirtualAddress+
							(DWORD)lpFile+
							lpSectionHead[i].PointerToRawData);
			//����Rva��Ӧ��ָ��PE�ڴ�ӳ��ĵ�ַ
	}
	return NULL;
}

