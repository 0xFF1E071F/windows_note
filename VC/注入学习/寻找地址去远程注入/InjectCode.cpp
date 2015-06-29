// InjectCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>


struct _RemoteData 
{
	char cLoad[13];                      //����LoadLibraryA�ַ���
	char cGetAddr[15];                   //����GetProcAddress�ַ���
	char cUser32[11];                    //����user32.dll�ַ���
	char cKernel32[13];                  //����kernel32.dll�ַ���
	char cMessageBox[12];                //����MessageBoxA�ַ���
	char cGetModuleFileName[19];         //����GetModuleFileNameA�ַ���
	wchar_t wcKernel32[13];              //����kernel32.dll���ֽ��ַ���
};
typedef int (__stdcall *MESSAGEBOX)(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
typedef DWORD (__stdcall *GETMODULEFILENAME)(HMODULE hModule,LPTSTR lpFilename,DWORD nSize);
typedef HMODULE (__stdcall *LOADLIBRARY)(LPCTSTR lpFileName);
typedef FARPROC (__stdcall *GETADDRESS)(HMODULE hModule,LPCSTR lpProcName);


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

static  void  WINAPI RemoteThread(_RemoteData *pParam)
{
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
		return ;
	}
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
	DWORD dwOrdinale = 0; //���ڱ��浼����ű��ж�Ӧ����š�
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
			dwOrdinale = pNameOrdinalsTable[i];
			dwLoadLibraryAddress = pAddressOfFunctions[dwOrdinale];
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
			dwOrdinale = pNameOrdinalsTable[i];
			dwGetProcAddress = pAddressOfFunctions[dwOrdinale];
			bGetProcAddress = FALSE;
		}
	}
	LOADLIBRARY myLoadLib = (LOADLIBRARY)(pKernelBaseMem+dwLoadLibraryAddress);
	GETADDRESS myGetAddr = (GETADDRESS)(pKernelBaseMem+dwGetProcAddress);
	char cBuffer[128] = {0};
	((GETMODULEFILENAME)myGetAddr(myLoadLib(pParam->cKernel32), pParam->cGetModuleFileName))(NULL, cBuffer, 128);
	((MESSAGEBOX)myGetAddr(myLoadLib(pParam->cUser32), pParam->cMessageBox))(0,cBuffer,0,0);
}
static void AfterThread(){}

int main(int argc, char* argv[])
{
	void *pFun = NULL, *pParam = NULL;
	_RemoteData node;
	HANDLE hThread;
	strcpy(node.cLoad, "LoadLibraryA");
	strcpy(node.cGetAddr, "GetProcAddress");
	strcpy(node.cUser32, "user32.dll");
	strcpy(node.cMessageBox, "MessageBoxA");
	strcpy(node.cGetModuleFileName, "GetModuleFileNameA");
	strcpy(node.cKernel32, "kernel32.dll");
	wcscpy(node.wcKernel32, L"kernel32.dll");
	LPBYTE iBack = (LPBYTE) AfterThread;
	LPBYTE iBof = (LPBYTE) RemoteThread;
	int cbCodeSize;
	if (iBof > iBack)
	{
		cbCodeSize = (iBof - iBack);
	}
	else
	{
		cbCodeSize = (iBack - iBof);
	}	
	DWORD dwProcessID;
	printf("������Ҫע��Ľ���ID��");
	scanf("%d", &dwProcessID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessID);
	if (hProcess == NULL)
	{
		printf("�ý����޷�ע�룡");
		return -1;
	}
	//��Զ�̽��̿ռ�����ڴ�,�Ӷ�����Ҫִ�еĴ���
	pFun = VirtualAllocEx(hProcess, 0, cbCodeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (pFun == NULL)
	{
		printf("�ý����޷�ע�룡");
		return -1;
	}
	BOOL bSuccess;
	//����Ҫִ�еĴ��뵽Զ�̽��̿ռ�
	bSuccess = WriteProcessMemory(hProcess, pFun, RemoteThread, cbCodeSize, NULL);
	if (!bSuccess)
	{
		printf("�ý����޷�ע�룡");
		return -1;
	}
	pParam = VirtualAllocEx(hProcess, 0, sizeof(_RemoteData), MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (pParam == NULL)
	{
		printf("�ý����޷�ע�룡");
		return -1;
	}
	bSuccess = WriteProcessMemory(hProcess, pParam, &node, sizeof(_RemoteData), NULL);
	if (!bSuccess)
	{
		printf("�ý����޷�ע�룡");
		return -1;
	}
	//����Զ�̽���,��ִ�п�����ȥ�Ĵ���
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFun, 
		pParam, 0, NULL);
	//�ȴ�Զ�̽��̴���ִ����Ϸ���
	WaitForSingleObject(hThread ,INFINITE);
	//�ͷ�Զ�̽����з�����ڴ�
	VirtualFreeEx(hProcess, pParam, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, pFun, 0, MEM_RELEASE);
	//�رս��̾��
	CloseHandle(hProcess);
	//�ر��߳̾��
	CloseHandle(hThread);
	return 0;
}

