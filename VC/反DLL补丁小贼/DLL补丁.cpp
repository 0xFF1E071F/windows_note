// DLL����.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "DLL����.h"

// ��ΪVC��SDK�汾̫����,SDK��û�� AddVectoredExceptionHandler �������.
// ����Ҫ�Լ����� ����...

// ����
PVOID (CALLBACK *AddVectoredExceptionHandler)(ULONG FirstHandler, PVOID VectoredHandler);



// VEH ����  �ص�����  Ҫ�з���ֵ
LONG WINAPI VectoredHandler(
							PEXCEPTION_POINTERS ExceptionInfo
							)
{
	if(ExceptionInfo->ExceptionRecord->ExceptionAddress == (PVOID)0x0041530D)
	{
		MessageBox(NULL, "�ɹ�����CC�쳣", ":)", MB_ICONINFORMATION);

		// �޸ĵ�ǰ�쳣�жϵ�EIP
		ExceptionInfo->ContextRecord->Eip = 0x00415216;

		// ���ڴ��ҳȨ��,��ֹû��дȨ
		DWORD dwOldProtect = 0;
		VirtualProtect((PVOID)0x0041530D, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		
		// ���￪ʼ�޸�CC,�ⷢ�쳣
		*(PBYTE)0x0041530D = 0x8b;

		// ��ʾ�Ҵ������쳣.
		return EXCEPTION_CONTINUE_EXECUTION;
	}


	// Ҫע���Ŀ��EXE, �Լ�����Ҳ������쳣����.����Ҫ���ؼ���ɨ��.
	return EXCEPTION_CONTINUE_SEARCH;
}

/*
SEH ջ��
_except()
{

}
*/


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				MessageBox(NULL, "DLLע��ɹ�", "��ϲ", MB_ICONINFORMATION);


				// ��ȡ AddVectoredExceptionHandler ������ַ.
				*(FARPROC*) &AddVectoredExceptionHandler = GetProcAddress(GetModuleHandle("kernel32.dll"), 
													  "AddVectoredExceptionHandler");

				// ���VEH
				AddVectoredExceptionHandler(1, VectoredHandler);

				// ���ڴ��ҳȨ��,��ֹû��дȨ
				DWORD dwOldProtect = 0;
				VirtualProtect((PVOID)0x0041530D, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);

				// ���￪ʼ�޸�CC,�ⷢ�쳣
				*(PBYTE)0x0041530D = 0xcc;


// 				DWORD dwOldProtect = 0;
// 				VirtualProtect((PVOID)0x00415211, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
// 				*(PBYTE)0x00415211 = 0x85;

				break;
			}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			{
				MessageBox(NULL, "DLL���̷���", ":)", MB_ICONINFORMATION);
			}

			break;
    }
    return TRUE;
}


// This is an example of an exported variable
DLL_API int nDLL=0;

// This is an example of an exported function.
DLL_API int fnDLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see DLL����.h for the class definition
CDLL::CDLL()
{ 
	return; 
}

