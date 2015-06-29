#include "SSDTHook.h"

//ԭʼ������ַ
ULONG g_OriginalNtQueryDirectoryFile = 0;
ULONG g_OriginalNtQuerySystemInformation = 0;

//�ļ�����
ULONG g_FileCount = 0;
//�ļ���
FILE_HIDDEN_TABLE g_FileHiddenTable;

//����ָ��
PFNNtQueryDirectoryFile g_pfnOriginalNtQueryDirectoryFile = NULL;
PFNNtQuerySystemInformation g_pfnOriginalNtQuerySystemInformation = NULL;

WCHAR g_szProcessName[60] = L"";

ULONG g_uNtQueryDirectoryFileIndex = 0;
ULONG g_uNtQuerySystemInformationIndex = 0;

//��ȡ��SSDT���еĵ�ַ
ULONG GetSSDTAddr( ULONG uIndex )
{
    ULONG uSSDTAddr = 0; 

    uSSDTAddr = (ULONG)KeServiceDescriptorTable->ServiceTableBase + uIndex * sizeof(ULONG);

    return uSSDTAddr;
}

//��ȡ����ʵ��ָ��ĵ�ַ
ULONG GetSSDTFunctionAddr( ULONG uIndex )
{
    ULONG uFunctionAddr = 0;

    uFunctionAddr = *(PULONG)((ULONG)KeServiceDescriptorTable->ServiceTableBase + uIndex * sizeof(ULONG));

    return uFunctionAddr;
}


BOOLEAN HookSSDTFunction( ULONG uIndex, ULONG uNewFunctionAddr, PULONG puOriginalFunctionAddr )
{
    if (uNewFunctionAddr == 0)
    {
        return FALSE;
    }

    ULONG uSSDTAddr = GetSSDTAddr(uIndex);

    *puOriginalFunctionAddr = *(PULONG)uSSDTAddr; //����ԭʼ������ַ

    DisablePageProtect();

    *(PULONG)uSSDTAddr = uNewFunctionAddr;//д���µ�ַ

    EnablePageProtect();

    return TRUE;
}

BOOLEAN UnHookSSDTFunction( ULONG uIndex, ULONG uOriginalFunctionAddr )
{
    if (uOriginalFunctionAddr == 0)
    {
        return FALSE;
    }

    ULONG uSSDTAddr = GetSSDTAddr(uIndex); //��ȡSSDT���ַ

    DisablePageProtect();

    *(PULONG)uSSDTAddr = uOriginalFunctionAddr; //д��ԭʼ��ַ

    EnablePageProtect();

    return TRUE;
}

VOID SetNtQueryDirectoryFileHook(BOOLEAN bHook )
{
    if (bHook)
    {
        ULONG uNtQueryDirectoryFileAddr = GetSSDTFunctionAddr(g_uNtQueryDirectoryFileIndex);

        g_pfnOriginalNtQueryDirectoryFile =  (PFNNtQueryDirectoryFile)uNtQueryDirectoryFileAddr;

        HookSSDTFunction(g_uNtQueryDirectoryFileIndex, (ULONG)NewNtQueryDirectoryFile, &g_OriginalNtQueryDirectoryFile);
    }
    else
    {
        if (g_OriginalNtQueryDirectoryFile != 0)
        {
            UnHookSSDTFunction(g_uNtQueryDirectoryFileIndex, g_OriginalNtQueryDirectoryFile);
            g_OriginalNtQueryDirectoryFile = 0;
        }
    }
}

VOID SetNtQuerySystemInformationHook( BOOLEAN bHook )
{
    if (bHook)
    {
        ULONG uNtQuerySystemInformationAddr = GetSSDTFunctionAddr(g_uNtQuerySystemInformationIndex);

        g_pfnOriginalNtQuerySystemInformation =  (PFNNtQuerySystemInformation)uNtQuerySystemInformationAddr;

        HookSSDTFunction(g_uNtQuerySystemInformationIndex, (ULONG)NewNtQuerySystemInformation, &g_OriginalNtQuerySystemInformation);
    }
    else
    {
        if (g_OriginalNtQuerySystemInformation != 0)
        {
            UnHookSSDTFunction(g_uNtQuerySystemInformationIndex, g_OriginalNtQuerySystemInformation);
            g_OriginalNtQuerySystemInformation = 0;
        }
    }
}


