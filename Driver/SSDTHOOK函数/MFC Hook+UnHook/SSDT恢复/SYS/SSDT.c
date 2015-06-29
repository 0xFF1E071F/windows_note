/*
 *                                    SSDT��������
 *                                     Made By Adly
 *
 *                                   QQ:369046978
 *
 */
/////////////////////////////////////////////////////////////////////////
//SSDT.c

#include <ntddk.h>
#include <ntstatus.h>
#include "..\\IOCTL.h"

extern PSSDT    KeServiceDescriptorTable;

	
NTSTATUS SSDTDeviceIoCtl( PDEVICE_OBJECT pDeviceObject, PIRP Irp )
{
	NTSTATUS s;
	PIO_STACK_LOCATION IrpStack;
	PVOID InputBuffer;
	PVOID OutputBuffer;
	ULONG InputBufferLength;
	ULONG OutputBufferLength;
	ULONG IoControlCode;
		
	s = Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	
	IrpStack = IoGetCurrentIrpStackLocation( Irp );
	
	InputBuffer = IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
	InputBufferLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
	OutputBuffer = Irp->UserBuffer;
	OutputBufferLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
	
	///////////////////////////////////////////////
	//���ﴦ��ַ�����
	switch( IoControlCode )
	{
	//*************************************************
	case IOCTL_GETSSDT:	//�õ�SSDT
		__try
		{
			ProbeForWrite( OutputBuffer, sizeof( SSDT ), sizeof( ULONG ) );
			RtlCopyMemory( OutputBuffer, KeServiceDescriptorTable, sizeof( SSDT ) );
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			s = GetExceptionCode();
			break;
		}
		DbgPrint( "SSDT: GetSSDT Completeled!" );
		break;
	//*************************************************
	case IOCTL_SETSSDT: //���� SSDT
		__try
		{
			ProbeForRead( InputBuffer, sizeof( SSDT ), sizeof( ULONG ) );
			//ȥ���ڴ汣��
			__asm
			{
				cli		;//���ж�
				mov eax, cr0
				and eax, ~0x10000
				mov cr0, eax
			}
			RtlCopyMemory( KeServiceDescriptorTable, InputBuffer, sizeof( SSDT ) );
			//���ж�,���ڴ汣������
			 __asm
			 {
				mov eax, cr0
				or eax, 0x10000
				mov cr0, eax
				sti		;//���ж�
			 }
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			s = GetExceptionCode();
			break;
		}
		DbgPrint( "SSDT: SetSSDT Completeled!" );
		break;
	//*************************************************
	case IOCTL_GETHOOK:	//��ѯSSDTָ����ַ
		__try
		{
			ProbeForRead( InputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
			ProbeForWrite( OutputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			s = GetExceptionCode();
			break;
		}
		//���Դ���Ĳ����Ƿ���ȷ
		if( KeServiceDescriptorTable->ulNumberOfServices <= *(PULONG)InputBuffer )
		{
			s = STATUS_INVALID_PARAMETER;
			break;
		}
		//����������û����λ��
		*((PULONG)OutputBuffer) = *( (PULONG)(KeServiceDescriptorTable->pvSSDTBase) + *(PULONG)InputBuffer );
		DbgPrint( "SSDT: GetHookedAddress Completeled!" );
		break;
	//*************************************************
	case IOCTL_SETHOOK:	//����SSDTָ����ַ
		__try
		{
			ProbeForRead( InputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
			ProbeForRead( OutputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			s = GetExceptionCode();
			break;
		}
		//���Դ���Ĳ����Ƿ���ȷ
		if( KeServiceDescriptorTable->ulNumberOfServices <= *(PULONG)InputBuffer )
		{
			s = STATUS_INVALID_PARAMETER;
			break;
		}
		//�ڴ˽�����������������뻺��������,����ָ��SSDT HOOK�ĵ�ֵַ
		//ȥ���ڴ汣��
		__asm
		{
			cli		;//���ж�
			mov eax, cr0
			and eax, ~0x10000
			mov cr0, eax
		}
		 *( (PULONG)(KeServiceDescriptorTable->pvSSDTBase) + *(PULONG)InputBuffer ) = *((PULONG)OutputBuffer);
		 //���ж�,���ڴ汣������
		 __asm
		 {
			mov eax, cr0
			or eax, 0x10000
			mov cr0, eax
			sti		;//���ж�
		 }
		 DbgPrint( "SSDT: SetHookedAddress Completeled!" );
		break;
	//*************************************************
	default:
		s = STATUS_INVALID_DEVICE_REQUEST;
		DbgPrint( "SSDT: Invalid Parameter Completeled!" );
		break;
	}
	///////////////////////////////////////////////
	
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	
	return s;
}

void SSDTUnload( PDRIVER_OBJECT pDriverObject )
{
	UNICODE_STRING	usDosDeviceName;
	
	RtlInitUnicodeString( &usDosDeviceName, DEVICE_NAME );
	IoDeleteSymbolicLink( &usDosDeviceName );
	IoDeleteDevice( pDriverObject->DeviceObject );
	
	DbgPrint( "SSDT: Unload Success!" );
}

NTSTATUS SSDTCreate( IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp )
{
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	DbgPrint( "SSDT: Create Success!" );
	return STATUS_SUCCESS;
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
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] \
			= SSDTDeviceIoCtl;
		pDriverObject->DriverUnload = SSDTUnload;
		
		IoCreateSymbolicLink( &usDosDeviceName, &usDriverName );
	}
	DbgPrint( "SSDT: Load Success!" );
	
	return s;
}