#include <Windows.h>

typedef HANDLE (WINAPI *CREATEFILEW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

CREATEFILEW dwCreateFileWAddr = 0;

HANDLE
WINAPI
MyCreateFileW(
            LPCWSTR lpFileName,
            DWORD dwDesiredAccess,
            DWORD dwShareMode,
            LPSECURITY_ATTRIBUTES lpSecurityAttributes,
            DWORD dwCreationDisposition,
            DWORD dwFlagsAndAttributes,
            HANDLE hTemplateFile
    )
{
    WCHAR wFileName[MAX_PATH] = { 0 };
    wcscpy(wFileName, lpFileName);
    if ( wcscmp(wcslwr(wFileName), L"g:\\test.txt") == 0 )
    {
        if ( MessageBox(NULL, "�Ƿ���ļ�", "��ʾ", MB_YESNO) == IDYES )
        {
            return dwCreateFileWAddr(lpFileName,
                            dwDesiredAccess,
                            dwShareMode,
                            lpSecurityAttributes,
                            dwCreationDisposition,
                            dwFlagsAndAttributes,
                            hTemplateFile);
        } 
        else
        {
            return INVALID_HANDLE_VALUE;
        }
    }
    else
    {
        return dwCreateFileWAddr(lpFileName,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile);
    }
}

VOID HookNotePadProcessIAT()
{
    // ���Createfile
    HMODULE hMod = LoadLibrary("kernel32.dll");
    DWORD dwFuncAddr = (DWORD)GetProcAddress(hMod, "CreateFileW");
    CloseHandle(hMod);

    // ��ȡ���±�����ģ���ַ
    HMODULE hModule = GetModuleHandleA(NULL);

    // ��λPE�ṹ
    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)((DWORD)hModule + pDosHdr->e_lfanew);

    // ����ӳ���ַ��������RVA
    DWORD dwImageBase = pNtHdr->OptionalHeader.ImageBase;
    DWORD dwImpRva = pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    // ������VA
    PIMAGE_IMPORT_DESCRIPTOR pImgDes = (PIMAGE_IMPORT_DESCRIPTOR)(dwImageBase + dwImpRva);

    char szAddr[10] = { 0 };

    PIMAGE_IMPORT_DESCRIPTOR pTmpImpDes = pImgDes;
    BOOL bFound = FALSE;

    // ������HOOK������ģ����
    while ( pTmpImpDes->Name )
    {
        DWORD dwNameAddr = dwImageBase + pTmpImpDes->Name;
        char szName[MAXBYTE] = { 0 };
        strcpy(szName, (char*)dwNameAddr);

        if ( strcmp(strlwr(szName), "kernel32.dll") == 0 )
        {
            bFound = TRUE;
            break;
        }
        pTmpImpDes ++;
    }

    // �ж��Ƿ��ҵ���HOOK�������ڵĺ�����
    if ( bFound == TRUE )
    {
        bFound = FALSE;
        char szAddr[10] = { 0 };

        // ���������ģ���IAT��ַ
        PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)(pTmpImpDes->FirstThunk + dwImageBase);

        while ( pThunk->u1.Function )
        {
            DWORD *pAddr = (DWORD *)&(pThunk->u1.Function);
            // �Ƚ��Ƿ�����HOOK�����ĵ�ַ��ͬ
            if ( *pAddr == dwFuncAddr )
            {
                bFound = TRUE;
                dwCreateFileWAddr = (CREATEFILEW)*pAddr;
                DWORD dwMyHookAddr = (DWORD)MyCreateFileW;
                // �޸�ΪHook�����ĵ�ַ
                WriteProcessMemory(GetCurrentProcess(), (LPVOID)pAddr, &dwMyHookAddr, sizeof(DWORD), NULL);
                break;
            }
            pThunk ++;
        }
    }

    if ( bFound == TRUE )
    {
        MessageBox(NULL, "Hook Successfully !", NULL, MB_OK);
    } 
    else
    {
        MessageBox(NULL, "Hook Unsuccessfully !", NULL, MB_OK);
    }
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
        {
            // ��DLL������ʱ����HookNotePadProcessIAT()
            HookNotePadProcessIAT();
            break;
        }
    }
    return TRUE;
}