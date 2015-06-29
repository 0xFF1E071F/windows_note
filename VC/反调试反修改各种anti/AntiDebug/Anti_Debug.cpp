#include "StdAfx.h"
#include "Anti_Debug.h"

Anti_Debug::Anti_Debug(void)
{
}

Anti_Debug::~Anti_Debug(void)
{
}

// ������
BOOL Anti_Debug::Anti_Debug_ProcessName()
{
	BOOL bResult = FALSE;
	TCHAR szOLLYDBG[] = _T("OLLYDBG.EXE");  //Ҫ����OD������
	TCHAR sz52[] = _T("�ᰮ�ƽ�.EXE");
	TCHAR szICey[] = _T("ICEYOD.EXE");
	TCHAR szODICE[] = _T("OLLYICE.EXE");
	TCHAR szWINDBG[] = _T("WINGUARD.EXE");
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnap)
	{
		return FALSE;
	}
	if (Process32First(hSnap, &pe))
	{
		do
		{	// �����ִ�Сд
			if (lstrcmpi(szOLLYDBG, pe.szExeFile) == 0 || 
				lstrcmpi(sz52, pe.szExeFile) == 0||
				lstrcmpi(szICey, pe.szExeFile) == 0 ||
				lstrcmpi(szODICE, pe.szExeFile) == 0||
				lstrcmpi(szWINDBG, pe.szExeFile) == 0)
			{
				bResult = TRUE;
			}
		} while (Process32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
	if (bResult)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// ��������
BOOL Anti_Debug::Anti_Debug_ClassName()
{
	TCHAR szWindowClass[] = _T("OLLYDBG");

	if(::FindWindow(szWindowClass, NULL)) //�ڶ�������ΪNULL��ʾ��ѯ���д���������OLLYDBG
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// ������
BOOL Anti_Debug::Anti_Debug_ParentProcess()
{
	BOOL bResult = FALSE;
	TCHAR szCurrentAndParentIdAndExplorerId[MAX_PATH] = {0};//���嵱ǰ����ID�����ĸ����ڼ��������ID

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	DWORD dwExplorerId; 
	//��ȡexplorer.exe���̵�ID��������̵�������"Progman"
	//ע�ⷵ�صĽ���ID����GetWindowThreadProcessId�����ĵڶ�������
	//GetWindowThreadProcessId�������ص��Ǵ���explorer.exe�̵߳ľ��
	::GetWindowThreadProcessId(::FindWindow(_T("Progman"), NULL), &dwExplorerId);

	Process32First(hSnap, &pe);
	do
	{
		if (GetCurrentProcessId() == pe.th32ProcessID)  //��������ǰ����
		{
			//��ǰ���̵ĸ�����ID�������explorer.exe���̵�ID�ţ�һ���ʾ��������
			if (dwExplorerId != pe.th32ParentProcessID)
			{
				bResult = TRUE;
			}
		}

	} while (Process32Next(hSnap, &pe));

	/*wsprintf(szCurrentAndParentIdAndExplorerId, _T("��ǰID��%u��������ID��%u���������ID��%u"), 
	GetCurrentProcessId(), 
	pe.th32ParentProcessID,
	dwExplorerId);

	::MessageBox(NULL, szCurrentAndParentIdAndExplorerId, _T("���"), 0);*/

	if (bResult)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	CloseHandle(hSnap);
}

// ����int3�ϵ�
BOOL Anti_Debug::Anti_Debug_FuncInt3(char* FuncName) 
{
	BOOL bODFound = FALSE;
	DWORD dwAddr;
	dwAddr = (DWORD)GetProcAddress(LoadLibrary(_T("user32.dll")), FuncName);

	__asm
	{
		cld	// clear Direction flag
			mov edi, dwAddr 
			mov ecx, 100h //ecxΪѭ������
			mov al, 055h	
			xor al, 099h //alΪѭ������
			repne scasb //ִ��edi++��ecx--������[edi]��al�Ƚϣ���ȾͲ��ٽ���edi++��ecx--����
			cmp ecx, 0h	
			jz ODNotFound
			mov bODFound, 1
ODNotFound:
	}
	if (bODFound)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

// code�μ��
BOOL Anti_Debug::Anti_Debug_CodeSectionInt3()
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS32 pNtHeaders;
	PIMAGE_SECTION_HEADER pSectionHeader;
	DWORD dwBaseImage = (DWORD)GetModuleHandle(NULL); //��ȡ��ǰ���̵Ļ�ַ


	pDosHeader = (PIMAGE_DOS_HEADER)dwBaseImage;
	pNtHeaders = (PIMAGE_NT_HEADERS32)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeaders + sizeof(pNtHeaders->Signature) + sizeof(IMAGE_FILE_HEADER) + 
		(WORD)pNtHeaders->FileHeader.SizeOfOptionalHeader);

	DWORD dwAddr = pSectionHeader->VirtualAddress + dwBaseImage; //��ȡ��������ڴ��еĵ�ַ
	DWORD dwCodeSize = pSectionHeader->SizeOfRawData;    //��ȡ����ζ����Ĵ�С
	BOOL bFound = FALSE;

	__asm
	{
			cld               ;�����뿪ʼ
			mov edi, dwAddr
			mov ecx, dwCodeSize //������������
			mov al, 055h 
			xor al, 099h
			repne scasb
			cmp ecx, 0h	//����0��ʾû�м�⵽cc
			jz     NotFound
			mov bFound,1
NotFound:             
	}
	if (bFound)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

//����PROCESSINFOCLASS�ṹ��
typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation, // 0 Y N
	ProcessQuotaLimits, // 1 Y Y
	ProcessIoCounters, // 2 Y N
	ProcessVmCounters, // 3 Y N
	ProcessTimes, // 4 Y N
	ProcessBasePriority, // 5 N Y
	ProcessRaisePriority, // 6 N Y
	ProcessDebugPort, // 7 Y Y
	ProcessExceptionPort, // 8 N Y
	ProcessAccessToken, // 9 N Y
	ProcessLdtInformation, // 10 Y Y
	ProcessLdtSize, // 11 N Y
	ProcessDefaultHardErrorMode, // 12 Y Y
	ProcessIoPortHandlers, // 13 N Y
	ProcessPooledUsageAndLimits, // 14 Y N
	ProcessWorkingSetWatch, // 15 Y Y
	ProcessUserModeIOPL, // 16 N Y
	ProcessEnableAlignmentFaultFixup, // 17 N Y
	ProcessPriorityClass, // 18 N Y
	ProcessWx86Information, // 19 Y N
	ProcessHandleCount, // 20 Y N
	ProcessAffinityMask, // 21 N Y
	ProcessPriorityBoost, // 22 Y Y
	ProcessDeviceMap,// 23 Y Y
	ProcessSessionInformation, // 24 Y Y
	ProcessForegroundInformation, // 25 N Y
	ProcessWow64Information // 26 Y N
} PROCESSINFOCLASS;

//�Զ���NtQueryInformationProcess����ָ��
typedef NTSTATUS (WINAPI *MYFUNC)
( 
 __in          HANDLE ProcessHandle,
 __in          PROCESSINFOCLASS ProcessInformationClass,
 __out         PVOID ProcessInformation,
 __in          ULONG ProcessInformationLength,
 __out_opt     PULONG ReturnLength
 );

//CheckRemoteDebuggerPresent�ڲ�ʵ��ԭ��NtQueryInformationProcess
//�����Լ�����
BOOL Anti_Debug::Anti_Debug_NtInformationProcess()
{
	HANDLE hProcess = GetCurrentProcess();
	DWORD dwResult;

	MYFUNC NtQueryInformationProcess = (MYFUNC)GetProcAddress(::LoadLibrary(_T("ntdll.dll")), 
		"NtQueryInformationProcess");
	NtQueryInformationProcess(hProcess, ProcessDebugPort, &dwResult, 4, 0);
	if (dwResult != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Anti_Debug::Anti_Debug_Checkremotedebuggerpresent()
{
	HANDLE hProcess = ::GetCurrentProcess();
	BOOL bDebuggerPresent = FALSE;

	CheckRemoteDebuggerPresent(hProcess, &bDebuggerPresent);
	if (TRUE == bDebuggerPresent)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//ԭ��:Windows����ϵͳ�е�explorer.exe�������̵�ʱ����STARTUPINFO�ṹ�е�ֵ��Ϊ0,
//����explorer.exe�������̵�ʱ����������ṹ�е�ֵ��Ҳ���ǽṹ�е�ֵ��Ϊ0��
//���Կ�������������ж�OD�Ƿ��ڵ��Գ���.
BOOL Anti_Debug::Anti_Debug_GetStartupInfo()
{
	STARTUPINFO info = {0};
	GetStartupInfo(&info); //��ȡ�ṹ�����Ϣ

	if (info.dwX!=0 || info.dwY!=0 || info.dwXCountChars!=0 || info.dwYCountChars!=0
		|| info.dwFillAttribute!=0 || info.dwXSize!=0 || info.dwYSize!=0)
	{
		return TRUE;
	}
	else
	{
		FALSE;
	}
}



//���뵥�����Բ��ܳɹ�   
//���˸о�ûɶ��
BOOL Anti_Debug::Anti_Debug_GetTickCount()
{
	DWORD start = GetTickCount();
	//һЩָ��
	GetCurrentProcessId();
	GetCurrentProcessId();
	GetCurrentProcessId();
	GetCurrentProcessId();
	/*
	sleep(50)  ����Ҳ���Ի���������
	*/
	DWORD end = GetTickCount();
	if ((end - start) > 100)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}