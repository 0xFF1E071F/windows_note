#include <windows.h>
#include "AddExport.h"

/*
������lpFileMapָ��LPSTFILEMAP���͵Ľṹ��
���ܣ�ͨ�������LPSTFILEMAP���ͽṹ���е�szDll���������ں˶������Ȩ�޵ȣ��ڴ�ӳ����ļ��������򿪵ľ����Map������봫��Ľṹ����
����ֵ������NULLΪ�������򷵻�ָ���ڴ�ӳ���ļ���ָ��
*/

LPVOID WINAPI MapFile (LPSTFILEMAP lpstFileMap)
{
	HANDLE	hFile;
	HANDLE	hFileMap;
	HANDLE	lpFileMap;
	
//	hFile = CreateFile	(lpstFileMap->szDll,GENERIC_READ | GENERIC_WRITE,
//		FILE_SHARE_READ | FILE_SHARE_WRITE,
	hFile = CreateFile	(lpstFileMap->szDll,lpstFileMap->dwDesiredAccess,
		lpstFileMap->dwShareMode,
		NULL,
		lpstFileMap->dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if ((DWORD)hFile == -1)
		return NULL;
	
//	hFileMap = CreateFileMapping (hFile,NULL,PAGE_READWRITE,0,0,NULL);
	hFileMap = CreateFileMapping (hFile,NULL,lpstFileMap->flProtect,0,0,NULL);
	if (!hFileMap)
	{
		CloseHandle(hFile);
		return NULL;
	}
//	lpFileMap = MapViewOfFile(hFileMap,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
	lpFileMap = MapViewOfFile(hFileMap,lpstFileMap->dwDesiredAccessMap,0,0,0);
	if (!lpFileMap)
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return NULL;
	}
	lpstFileMap->hFile = hFile;
	lpstFileMap->hFileMap = hFileMap;
	return (LPVOID)lpFileMap;
	/**/
}