VOID EnablePageProtect()
{
    _asm
    {
        push eax
        mov eax, cr0
        or    eax, 0x10000
        mov cr0, eax
        pop eax
        sti
    }
}

VOID DisablePageProtect()
{
    _asm
    {
        cli
        push eax
        mov eax, cr0
        and eax , 0xFFFEFFFF
        mov cr0, eax
        pop eax
    }
}

NTSTATUS NewNtQueryDirectoryFile(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID FileInformation,
    ULONG Length,
    FILE_INFORMATION_CLASS FileInformationClass,
    BOOLEAN ReturnSingleEntry,
    PUNICODE_STRING FileName,
    BOOLEAN RestartScan
    )
{

    NTSTATUS status;
   /* KdPrint(("I'm In NewZwQueryDirectoryFile \n"));*/
    
    status = ((PFNNtQueryDirectoryFile)g_pfnOriginalNtQueryDirectoryFile)(FileHandle, Event,
                                                                                                   ApcRoutine, ApcContext,
                                                                                                   IoStatusBlock, FileInformation,
                                                                                                   Length, FileInformationClass,
                                                                                                   ReturnSingleEntry, FileName, RestartScan);


    //���ļ�������������
    if (NT_SUCCESS(status) && (FileInformationClass == FileIdBothDirectoryInformation))
    {
        PFILE_ID_BOTH_DIR_INFORMATION pFileInfo;
        PFILE_ID_BOTH_DIR_INFORMATION pLastFileInfo;
        BOOLEAN bLastOne = FALSE;

        pFileInfo = (PFILE_ID_BOTH_DIR_INFORMATION)FileInformation;
        pLastFileInfo = NULL;
        do 
        {
            bLastOne = !(pFileInfo->NextEntryOffset);
            
           
            if (CheckFileList(pFileInfo->FileName) == TRUE)
            {
                if (bLastOne)
                {
                    if (pFileInfo == (PFILE_ID_BOTH_DIR_INFORMATION)FileInformation)
                    {
                        status = STATUS_NO_MORE_FILES;
                    }
                    else
                    {
                        pLastFileInfo->NextEntryOffset = 0;
                    }
                    
                    break;
                }
                else
                {
                    int iPos = ((ULONG)pFileInfo) - (ULONG)FileInformation;
                    int iLeft = (ULONG)Length - iPos - pFileInfo->NextEntryOffset;
                    RtlCopyMemory((PVOID)pFileInfo, (PVOID)((char*)pFileInfo + pFileInfo->NextEntryOffset),(ULONG)iLeft);

                    continue;
                }
            }

            pLastFileInfo = pFileInfo;
            pFileInfo = (PFILE_ID_BOTH_DIR_INFORMATION)((char*)pFileInfo + pFileInfo->NextEntryOffset);
        } while (!bLastOne);
       
    }

    //��CMD������

    if (NT_SUCCESS(status) && (FileInformationClass == FileFullDirectoryInformation))
    {
        PFILE_FULL_DIR_INFORMATION pFileInfo;
        PFILE_FULL_DIR_INFORMATION pLastFileInfo;
        BOOLEAN bLastOne = FALSE;

        pFileInfo = (PFILE_FULL_DIR_INFORMATION)FileInformation;
        pLastFileInfo = NULL;
        do 
        {
            bLastOne = !(pFileInfo->NextEntryOffset);


            if (CheckFileList(pFileInfo->FileName) == TRUE)
            {
                if (bLastOne)
                {
                    if (pFileInfo == (PFILE_FULL_DIR_INFORMATION)FileInformation)
                    {
                        status = STATUS_NO_MORE_FILES;
                    }
                    else
                    {
                        pLastFileInfo->NextEntryOffset = 0;
                    }

                    break;
                }
                else
                {
                    int iPos = ((ULONG)pFileInfo) - (ULONG)FileInformation;
                    int iLeft = (ULONG)Length - iPos - pFileInfo->NextEntryOffset;
                    RtlCopyMemory((PVOID)pFileInfo, (PVOID)((char*)pFileInfo + pFileInfo->NextEntryOffset),(ULONG)iLeft);

                    continue;
                }
            }

            pLastFileInfo = pFileInfo;
            pFileInfo = (PFILE_FULL_DIR_INFORMATION)((char*)pFileInfo + pFileInfo->NextEntryOffset);
        } while (!bLastOne);

    }
  
    return status;
}

