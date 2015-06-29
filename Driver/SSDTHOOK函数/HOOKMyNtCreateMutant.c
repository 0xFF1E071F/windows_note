#include <ntddk.h>

typedef struct _SystemServiceDescriptorTable 
{ 
    PVOID    ServiceTableBase; //SSDT��Ļ���ַ
    PULONG    ServiceCounterTableBase; //ָ����һ���������ñ������ÿ�����������õĴ���
    ULONG    NumberOfService; //��ǰϵͳ��֧�ֵķ������
    ULONG    ParamTableBase; //������ÿ����������Ĳ����ֽ���
}SystemServiceDescriptorTable,*PSystemServiceDescriptorTable; 
extern PSystemServiceDescriptorTable KeServiceDescriptorTable; //����������DDK��ͷ�ļ��в�δ����


typedef NTSTATUS(*NTCREATEMUTANT)(
                                  IN PUNICODE_STRING DriverServiceName
                                  );

NTCREATEMUTANT pRealNtCreateMutantAddr;

ULONG RealServiceAddress;  //���ܱ�hook�ĺ�����ַ


NTSTATUS MyNtCreateMutant(
                          IN PUNICODE_STRING DriverServiceName
                          )
                          
//�����Լ���NtCreateMutant
{

 DriverServiceName=DriverServiceName+1;
    // MutantHandle=MutantHandle+1;//α��һ������ľ��
     //return STATUS_ACCESS_DENIED;//����ֱ�ӷ���
  

  return (NTSTATUS)(NTCREATEMUTANT)pRealNtCreateMutantAddr( DriverServiceName);
      
}

NTSTATUS Hook()
{
  ULONG Address;

  Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + 97* 4;//43������xt���߲鿴
  RealServiceAddress = *(ULONG*)Address;  
  pRealNtCreateMutantAddr = (NTCREATEMUTANT)RealServiceAddress;

  //����SSDT ��Ϊ��д
  _asm
  {
    cli;
    mov  eax, cr0;
    and eax, not 10000h;
    mov  cr0, eax;
  }

  *((ULONG*)Address) = (ULONG)MyNtCreateMutant;  //�滻Ϊ�����Լ���MyNtCreateMutant����
  //����SSDT��λֻ��
  _asm
  {
    mov  eax, cr0;
    or  eax, 10000h;
    mov  cr0, eax;
    sti;  
  }
  
}
VOID UnHook()//��SSDT�е�NtCreateMutant������ԭ
{
  ULONG Address;
  
  Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + 97 * 4;

  
  __asm 
     { 
       cli 
       mov    eax, cr0;
       and    eax, not 10000h ;
       mov    cr0, eax ;
     } 
  
     *((ULONG*)Address) = (ULONG)RealServiceAddress; 
    
  __asm 
     { 
       mov    eax, cr0 ;
       or   eax, 10000h ;
       mov    cr0, eax ;
       sti ;
     } 
}



VOID Unload(PDRIVER_OBJECT driver)
{
  UNREFERENCED_PARAMETER(driver);
  UnHook();
  DbgPrint(("�����˳�"));
  
}
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING str)
{
  NTSTATUS status;
  UNREFERENCED_PARAMETER(str);
  driver->DriverUnload = Unload;

  status = Hook();
  return STATUS_SUCCESS;
}