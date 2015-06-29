#include <ntddk.h>

typedef struct _SystemServiceDescriptorTable 
{ 
    PVOID ServiceTableBase; //SSDT��Ļ���ַ
    PULONG ServiceCounterTableBase; //ָ����һ���������ñ������ÿ�����������õĴ���
    ULONG NumberOfService; //��ǰϵͳ��֧�ֵķ������
    ULONG ParamTableBase; //������ÿ����������Ĳ����ֽ���
}SystemServiceDescriptorTable,*PSystemServiceDescriptorTable; 
extern "C" PSystemServiceDescriptorTable KeServiceDescriptorTable; //����������DDK��ͷ�ļ��в�δ����

typedef NTSTATUS (*NTTERNIMATEPROCESS)( IN HANDLE  ProcessHandle,
IN NTSTATUS  ExitStatus );  //����һ������ָ��
NTTERNIMATEPROCESS pRealNtTerminateAddr;
ULONG RealServiceAddress;  //���ܱ�hook�ĺ�����ַ
CHAR *TerminateName = "notepad.exe";  //����������ǵļ��±�������

extern "C" __declspec(dllimport) UCHAR* PsGetProcessImageFileName( IN PEPROCESS Process );  

BOOLEAN IsProtect(CHAR *temp)  //�ж����ڽ����Ľ����Ƿ�������Ҫ�����ļ��±�����
{
    ULONG len = strcmp(TerminateName, temp);
    if(!len)	  
        return TRUE;
    return FALSE;
}

NTSTATUS MyNtTerminateProcess(IN HANDLE  ProcessHandle, IN NTSTATUS  ExitStatus)//�����Լ���NtTerminateProcess
{
    PEPROCESS process;  //����ͨ��ProcessHandle���صĽ���
    NTSTATUS status;  
    CHAR *pName;  //���ܽ��̵Ľ����� 
    status = ObReferenceObjectByHandle(ProcessHandle,FILE_READ_DATA,0
                                      ,KernelMode,(PVOID *)&process,NULL);  //��ȡ����

    if(!NT_SUCCESS(status))  
	return (NTSTATUS)(NTTERNIMATEPROCESS)pRealNtTerminateAddr(ProcessHandle, ExitStatus);
    pName = (CHAR*)PsGetProcessImageFileName(process);  //��ȡ������

    if(IsProtect(pName))  //�ж��Ƿ�������Ҫ�����Ľ���
	return STATUS_ACCESS_DENIED;
    return (NTSTATUS)(NTTERNIMATEPROCESS)pRealNtTerminateAddr(ProcessHandle, ExitStatus);
}

NTSTATUS Hook()
{
    ULONG Address;
    KdPrint(("entry Hook.........\n"));
    Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + 0x101 * 4;//0x101������windbg���
    RealServiceAddress = *(ULONG*)Address;  
    pRealNtTerminateAddr = (NTTERNIMATEPROCESS)RealServiceAddress;

    //����SSDT ��Ϊ��д
    _asm
    {
        cli;
        mov eax, cr0;
        and eax, not 10000h;
        mov  cr0, eax;
    }

    *((ULONG*)Address) = (ULONG)MyNtTerminateProcess;  //�滻Ϊ�Լ���NtTerminateProcess����
    //����SSDT��λֻ��
    _asm
    {
       mov eax, cr0;
       or eax, 10000h;
       mov cr0, eax;
       sti;  
    }

}
VOID UnHook()//��SSDT�е�NtTerminateprocess������ԭ
{
    ULONG Address;
    Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + 0x101 * 4;
    __asm 
    { 
        cli
        mov eax, cr0;
        and eax, not 10000h ;
        mov cr0, eax ;
    } 
    *((ULONG*)Address) = (ULONG)RealServiceAddress; 
    __asm 
    { 
        mov eax, cr0 ;
        or eax, 10000h ;
        mov cr0, eax ;
        sti;
    } 
}


VOID Unload(PDRIVER_OBJECT driver)
{
    UNREFERENCED_PARAMETER(driver);
    UnHook();
    DbgPrint(("Unload success........."));
}
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING str)
{
    NTSTATUS status;
    DbgPrint("hello ProcessProtect.......\n");
    UNREFERENCED_PARAMETER(str);
    driver->DriverUnload = Unload;
    status = Hook();
    return STATUS_SUCCESS;
}
