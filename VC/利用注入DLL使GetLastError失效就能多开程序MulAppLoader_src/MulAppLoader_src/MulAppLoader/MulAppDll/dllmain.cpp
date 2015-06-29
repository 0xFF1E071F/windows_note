#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include ".\\detour\\detours.h"

static DWORD (WINAPI * Old_GetLastError)() = GetLastError;
DWORD New_GetLastError()
{
	DWORD dwLastError = Old_GetLastError();
	return dwLastError == ERROR_ALREADY_EXISTS ? 0 : dwLastError;
}

VOID Hook ()
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// �������������ε���DetourAttach������Hook�������
	DetourAttach(&(PVOID&)Old_GetLastError, New_GetLastError ) ;

	DetourTransactionCommit();
}

VOID UnHook ()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// �������������ε���DetourDetach�����������������Hook
	DetourDetach(&(PVOID&)Old_GetLastError, New_GetLastError ) ;

	DetourTransactionCommit();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					   )
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		Hook();
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		UnHook();
	}

	return TRUE;
}