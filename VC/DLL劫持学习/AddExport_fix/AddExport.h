#include <WINDOWS.H>
#include <SHLWAPI.H>
#include "resource.h"

//��װ���ڴ�ӳ���ļ���Ҫ�õ��Ľṹ
typedef struct  
{
	PTCHAR szDll;	//�ļ���ָ��
	DWORD dwDesiredAccess;	//CreateFile����
	DWORD dwShareMode;	//CreateFile����
	DWORD dwCreationDisposition;	//CreateFile����
	DWORD flProtect;	//CreateFileMaping����
	DWORD dwDesiredAccessMap;	//MapViewOfFile����
	HANDLE hFile;	//�򿪺���ļ����
	HANDLE hFileMap;	//Map���
}STFILEMAP,*LPSTFILEMAP;

//���ݸ��߳���Ҫ�õ��Ľṹ����Ϣ
typedef struct
{
	PTCHAR lpszExportDll;	//��Ҫ���Ƶ�������Ϣ��DLL�ļ���ָ��
	PTCHAR lpszTargetDll;	//��Ҫ�����ת��������Ϣ��DLL�ļ���ָ��
	PTCHAR lpszTransitName;	//��תDLL��
	PTCHAR lpszSaveName;	//������ļ���ָ��
	DWORD dwIsAddSection;	//�Ƿ�ʹ�������ڷ�ʽ��Ϊ0��ʹ���������һ���ڵķ�ʽ
	HANDLE hDlg;
}STPARAM,*LPSTPARAM;

//��������
DWORD WINAPI AddExportThread (lpParameter);
INT_PTR CALLBACK DialogProc (hWnd,uMsg,wParam,lParam);
VOID * WINAPI RvaToOffset (IMAGE_DOS_HEADER *lpFile,VOID *Rva);
LPVOID WINAPI MapFile (LPSTFILEMAP lpFileMap);
BOOL WINAPI FreeMap(LPVOID lpFileMap,LPSTFILEMAP lpstFileMap);
DWORD CalExportLength(LPVOID lpFile,DWORD dwTransitNameLen);
DWORD WINAPI AddSection(PTCHAR lpszDll,DWORD dwLength);
DWORD WINAPI ExpandLastSection(PTCHAR lpszDll,DWORD dwLength);
BOOL WINAPI CopyExportToBuffer(PIMAGE_DOS_HEADER lpFile,DWORD lpExport,DWORD lpBuffer,LPSTR lpTransitName,DWORD Rva);