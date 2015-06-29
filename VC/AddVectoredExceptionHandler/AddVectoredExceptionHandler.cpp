// AddVectoredExceptionHandler.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "AddVectoredExceptionHandler.h"

PVOID (CALLBACK* AddVectoredExceptionHandler)(ULONG FirstHandler,PVOID VectoredHandler);

LONG WINAPI VectoredHandler(
  PEXCEPTION_POINTERS ExceptionInfo
  )
{
	if (ExceptionInfo->ExceptionRecord->ExceptionAddress = (PVOID)0x00408EC8)
		//�����ַ��JE����ʧ�ܵĵ�һ����ַ
	{
	//	MessageBox(NULL,"�ɹ�����CC �쳣 VEH","VEH",MB_ICONINFORMATION);
		
		ExceptionInfo->ContextRecord->Eip = 0x00408ED2;
		
		
		DWORD dwpProtect = 0;
		VirtualProtect((PVOID)0x00408EC8,0x1000,PAGE_EXECUTE_READWRITE,&dwpProtect);
		*(PBYTE)0x00408EC8 = 0x6A;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	
	// Ҫע���Ŀ��EXE, �Լ�����Ҳ������쳣����.����Ҫ���ؼ���ɨ��.
	return EXCEPTION_CONTINUE_SEARCH;
}
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
		//		MessageBox(NULL,"DLLע��ɹ�","��ϲ",MB_ICONINFORMATION);

				*(FARPROC*)&AddVectoredExceptionHandler = GetProcAddress(GetModuleHandle("kernel32.dll"),\
					"AddVectoredExceptionHandler");
				//���VEH
				AddVectoredExceptionHandler(1,VectoredHandler);
				Sleep(1000);
				//�Ľ��ڴ��ҳȨ�ޣ���ֹû��дȨ
				DWORD dwOldProtect = 0;
				VirtualProtect((PVOID)0x00408EC8,0x1000,PAGE_EXECUTE_READWRITE,&dwOldProtect);
				*(PBYTE)0x00408EC8 = 0xcc;

			}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			{
		//		MessageBox(NULL,"DLL����","ע��",MB_ICONINFORMATION);
			}
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
ADDVECTOREDEXCEPTIONHANDLER_API int nAddVectoredExceptionHandler=0;

// This is an example of an exported function.
ADDVECTOREDEXCEPTIONHANDLER_API int fnAddVectoredExceptionHandler(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see AddVectoredExceptionHandler.h for the class definition
CAddVectoredExceptionHandler::CAddVectoredExceptionHandler()
{ 
	return; 
}

