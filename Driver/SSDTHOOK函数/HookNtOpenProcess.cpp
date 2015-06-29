#ifdef __cplusplus
extern "C"
{
#endif
#include <ntddk.h>
#ifdef __cplusplus
}
#endif

typedef struct _ServiceDescriptorTable {
    unsigned int* ServiceTableBase; //System Service Dispatch Table �Ļ���ַ  
    unsigned int* ServiceCounterTable;
    //������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ���¡� 
    unsigned int NumberOfServices;//�� ServiceTableBase �����ķ������Ŀ��  
    unsigned char* ParamTableBase; //����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ��������� 
}*PServiceDescriptorTable;  

extern "C" extern PServiceDescriptorTable KeServiceDescriptorTable;

typedef NTSTATUS(*MYNTOPENPROCESS)(
    OUT PHANDLE             ProcessHandle,
    IN ACCESS_MASK          AccessMask,
    IN POBJECT_ATTRIBUTES   ObjectAttributes,
    IN PCLIENT_ID           ClientId );//����һ������ָ�룬���������O_NtOpenProcess����ǿ��ת��

ULONG O_NtOpenProcess = 0; 

extern "C" NTSTATUS
    PsLookupProcessByProcessId(
    IN HANDLE ProcessId,
    OUT PEPROCESS *Process
    );

void PageProtectOff()//�ر�ҳ�汣��
{
    __asm{
        cli
            mov  eax,cr0
            and  eax,not 10000h
            mov  cr0,eax
    }
}
void PageProtectOn()//��ҳ�汣��
{
    __asm{
        mov  eax,cr0
            or   eax,10000h
            mov  cr0,eax
            sti
    }
}

// �ж��Ƿ�򿪵����Լ��뱣���Ľ���
BOOLEAN ProtectProcess(HANDLE ProcessId,char *str_ProtectObjName)
{
    NTSTATUS status;
    PEPROCESS process_obj;

    if(!MmIsAddressValid(str_ProtectObjName))//��������������ж�Ŀ��������Ƿ���Ч
    {
        return FALSE;
    }
    if(ProcessId == 0)//��������������ų�System Idle Process���̵ĸ���
    {
        return FALSE;
    }
    status=PsLookupProcessByProcessId(ProcessId, &process_obj);//���������ȡĿ����̵�EPROCESS�ṹ
    if(!NT_SUCCESS(status))
    {
        KdPrint(("�Ҵ��ˣ�����Ǵ����:%X---����ǽ���ID:%d\n",status,ProcessId));
        return FALSE;
    }
    if(!strcmp((char *)process_obj + 0x174, str_ProtectObjName))//���бȽ�
    {
        ObDereferenceObject(process_obj);//�����������1��Ϊ�˻ָ�������������������ڻ���
        return TRUE;
    }
    ObDereferenceObject(process_obj);
    return FALSE;
    //ʹ����PsLookupProcessByProcessId�󣬱�����ObDereferenceObject���գ�
    //PsLookupProcessByProcessId��ȡָ�����̵�EPROCESS�ṹ,Ŀ�ľ��ǻ�ȡ������
}
NTSTATUS MyNtOpenProcess (
    __out PHANDLE ProcessHandle,
    __in ACCESS_MASK DesiredAccess,
    __in POBJECT_ATTRIBUTES ObjectAttributes,
    __in_opt PCLIENT_ID ClientId
    )
{
    if(ProtectProcess(ClientId->UniqueProcess,"notepad.exe"))    // ����򿪵Ľ�����Ŀ�����
    {
        KdPrint(("%s���ļ�ʧ��\n",(char *)PsGetCurrentProcess()+0x174));//dt _EPROCESS ���Բ鿴EPROCESS�ṹ
        return STATUS_UNSUCCESSFUL;    
		// ����һ���ؼ�������STATUS_UNSUCCESSFUL��CE�ͻ���ʾ�򿪲��ɹ�
    }
    
    return ((MYNTOPENPROCESS)O_NtOpenProcess)(ProcessHandle,
		//�������Լ�������󣬵���ԭ���ĺ�����������������������
        DesiredAccess,
        ObjectAttributes,
        ClientId);
}


VOID UnHookSsdt()
{
    PageProtectOff();
    KeServiceDescriptorTable->ServiceTableBase[122] = O_NtOpenProcess; //�ָ�ssdt��ԭ���ĺ�����ַ
    PageProtectOn();
}
NTSTATUS HookSsdt()
{
    // 1����ssdt_hook()�����б���Ҫhook�����ģ�NtOpenProcess����ַ
    O_NtOpenProcess = KeServiceDescriptorTable->ServiceTableBase[122];
    PageProtectOff();
    // 2����ԭ��ssdt����Ҫhook�ĺ�����ַ���������Լ��ĺ�����ַ
    KeServiceDescriptorTable->ServiceTableBase[122] = (unsigned int)MyNtOpenProcess;
    // ��ʱ������CE�򿪱������Ľ��̣��ͻ���������Լ��ĺ���
    PageProtectOn();
    
    return STATUS_SUCCESS;
}
VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    UnHookSsdt();
    KdPrint(("Driver Unload Success !\n"));
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath )
{
    KdPrint(("Welcome to My Driver\n"));
    pDriverObject->DriverUnload = DriverUnload;
    /* ���к����ĵ�ַ
    for (int i = 0; i < KeServiceDescriptorTable->NumberOfServices; i++)
    {
	KdPrint(("NumberOfService[%d]-------%X\n", i, KeServiceDescriptorTable->ServiceTableBase[i]));
}*/
    HookSsdt();
    return STATUS_SUCCESS;
}