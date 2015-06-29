SSDT HOOK NtOpenProcessʵ��

#include ��ntddk.h��
#define NT_DEVICE_NAME L��\\Device\\ProtectProcess��
#define DOS_DEVICE_NAME L��\\DosDevices\\ProtectProcess��
#define IOCTL_PROTECT_CONTROL CTL_CODE(FILE_DEVICE_UNKNOWN, 0��800, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT DeviceObject,IN PIRP Irp);

VOID OnUnload(IN PDRIVER_OBJECT DriverObject);

#pragma pack(1) //SSDT��Ľṹ
typedef struct ServiceDescriptorEntry {
unsigned int *ServiceTableBase;
unsigned int *ServiceCounterTableBase; //Used only in checked build
unsigned int NumberOfServices;
unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

extern ��C�� __declspec(dllimport) ServiceDescriptorTableEntry_t KeServiceDescriptorTable; //�������ǲ��ܱ��,��Ϊ�Ǵ��ⲿ����
//����ǲ�ѯĳ�������ĵ�ַ��һ����
#define SYSCALL_INDEX(ServiceFunction) (*(PULONG)((PUCHAR)ServiceFunction + 1))

#define SYSCALL_FUNCTION(ServiceFunction) KeServiceDescriptorTable.ServiceTableBase[SYSCALL_INDEX(ServiceFunction)]

NTSYSAPI NTSTATUS NTAPI ZwOpenProcess(OUT PHANDLE ProcessHandle,IN ACCESS_MASK DesiredAccess,IN POBJECT_ATTRIBUTES ObjectAttributes,IN PCLIENT_ID ClientId OPTIONAL);
typedef NTSTATUS (*ZWOPENPROCESS)(OUT PHANDLE ProcessHandle,IN ACCESS_MASK DesiredAccess,IN POBJECT_ATTRIBUTES ObjectAttributes,IN PCLIENT_ID ClientId OPTIONAL);

ZWOPENPROCESS OldZwOpenProcess;
long pid = -1;

//���� SSDT(ϵͳ����������) �з�������������Ŀ
#define MAX_SYSTEM_SERVICE_NUMBER 1024
//�������� SSDT �����еľɵķ������ĵ�ַ
ULONG oldSysServiceAddr[MAX_SYSTEM_SERVICE_NUMBER];

//Descripion: ����ȥ���ڴ��ֻ���������Ӷ�ʵ�ֿ���д�ڴ�
VOID EnableWriteProtect()
{
__asm
{
cli;
mov eax, cr0;
and eax, not 10000h;
mov cr0, eax;
}
}

//Descripion: ����ȥ���ڴ�Ŀ�д���ԣ��Ӷ�ʵ���ڴ�ֻ��
VOID DisableWriteProtect()
{
__asm
{
mov eax, cr0;
or eax, 10000h;
mov cr0, eax;
sti;
}
}

VOID BackupSysServiceTable()
{
ULONG i;
for (i = 0; (i < KeServiceDescriptorTable.NumberOfServices) && (i < MAX_SYSTEM_SERVICE_NUMBER); i++)
{
//��������ַ
oldSysServiceAddr[i] = KeServiceDescriptorTable.ServiceTableBase[i];
KdPrint(("Function Information { Number: 0x%04X , Address: %08X}", i, oldSysServiceAddr[i]));
}
}

NTSTATUS InstallSysServiceHook(ULONG oldService, ULONG newService)
{
ULONG uOldAddr = 0;
EnableWriteProtect();
SYSCALL_FUNCTION(oldService) = newService;
DisableWriteProtect();

return STATUS_SUCCESS;
}

NTSTATUS UnInstallSysServiceHook(ULONG oldService)
{
ULONG uOldAddr = 0;
EnableWriteProtect();

SYSCALL_FUNCTION(oldService) = oldSysServiceAddr[SYSCALL_INDEX(oldService)];
DisableWriteProtect();
return STATUS_SUCCESS;
}

NTSTATUS NewZwOpenProcess(OUT PHANDLE ProcessHandle,IN ACCESS_MASK DesiredAccess,IN POBJECT_ATTRIBUTES ObjectAttributes,IN PCLIENT_ID ClientId OPTIONAL)
{
//�����滻���º���
NTSTATUS nStatus = STATUS_SUCCESS;

if((long)ClientId->UniqueProcess == pid)
{
//�ж��൱���ʾӦ�ò�������������openprocess�򿪽��̣�����ֱ�ӷ���
DbgPrint(���������� PID:%ld\n��,pid);
return STATUS_ACCESS_DENIED;
}

//ʣ�µĽ������ǵ�ԭ����
//��ȡOldSysServiceAdde��
OldZwOpenProcess = (ZWOPENPROCESS)oldSysServiceAddr[SYSCALL_INDEX(ZwOpenProcess)];

nStatus = OldZwOpenProcess(ProcessHandle,DesiredAccess,ObjectAttributes,ClientId);

return STATUS_SUCCESS;
}

VOID OnUnload(IN PDRIVER_OBJECT DriverObject)
{
//ж��ʱ�����
UNICODE_STRING DeviceLinkString;
PDEVICE_OBJECT DeviceObjectTemp1=NULL;
PDEVICE_OBJECT DeviceObjectTemp2=NULL;

DbgPrint(����������ж�ء�\n��);

RtlInitUnicodeString(&DeviceLinkString,DOS_DEVICE_NAME);
IoDeleteSymbolicLink(&DeviceLinkString);
if(DriverObject)
{
DeviceObjectTemp1=DriverObject->DeviceObject;
while(DeviceObjectTemp1)
{
DeviceObjectTemp2=DeviceObjectTemp1;
DeviceObjectTemp1=DeviceObjectTemp1->NextDevice;
IoDeleteDevice(DeviceObjectTemp2);
}
}
DbgPrint(���豸�Ѿ�ж��\n��);

DbgPrint(���޸�SSDT��\n��);
UnInstallSysServiceHook((ULONG)ZwOpenProcess);

DbgPrint(������ж�����.\n��);
}

NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT DeviceObject,IN PIRP Irp)
{
//IRP_MJ_DEVICE_CONTROL����Ӧ����
NTSTATUS nStatus = STATUS_SUCCESS;
ULONG IoControlCode = 0;
PIO_STACK_LOCATION IrpStack = NULL;

long* inBuf = NULL;
char* outBuf = NULL;
ULONG inSize = 0;
ULONG outSize = 0;
PCHAR buffer = NULL;
PMDL mdl = NULL;

Irp->IoStatus.Status = STATUS_SUCCESS;
Irp->IoStatus.Information = 0;

IrpStack = IoGetCurrentIrpStackLocation(Irp);

switch(IrpStack->MajorFunction)
{
case IRP_MJ_CREATE:
DbgPrint(��IRP_MJ_CREATE ������\n��);
break;
case IRP_MJ_CLOSE:
DbgPrint(��IRP_MJ_CLOSE ������\n��);
break;
case IRP_MJ_DEVICE_CONTROL:
DbgPrint(��IRP_MJ_DEVICE_CONTROL ������\n��);
IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
switch(IoControlCode)
{
case IOCTL_PROTECT_CONTROL:
inSize = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
outSize = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
inBuf = (long*)Irp->AssociatedIrp.SystemBuffer;

pid = *inBuf;
DbgPrint(��===========================\n��);
DbgPrint(��IOCTL_PROTECT_CONTROL ������,ͨѶ�ɹ�!\n��);
DbgPrint(�����뻺������С: %d\n��,inSize);
DbgPrint(�������������С: %d\n��,outSize);
DbgPrint(�����뻺��������: %ld\n��,*inBuf);
DbgPrint(����ǰ��������ID: %ld\n��,pid);
DbgPrint(��===========================\n��);

strcpy((char*)Irp->UserBuffer,��OK!\n��);
break;
default:
break;
}
break;
default:
DbgPrint(��δ֪�����������\n��);
break;
}

nStatus = Irp->IoStatus.Status;

IoCompleteRequest(Irp,IO_NO_INCREMENT);

return nStatus;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,IN PUNICODE_STRING theRegistryPath)
{
//������ں���
NTSTATUS ntStatus = STATUS_SUCCESS;
UNICODE_STRING ntDeviceName;
UNICODE_STRING DeviceLinkString;
PDEVICE_OBJECT deviceObject = NULL;

DbgPrint(������������ء�\n��);

RtlInitUnicodeString( &ntDeviceName, NT_DEVICE_NAME );

ntStatus = IoCreateDevice(
DriverObject,
0,
&ntDeviceName,
FILE_DEVICE_UNKNOWN,
0,
FALSE,
&deviceObject );

if ( !NT_SUCCESS( ntStatus ) )
{
DbgPrint(���޷����������豸��);
return ntStatus;
}

RtlInitUnicodeString(&DeviceLinkString,DOS_DEVICE_NAME);
ntStatus=IoCreateSymbolicLink(&DeviceLinkString,&ntDeviceName);

if(!NT_SUCCESS(ntStatus))
{
return ntStatus;
}

DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchDeviceControl;
DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchDeviceControl;
DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
DriverObject->DriverUnload = OnUnload;

DbgPrint(�����������Ѿ�����\n��);

DbgPrint(���޸�SSDT��\n��);

//�޸� ZwOpenProcess ������ַ
BackupSysServiceTable();//����ϵͳ�����

InstallSysServiceHook((ULONG)ZwOpenProcess, (ULONG)NewZwOpenProcess);
DbgPrint(����������������.\n��);

return STATUS_SUCCESS;
}