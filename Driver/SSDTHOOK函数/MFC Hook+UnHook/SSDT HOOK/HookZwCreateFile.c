
#include <ntddk.h>
#include <ntstatus.h>
#include "IOCTL.h"

#define		HOOK_SSDT_NUMBER		0x27	//ZwCreateFile��SSDT�е�λ��

ULONG ulCount = 0;

extern PSSDT    KeServiceDescriptorTable;

typedef NTSTATUS (*ZWCREATEFILE)(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
	IN ULONG  FileAttributes,
	IN ULONG  ShareAccess,
	IN ULONG  CreateDisposition,
	IN ULONG  CreateOptions,
	IN PVOID  EaBuffer  OPTIONAL,
	IN ULONG  EaLength
);

ZWCREATEFILE OldZwCreateFile = NULL;

//Hook ����º���
NTSTATUS
NewZwCreateFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
	IN ULONG  FileAttributes,
	IN ULONG  ShareAccess,
	IN ULONG  CreateDisposition,
	IN ULONG  CreateOptions,
	IN PVOID  EaBuffer  OPTIONAL,
	IN ULONG  EaLength
)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT file;
	OBJECT_HANDLE_INFORMATION info;
	
	//����ԭ����
	ntStatus = OldZwCreateFile(
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		AllocationSize,
		FileAttributes,
		ShareAccess,
		CreateDisposition,
		CreateOptions,
		EaBuffer,
		EaLength
	);
	
	DbgPrint("SSDT: ZwCreateFile Invoked (%u) !", ulCount++);
	
	/////////////////////// ͨ�� Handle �õ��ļ���
	ObReferenceObjectByHandle(ObjectAttributes->RootDirectory, 0, 0, \
		KernelMode, &file, &info );
	DbgPrint("SSDT: FileHandle %08X", FileHandle);
	if(file)
	{
		DbgPrint("SSDT: FileName = %ws%ws", \
			file->FileName, ObjectAttributes->ObjectName->Buffer);
		
		ObDereferenceObject(file);	//�������ü���
	}
	else
	{
		DbgPrint("SSDT: FileName = %ws",ObjectAttributes->ObjectName->Buffer);
	}
	
	return ntStatus;
}

void SSDTUnload( PDRIVER_OBJECT pDriverObject )
{
	UNICODE_STRING	usDosDeviceName;
	//ȥ���ڴ汣��
	__asm
	{
		cli		;//���ж�
		mov eax, cr0
		and eax, ~0x10000
		mov cr0, eax
	}

	//�ָ�SSDT�е� ZwWriteFile
	*( (PULONG)(KeServiceDescriptorTable->pvSSDTBase) + \
		(ULONG)HOOK_SSDT_NUMBER ) \
		= (ULONG)OldZwCreateFile;

	//���ж�,���ڴ汣������
	 __asm
	 {
		mov eax, cr0
		or eax, 0x10000
		mov cr0, eax
		sti		;//���ж�
	 }

	RtlInitUnicodeString( &usDosDeviceName, DEVICE_NAME );
	IoDeleteSymbolicLink( &usDosDeviceName );
	IoDeleteDevice( pDriverObject->DeviceObject );
	
	DbgPrint( "SSDT: Unload/UnHook Success!" );
}

NTSTATUS SSDTCreate( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp )
{
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;

	DbgPrint( "SSDT: Create Success!" );
	
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	
	return STATUS_SUCCESS;
}

NTSTATUS SSDTDeviceIoCtl( PDEVICE_OBJECT pDeviceObject, PIRP Irp )
{
	NTSTATUS s = STATUS_SUCCESS;
	
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	
	return s;
}

NTSTATUS DriverEntry(	PDRIVER_OBJECT pDriverObject,
						PUNICODE_STRING pRegistryPath )
{
	PDEVICE_OBJECT pdo = NULL;
	NTSTATUS s = STATUS_SUCCESS;
	UNICODE_STRING usDriverName, usDosDeviceName;
	
	RtlInitUnicodeString( &usDriverName, DRIVER_NAME );
	RtlInitUnicodeString( &usDosDeviceName, DEVICE_NAME );
	
	s = IoCreateDevice( pDriverObject, 0, &usDriverName, \
		FILE_DRIVER_SSDT, FILE_DEVICE_SECURE_OPEN, \
		FALSE, &pdo );
	
	if( STATUS_SUCCESS == s )
	{
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = SSDTCreate;
		pDriverObject->DriverUnload = SSDTUnload;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] \
			= SSDTDeviceIoCtl;
		
		IoCreateSymbolicLink( &usDosDeviceName, &usDriverName );
		
		DbgPrint( "SSDT: Load Success!" );
		
		DbgPrint( "SSDT: Hook ZwCreateFile Prepare!" );
	
		////////////////////////////////////////////////////////////////////
		//                                     ��ʼHOOK ZwWriteFile
		//ȥ���ڴ汣��
		__asm
		{
			cli		;//���ж�
			mov eax, cr0
			and eax, ~0x10000
			mov cr0, eax
		}

		//����ԭʼֵ
		(ULONG)OldZwCreateFile = \
			*( (PULONG)(KeServiceDescriptorTable->pvSSDTBase) + \
			(ULONG)HOOK_SSDT_NUMBER );

		//�޸�SSDT�е� ZwWriteFile ָ���º���
		*( (PULONG)(KeServiceDescriptorTable->pvSSDTBase) + \
			(ULONG)HOOK_SSDT_NUMBER ) \
			= (ULONG)NewZwCreateFile;

		//���ж�,���ڴ汣������
		__asm
		{
			mov eax, cr0
			or eax, 0x10000
			mov cr0, eax
			sti		;//���ж�
		}
		 ///////////////////////////////// HOOK ���
		 
		DbgPrint( "SSDT: Hook ZwCreateFile Success!" );
	}
	
	return s;
}