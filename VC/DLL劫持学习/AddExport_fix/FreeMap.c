#include <WINDOWS.H>
#include "AddExport.h"

/*
������lpFileMapָ���ڴ�ӳ���ļ�
���ܣ�ͨ�������lpFileMap�Լ�LPSTFILEMAP���ͽṹ���е�hFile��hFileMap���ͷŸ��ڴ�ӳ���ļ�
����ֵ������NULLΪ�������򷵻�-1
*/

BOOL WINAPI FreeMap (LPVOID lpFileMap,LPSTFILEMAP lpstFileMap)
{
	if (!UnmapViewOfFile(lpFileMap))
		return 0;
	if (!CloseHandle(lpstFileMap->hFileMap))
		return 0;
	if (!CloseHandle(lpstFileMap->hFile))
		return 0;
	return -1;
	
}
