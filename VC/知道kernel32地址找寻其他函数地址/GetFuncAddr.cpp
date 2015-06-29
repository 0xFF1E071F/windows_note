// GetFuncAddr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	PBYTE pKernelBaseMem = (PBYTE)0x7c800000; //Kernel32.dll�Ļ�ַͨ��Ϊ0x7c800000��
	//pKernelBaseMem��Kernel32.dllģ��Ļ�ַ
	IMAGE_NT_HEADERS *pPeHeader = (IMAGE_NT_HEADERS *)(pKernelBaseMem + ((IMAGE_DOS_HEADER *)pKernelBaseMem)->e_lfanew);
	//�õ�Kernel32.dllģ��ĵ�����
	IMAGE_EXPORT_DIRECTORY *pExportDescriptor =(IMAGE_EXPORT_DIRECTORY *)(pKernelBaseMem + pPeHeader->OptionalHeader.DataDirectory[0].VirtualAddress);
	//�������Ƶ����ĺ�������
	int nNamesNum = pExportDescriptor->NumberOfNames;
	//�ֱ��õ�������������ű�����������ַ�������������Ƶ�ַ��
	WORD * pNameOrdinalsTable = (WORD *)(pKernelBaseMem+pExportDescriptor-> AddressOfNameOrdinals);
	DWORD * pAddressOfName = (DWORD *)(pKernelBaseMem+pExportDescriptor-> AddressOfNames);
	DWORD * pAddressOfFunctions = (DWORD *)(pKernelBaseMem+pExportDescriptor-> AddressOfFunctions);
	//�������ڱ�������������ַ�ı���
	DWORD dwLoadLibraryAddress = 0, dwGetProcAddress = 0;
	BOOL bLoadLibrary = TRUE, bGetProcAddress = TRUE; 
	DWORD dwOrdinale = 0; //���ڱ��浼����ű��ж�Ӧ�����
	char *pLoadLibrary = "LoadLibraryA", *pGetProcAddress = "GetProcAddress";
	for (int i=0; i<nNamesNum&&(bLoadLibrary || bGetProcAddress); i++)
	{
		char *pFuncName = (char *)pKernelBaseMem+pAddressOfName[i];
		int j = 0;
		while (bLoadLibrary)
		{
			if (pFuncName[j] != pLoadLibrary [j] || pLoadLibrary [j] == 0)
			{	//�����ΪpLoadLibrary [j] == 0���˳�ѭ�����ҵ�LoadLibraryA����
				break;
			}
			j++;
		}
		if (bLoadLibrary && pLoadLibrary [j] == 0)
		{
			dwOrdinale = pNameOrdinalsTable[i];
			dwLoadLibraryAddress = pAddressOfFunctions[dwOrdinale];
			bLoadLibrary = FALSE;
		}
		j=0;
		while (bGetProcAddress)
		{
			if (pFuncName[j] != pGetProcAddress [j] || pGetProcAddress [j] == 0)
			{//�����ΪpGetProcAddress [j] == 0���˳�ѭ�����ҵ�GetProcAddress����
				break;
			}
			j++;
		}
		if (bGetProcAddress && pGetProcAddress [j] == 0)
		{
			dwOrdinale = pNameOrdinalsTable[i];
			dwGetProcAddress = pAddressOfFunctions[dwOrdinale];
			bGetProcAddress = FALSE;
		}
	}
	printf("LoadLibraryA�ĵ�ַ��  0x%X\n", pKernelBaseMem+dwLoadLibraryAddress);
	printf("GetProcAddress�ĵ�ַ��0x%X", pKernelBaseMem+dwGetProcAddress);
	return 0;
}