NTSTATUS NewNtQuerySystemInformation( ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength )
{
    NTSTATUS ntStatus;
    ntStatus = ((PFNNtQuerySystemInformation)g_OriginalNtQuerySystemInformation)(
                    SystemInformationClass,
                    SystemInformation,
                    SystemInformationLength,
                    ReturnLength);

    if (NT_SUCCESS(ntStatus))
    {
        if (SystemInformationClass == 5)//SystemProcessInformation
        {
            PSYSTEM_PROCESSES pCurr = (PSYSTEM_PROCESSES)SystemInformation;
            PSYSTEM_PROCESSES pPrev = NULL;

            while(pCurr)
            {
                if (pCurr->ProcessName.Buffer != NULL)
                {
                    //KdPrint(("pCurr->ProcessName = %S ", pCurr->ProcessName.Buffer));
                    if (0 == memcmp(pCurr->ProcessName.Buffer, g_szProcessName,(wcslen(g_szProcessName)*sizeof(WCHAR))))
                    {
                        if (pPrev)
                        {
                            if (pCurr->NextEntryDelta)
                            {
                                pPrev->NextEntryDelta += pCurr->NextEntryDelta;
                            }
                            else
                            {
                                pPrev->NextEntryDelta = 0;
                            }
                        }
                        else
                        {
                            if (pCurr->NextEntryDelta)
                            {
                                SystemInformation =(PSYSTEM_PROCESSES)((char*)pCurr + pCurr->NextEntryDelta);
                            }
                            else
                            {
                                //Ωһ�Ľ���
                                SystemInformation = NULL;
                            }
                        }
                    }
                }
                else
                {

                }

                pPrev = pCurr;
                if (pCurr->NextEntryDelta)
                {
                    pCurr = (PSYSTEM_PROCESSES)((char*)pCurr + pCurr->NextEntryDelta);
                }
                else
                {
                    pCurr = NULL;
                }
            }
        }
    }
    return ntStatus;
}

/***********************************************************************/
void InitializeFileHiddenTable()
{
    int nIdx = 0;
    for (nIdx = 0; nIdx < MAX_FILE_COUNT; ++nIdx)
    {
        g_FileHiddenTable.bHiddenSwitch[nIdx] = 1; //��ʼ��Ϊȫ������
    }
}

void AddFileToFileHiddenTable( LPCWSTR lpFileName )
{
    INT nIdx = 0;//g_FileCount;

    //�ȼ���Ƿ��Ѿ��д�����
    for (nIdx = 0; nIdx < g_FileCount; ++nIdx)
    {
        //����д�����Ŀ��ֱ�ӽ�����Ϊ����
        if (wcscmp(g_FileHiddenTable.szFolderName[nIdx], lpFileName) == 0)
        {
            SetFileHiddenState(lpFileName, TRUE);
            return;
        }
    }

    INT nIndex = g_FileCount;

    //����µ���Ŀ������
    INT nLength = (wcslen(lpFileName)+1) * sizeof(WCHAR);
    RtlCopyBytes(g_FileHiddenTable.szFolderName[nIndex], lpFileName, nLength);

    //ȫ���ļ�����+1
    g_FileCount = g_FileCount + 1;

    //KdPrint(("g_FileCount = %d", g_FileCount));
}

void SetFileHiddenState( LPCWSTR lpFileName, BOOLEAN bHidden )
{
    INT nIdx = 0;
    for (nIdx = 0; nIdx < g_FileCount; ++nIdx)
    {
        if (wcscmp(g_FileHiddenTable.szFolderName[nIdx], lpFileName) == 0)
        {
            //���ÿ���
            g_FileHiddenTable.bHiddenSwitch[nIdx] = bHidden;
            break;
        }
    }
}

