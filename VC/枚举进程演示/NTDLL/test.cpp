#include <windows.h>
#include <ntsecapi.h>
#include <stdio.h>

#define SystemProcessesAndThreadsInformation	5

// ��̬����
typedef DWORD (WINAPI *ZWQUERYSYSTEMINFORMATION) (DWORD,
												  PVOID,
												  DWORD,
												  PDWORD);

// �ṹ����
typedef struct _SYSTEM_PROCESS_INFORMATION{
	DWORD		NextEntryDelta;
	DWORD		ThreadCount;
	DWORD		Reserved1[6];
	FILETIME	ftCreateTime;
	FILETIME	ftUserTime;
	FILETIME	ftKernelTime;
	UNICODE_STRING ProcessName;
	DWORD		BasePriority;
	DWORD		ProcessId;
	DWORD		InheritedFromProcessId;
	DWORD		HandleCount;
	DWORD		Reserved2[2];
	DWORD		VmCounters;
	DWORD		dCommitCharge;
	PVOID		ThreadInfos[1];
}SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;


int main()
{
	// ��������
	HMODULE hNtDll = GetModuleHandle("ntdll.dll");
	ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(hNtDll,"ZwQuerySystemInformation");

	ULONG  cbBuffer = 0x10000;
	LPVOID pBuffer  = NULL;

	pBuffer = malloc(cbBuffer);
	if(pBuffer == NULL)
		return -1;

	// ��ȡ������Ϣ
	ZwQuerySystemInformation(SystemProcessesAndThreadsInformation,pBuffer,cbBuffer,NULL);
	// ָ��ָ������ͷ��
	PSYSTEM_PROCESS_INFORMATION pInfo = (PSYSTEM_PROCESS_INFORMATION)pBuffer;

	// ������
	for(;;)
	{
		printf("PID:%d  \t%ls\n",pInfo->ProcessId,pInfo->ProcessName.Buffer);

		if(pInfo->NextEntryDelta == 0)
			break;

		// ��ȡ��һ���ڵ�
		pInfo = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)pInfo)+pInfo->NextEntryDelta);
	}
	// �ͷŻ�����
	free(pBuffer);

	return 0;
}