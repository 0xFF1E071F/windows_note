#include "stdio.h"
#include "windows.h"
#include "Sirius.h"

//�쳣����������
EXCEPTION_DISPOSITION _cdecl ExceptionHandler(EXCEPTION_RECORD * ExceptionRecord,
											  LPVOID EstablisherFrame,
											  CONTEXT * ContextRecord,
											  LPVOID DispatcherContext);
//ȫ�ֱ������ڴ洢�쳣��������ַ
DWORD dwHandlerAddress = (DWORD)ExceptionHandler;

LPVOID APIENTRY GetKernelBase()
{
	LPVOID lpModule = NULL; 
	{
		_asm push ebp					//��������,ԭʼEBP
		_asm push offset next			//��������,��ȫ��������
		_asm push dwHandlerAddress
		_asm push fs:[0]
		_asm mov fs:[0],esp
		_asm cld
		_asm xor esi,esi
		_asm mov esi,dword ptr fs:[0] 
_asm lookup:
		_asm lodsd      
		_asm inc eax
		_asm je find
		_asm dec eax
		_asm xchg esi,eax
		_asm jmp short lookup             //�õ����һ��SEH hanlder
_asm find:
	    _asm mov edi,dword ptr [esi]
	    _asm and edi,0xffff0000
_asm start:
		_asm cmp word ptr [edi],0x5a4d
		_asm jnz next
		_asm mov esi,edi
	    _asm add esi,dword ptr ds:[esi+0x3c]
		_asm cmp word ptr [esi],0x4550
		_asm jnz next
		_asm mov lpModule,edi
		_asm jmp short end	               //�ҵ� NTDLL
_asm next:
		_asm sub edi,0x10000
		_asm cmp edi,0x70000000
		_asm je end
		_asm jmp short start
_asm end:
		_asm pop fs:[0]
        _asm add esp,0x0c
	}
	return lpModule;
}  

EXCEPTION_DISPOSITION _cdecl ExceptionHandler(EXCEPTION_RECORD * ExceptionRecord,
											  LPVOID EstablisherFrame,
											  CONTEXT * ContextRecord,
											  LPVOID DispatcherContext)
{
	if(ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION)
	{
		//�ڴ�����쳣,����EIPΪ�����ĵ�ַ
		DWORD dwEbp=0,dwEip=0,dwEsp=0;
		_asm
		{
			mov eax,EstablisherFrame
			push dword ptr [eax+0x08]
			pop dwEip
			push dword ptr [eax+0x0c]
			pop	dwEbp
			mov dwEsp,eax
		}
		ContextRecord->Eip=dwEip;
		ContextRecord->Ebp=dwEbp;
		ContextRecord->Esp=dwEsp;
		return ExceptionContinueExecution;
	}

}

int main(void)
{
	printf("Get Kenel Module ImageBase by Search SEH is : %p \n",GetKernelBase());
	printf("Get Kenel Module ImageBase by call the LoadLibrary Procedure is : %p \n",LoadLibrary(TEXT("Kernel32.dll")));
	system("pause");
	return 0;
}