BOOLEAN  CheckFileList( LPWSTR lpFileName )
{
    INT nIdx;
    for (nIdx = 0; nIdx < g_FileCount; ++nIdx)
    {
      
        if (wcsstr(lpFileName, g_FileHiddenTable.szFolderName[nIdx]) != NULL||
            wcscmp(lpFileName, g_FileHiddenTable.szFolderName[nIdx])==0) //����ļ��б����Ƿ��ж�Ӧ���ļ���
        {
            if (g_FileHiddenTable.bHiddenSwitch[nIdx] == TRUE)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void ClearFileCount()
{
    g_FileCount = 0;
}

VOID SetProcessHidden( LPCWSTR lpProcessName, BOOLEAN bHidden )
{
    if (lpProcessName != NULL)
    {
        if (bHidden)
        {
            if (g_OriginalNtQuerySystemInformation != 0)//����Hook�У��Ȱ�Hookͣ��
            {
                SetNtQuerySystemInformationHook(FALSE);
            }
            RtlZeroMemory(g_szProcessName, sizeof(g_szProcessName));
            RtlCopyMemory(g_szProcessName, lpProcessName, (wcslen(lpProcessName)+1)*sizeof(WCHAR));

            SetNtQuerySystemInformationHook(TRUE);
        }
        else
        {
            SetNtQuerySystemInformationHook(FALSE);
        }

    }
}

ULONG GetFunctionAddr(PCWSTR FunctionName)
{
    UNICODE_STRING strFunctionName;

    RtlInitUnicodeString(&strFunctionName,FunctionName);

    return (ULONG)MmGetSystemRoutineAddress(&strFunctionName);
}

WIN_VER_DETAIL GetWindowsVersion()
{
    RTL_OSVERSIONINFOEXW OSVersionInfoEx = { sizeof(RTL_OSVERSIONINFOEXW) };
    PFN_RtlGetVersion pfnRtlGetVersion = NULL;
    pfnRtlGetVersion = (PFN_RtlGetVersion)GetFunctionAddr(L"RtlGetVersion");
    if ( NULL == pfnRtlGetVersion)
    {
        PsGetVersion(&OSVersionInfoEx.dwMajorVersion,&OSVersionInfoEx.dwMinorVersion,
            &OSVersionInfoEx.dwBuildNumber,NULL);
    }
    else
    {
        pfnRtlGetVersion((PRTL_OSVERSIONINFOW)&OSVersionInfoEx);
    }
    if (5 == OSVersionInfoEx.dwMajorVersion)
    {
        switch (OSVersionInfoEx.dwMinorVersion)
        {
        case 0:
            return WINDOWS_VERSION_2K;
        case 1:
            return WINDOWS_VERSION_XP;
        case 2:
            if (0 == OSVersionInfoEx.wServicePackMajor)
                return WINDOWS_VERSION_2K3;
            else
                return WINDOWS_VERSION_2K3_SP1_SP2;
        }
    }
    if (6 == OSVersionInfoEx.dwMajorVersion)
    {
        switch (OSVersionInfoEx.dwMinorVersion)
        {
        case 0:
            return WINDOWS_VERSION_VISTA;
        case 1:
            return WINDOWS_VERSION_7;
        }
    }
    return WINDOWS_VERSION_UNKNOWN;
}

VOID CheckWindowsVersion()
{
    ULONG uIndex = 0;
    WIN_VER_DETAIL WinVer = GetWindowsVersion();
    if (WinVer == WINDOWS_VERSION_XP)
    {
        g_uNtQueryDirectoryFileIndex = 0x91;
        g_uNtQuerySystemInformationIndex = 0xAD;
    }
    else if (WinVer == WINDOWS_VERSION_7)
    {
        g_uNtQueryDirectoryFileIndex = 0xDF;
        g_uNtQuerySystemInformationIndex = 0x105;
    }


}



/***********************************************************************/