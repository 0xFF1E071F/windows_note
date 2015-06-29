// InjectedDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
struct _InjectData 
{
	char cLoad[13];                      //����LoadLibraryA�ַ���
	char cGetAddr[15];                   //����GetProcAddress�ַ���
	char cKernel32[13];                  //����kernel32.dll�ַ���
	wchar_t wcKernel32[13];              //����kernel32.dll���ֽ��ַ���
	DWORD dwImageBase;
};

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
    PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;

typedef struct _PEB_LDR_DATA
{
	ULONG Length; // +0x00
	BOOLEAN Initialized; // +0x04
	PVOID SsHandle; // +0x08
	LIST_ENTRY InLoadOrderModuleList; // +0x0c
	LIST_ENTRY InMemoryOrderModuleList; // +0x14
	LIST_ENTRY InInitializationOrderModuleList;// +0x1c
} PEB_LDR_DATA,*PPEB_LDR_DATA; // +0x24

typedef struct _LDR_MODULE
{
	LIST_ENTRY InLoadOrderModuleList; //+0x00
	LIST_ENTRY InMemoryOrderModuleList; //+0x08
	LIST_ENTRY InInitializationOrderModuleList; //0x10
	void* BaseAddress; //0x18   //ģ���ַ
	void* EntryPoint; //0x1c
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;   //ģ������
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	SHORT LoadCount;
	SHORT TlsIndex;
	HANDLE SectionHandle;
	ULONG CheckSum;
	ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;


typedef struct _LDR_MODULE2
{
	LIST_ENTRY InMemoryOrderModuleList; //+0x08
	LIST_ENTRY InInitializationOrderModuleList; //0x10
	void* BaseAddress; //0x18   //ģ���ַ
	void* EntryPoint; //0x1c
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;   //ģ������
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	SHORT LoadCount;
	SHORT TlsIndex;
	HANDLE SectionHandle;
	ULONG CheckSum;
	ULONG TimeDateStamp;
} LDR_MODULE2, *PLDR_MODULE2;

typedef struct _PEB
{
	UCHAR InheritedAddressSpace; // 00h
	UCHAR ReadImageFileExecOptions; // 01h
	UCHAR BeingDebugged; // 02h
	UCHAR Spare; // 03h
	PVOID Mutant; // 04h
	PVOID ImageBaseAddress; // 08h
	PPEB_LDR_DATA Ldr; // 0Ch
	//...... ����ĳ�Ա����
} PEB, *PPEB;

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;

typedef struct _NT_TEB
{
	NT_TIB Tib; // 00h
	PVOID EnvironmentPointer; // 1Ch
	CLIENT_ID Cid; // 20h
	PVOID ActiveRpcInfo; // 28h
	PVOID ThreadLocalStoragePointer; // 2Ch
	PPEB Peb; // 30h
	//...... ����ĳ�Ա����
} NT_TEB, *PNT_TEB;

typedef HMODULE (__stdcall *LOADLIBRARY)(LPCTSTR lpFileName);
typedef FARPROC (__stdcall *GETADDRESS)(HMODULE hModule,LPCSTR lpProcName);


DWORD __stdcall ThreadSelf()
{
	while (1)   //ѭ��ɨ��ϵͳ����
	{
		TCHAR tcBuffer[4] = {0}; //׼����С�ڴ�,����̽��ʵ����Ҫ���ڴ��С��
		DWORD dwNeededSize = GetLogicalDriveStrings(4, tcBuffer); //��һ��ִ�У����ʵ����Ҫ�ڴ档
		if (dwNeededSize > 4)
		{   //����ֵ����ָ���ڴ��С,��Ҫ����׼���㹻�ڴ��ٴ�ִ�С�
			TCHAR *pBuffer = (TCHAR *)GlobalAlloc(GPTR, dwNeededSize*sizeof(TCHAR));  //׼���㹻����ڴ�
			dwNeededSize = GetLogicalDriveStrings(dwNeededSize, pBuffer); //����߼��������ַ���
			TCHAR *pDrive = pBuffer;
			int iPreLength = 0;
			while (1)
			{
				pDrive = pBuffer+iPreLength;  //�����һ��������
				if (pDrive[0] == 0) //��ȡ����������
				{
					break; //�˳�ѭ��
				}
				DWORD dwDriveType = GetDriveType(pDrive); //��ȡ����������
				if (dwDriveType == DRIVE_REMOVABLE)
				{   //������ƶ�������
					TCHAR tcFile[MAX_PATH] = {0};
					_stprintf(tcFile, _T("%sa.txt"), pDrive);
					//���Ѵ��ڵ�a.txt�ļ�
					HANDLE hFile = CreateFile(tcFile, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						//��ʧ���򴴽�һ����
						hFile = CreateFile(tcFile, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						DWORD dwWrite = 0;
						WriteFile(hFile, _T("Hello"), 5, &dwWrite, NULL);
					}
					CloseHandle(hFile);
				}
				iPreLength += _tcslen(pDrive);  //������ǰ������
				iPreLength += sizeof(TCHAR);    //����'\0'��
			}
			if (pBuffer != NULL)
			{
				GlobalFree(pBuffer);
				pBuffer = NULL;
			}
		}
		Sleep(500);  //��ͣ500��������ɨ��
	}
}

extern "C" __declspec(dllexport) BOOL ThreadMain(_InjectData *pParam)
{
	//1�����kernel32.dllģ���ַ
	PBYTE pKernelBaseMem = NULL;
	////////
	PNT_TEB pTeb = NULL; 
	__asm
	{
		mov eax,fs:[0x18]    //���FS�μĴ������ڴ��еľ����ַ,��TEB�ĵ�ַ
		mov pTeb,eax         
	}
	PPEB pPeb = pTeb->Peb;   //ͨ��TEB���PEB�ĵ�ַ
	PPEB_LDR_DATA pLdrData = pPeb->Ldr;
	PLDR_MODULE pLdrModule = (PLDR_MODULE)pLdrData->InLoadOrderModuleList.Flink;
	PLDR_MODULE pNext = pLdrModule; //��ñ���ģ����Ϣ����ͷ
	//��������,����kernel32.dlģ��Ļ�ַ
	do 
	{
		int i=0, iPos;
		//���Ȳ���ģ��·�������һ��"\"��λ��
		while (pNext->FullDllName.Buffer[i])
		{
			if (pNext->FullDllName.Buffer[i] == L'\\')
			{
				iPos = i;
			}	
			i++;
		}
		i = 0;
		//�жϸ�ģ�������Ƿ�Ϊkernel32.dll
		while (i<=11)
		{
			if (pNext->FullDllName.Buffer[iPos+i+1] == pParam->wcKernel32[i])
			{
				i++;
			}
			else
			{
				break;
			}
		}
		if (i==12)
		{   //����ҵ�kernel32.dllģ�飬ȡ�����ַ��
			pKernelBaseMem = (PBYTE)pNext->BaseAddress;
			break;
		}
		pNext = (PLDR_MODULE)pNext->InLoadOrderModuleList.Flink;
	} while (pLdrModule != pNext); //����ֻص�����ͷ���˳�
	//////////////////////////
	if (pKernelBaseMem == NULL)
	{
		return FALSE;
	}
	//2������kernel32.dllģ�鵼������ȡLoadLibrary��GetProcAddress������ַ
	//pKernelBaseMem��Kernel32.dllģ��Ļ�ַ
	IMAGE_NT_HEADERS *pPeHeader = (IMAGE_NT_HEADERS *)(pKernelBaseMem + ((IMAGE_DOS_HEADER *)pKernelBaseMem)->e_lfanew);
	//�õ�Kernel32.dllģ��ĵ�����
	IMAGE_EXPORT_DIRECTORY *pExportDescriptor =(IMAGE_EXPORT_DIRECTORY *)(pKernelBaseMem + pPeHeader->OptionalHeader.DataDirectory[0].VirtualAddress);
	//�������Ƶ����ĺ�������
	int nNamesNum = pExportDescriptor->NumberOfNames;
	//�ֱ��õ�������������ű�����������ַ�������������Ƶ�ַ��
	WORD * pNameOrdinalsTable = (WORD *)(pKernelBaseMem+pExportDescriptor->AddressOfNameOrdinals);
	DWORD * pAddressOfName = (DWORD *)(pKernelBaseMem+pExportDescriptor->AddressOfNames);
	DWORD * pAddressOfFunctions = (DWORD *)(pKernelBaseMem+pExportDescriptor->AddressOfFunctions);
	//�������ڱ�������������ַ�ı���
	DWORD dwLoadLibraryAddress = 0, dwGetProcAddress = 0;
	BOOL bLoadLibrary = TRUE, bGetProcAddress = TRUE;
	for (int i=0; i<nNamesNum&&(bLoadLibrary || bGetProcAddress); i++)
	{
		char *pFuncName = (char *)pKernelBaseMem+pAddressOfName[i];
		int j = 0;
		while (bLoadLibrary)
		{
			if (pFuncName[j] != pParam->cLoad[j] || pParam->cLoad[j] == 0)
			{	//�����ΪpParam->cLoad[j] == 0���˳�ѭ�����ҵ�LoadLibraryA����
				break;
			}
			j++;
		}
		if (bLoadLibrary && pParam->cLoad[j] == 0)
		{
			dwLoadLibraryAddress = DWORD(pKernelBaseMem+pAddressOfFunctions[pNameOrdinalsTable[i]]);
			bLoadLibrary = FALSE;
		}
		j=0;
		while (bGetProcAddress)
		{
			if (pFuncName[j] != pParam->cGetAddr[j] || pParam->cGetAddr[j] == 0)
			{//�����ΪpParam->cLoad[j] == 0���˳�ѭ�����ҵ�GetProcAddress����
				break;
			}
			j++;
		}
		if (bGetProcAddress && pParam->cGetAddr[j] == 0)
		{
			dwGetProcAddress = DWORD(pKernelBaseMem+pAddressOfFunctions[pNameOrdinalsTable[i]]);
			bGetProcAddress = FALSE;
		}
	}
	//3�����IAT
	//���DLLע���Ļ�ַ
	PBYTE pMem = (PBYTE)pParam->dwImageBase;
	//���DLL��PEͷ
	pPeHeader = (PIMAGE_NT_HEADERS)(pMem+((PIMAGE_DOS_HEADER)pMem)->e_lfanew);
	DWORD dwImportRVA = pPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	//���DLL�ĵ������ʼ��ַ
	PIMAGE_IMPORT_DESCRIPTOR pImportTable = (PIMAGE_IMPORT_DESCRIPTOR)(pMem+dwImportRVA);
	IMAGE_THUNK_DATA *pOriginalThunk = NULL;
	IMAGE_THUNK_DATA *pThunk = NULL;
	LPCSTR pFxName = NULL;
	PIMAGE_IMPORT_BY_NAME piibn = NULL;
	while (pImportTable->Name)
	{
		//���ص������Ŀ�
		HMODULE hLib = ((LOADLIBRARY)dwLoadLibraryAddress)((char *)(pMem+pImportTable->Name));
		if (hLib == NULL)
		{
			return FALSE;
		}
		//��õ��뺯�����Ƶ�ַ��
		if (pImportTable->OriginalFirstThunk == 0)
		{ 
			pOriginalThunk = (IMAGE_THUNK_DATA *)(pMem + pImportTable->FirstThunk);
		}
		else
		{
			pOriginalThunk = (IMAGE_THUNK_DATA *)(pMem + pImportTable->OriginalFirstThunk);
		}
		//��õ��뺯����ַ��
		pThunk = (IMAGE_THUNK_DATA *)(pMem + pImportTable->FirstThunk);
		while (pOriginalThunk->u1.Function)
		{
			//�ж��Ƿ�����ŵ���
			if(pOriginalThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
			{	//���浼�����
				pFxName = (LPCSTR)IMAGE_ORDINAL(pOriginalThunk->u1.Ordinal);
			}
			else
			{   //���浼�뺯�������ַ������ڵ�ַ
				piibn = (PIMAGE_IMPORT_BY_NAME)(pMem + (DWORD)pOriginalThunk->u1.AddressOfData);
				pFxName = (LPCSTR)piibn->Name;
			}
			//��ȡ���뺯���ĵ�ַ��д��IAT��
			if(pThunk->u1.Function == NULL)
			{
				return FALSE;
			}
			pThunk->u1.Function = (PDWORD)((GETADDRESS)dwGetProcAddress)(hLib, pFxName);	
			pThunk++; //������һ��������ַ
			pOriginalThunk++;//������һ�����Ƶ�ַ
		}
		pImportTable++;  //������һ�������
	}
	//ThreadSelf();
	MessageBox(NULL,"��ʾע��ɹ���",NULL,MB_OK);
	return TRUE;
}

