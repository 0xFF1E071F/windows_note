// InjectDll����.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
HMODULE g_hmodule;

void HideDll()
{
	__asm
	{
		pushad
			xor eax,eax
			mov eax,fs:[0x30]
			mov eax,[eax+0xc]
			mov eax,[eax+0xc]
__loop:
		mov ecx,[eax+0x18]
			cmp ecx,g_hmodule
			jz __found
			mov eax,[eax]
			jmp __loop
__found:
		mov ecx,[eax+0x24+0x4]
			mov dword ptr [ecx],0
			popad
	}
	return ;
}
BOOL APIENTRY DllMain( HANDLE hModule, //���ص�DLL���ж϶�
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//__asm int 3;
			g_hmodule = (HMODULE)hModule;
			MessageBox(NULL,"��ʼ����","��ʼ����",MB_OK);
			DisableThreadLibraryCalls(g_hmodule);
			HideDll();	
		MessageBox(NULL,"�ɹ�","�ɹ�",MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL,"ж��","ж��",MB_OK);
		break;
	}
    return TRUE;
}

