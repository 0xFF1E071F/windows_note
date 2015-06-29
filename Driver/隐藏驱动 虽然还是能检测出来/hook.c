#include <ntddk.h>

typedef unsigned long DWORD;

typedef struct _KLDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    PVOID ExceptionTable;
    ULONG ExceptionTableSize;
    PVOID GpValue;
    DWORD UnKnow;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT __Unused5;
    PVOID SectionPointer;
    ULONG CheckSum;
    PVOID LoadedImports;
    PVOID PatchInformation;
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

PDRIVER_OBJECT pDriverObject = NULL;

VOID 
HideDriver()
{
    PKLDR_DATA_TABLE_ENTRY entry =(PKLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
    PKLDR_DATA_TABLE_ENTRY firstentry;
    UNICODE_STRING uniDriverName;
    
    firstentry = entry;

    // ��ʼ��Ҫ����������������
    RtlInitUnicodeString(&uniDriverName, L"XueTr.sys");
    
    while((PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink != firstentry)
    {
        if (entry->FullDllName.Buffer != 0)
        {    
            if (RtlCompareUnicodeString(&uniDriverName, &(entry->BaseDllName), FALSE) == 0)
            {
                KdPrint(("�������� %ws �ɹ�!\n", entry->BaseDllName.Buffer));    
                // �޸� Flink �� Blink ָ��, ����������Ҫ���ص�����
                *((DWORD*)entry->InLoadOrderLinks.Blink) = (DWORD)entry->InLoadOrderLinks.Flink;
                entry->InLoadOrderLinks.Flink->Blink = entry->InLoadOrderLinks.Blink;
                
                /* 
                    ʹ����������LIST_ENTRY�ṹ���Flink, Blink��ָ���Լ�
                    ��Ϊ�˽ڵ㱾����������, ��ô���ڽӵĽڵ�������ж��ʱ, 
                    ϵͳ��Ѵ˽ڵ��Flink, Blink��ָ�������ڽڵ����һ���ڵ�.
                    ����, ����ʱ�Ѿ�����������, ���������ԭ�����ڵĽڵ�������
                    ж����, ��ô�˽ڵ��Flink, Blink���п���ָ�����õĵ�ַ, ��
                    �������Ե�BSoD.
                */
                entry->InLoadOrderLinks.Flink = (LIST_ENTRY*)&(entry->InLoadOrderLinks.Flink);
                entry->InLoadOrderLinks.Blink = (LIST_ENTRY*)&(entry->InLoadOrderLinks.Flink);

                break;
            }
        }
        // ������ǰ��
        entry = (PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink;
    }
}

NTSTATUS 
UnloadDriver(
             IN PDRIVER_OBJECT DriverObject
             )
{
    return STATUS_SUCCESS;
}

NTSTATUS 
DriverEntry(
            IN PDRIVER_OBJECT DriverObject, 
            IN PUNICODE_STRING  RegistryPath
            )
{
    DriverObject->DriverUnload = UnloadDriver;
    pDriverObject = DriverObject;
    HideDriver();
    return STATUS_SUCCESS;
}