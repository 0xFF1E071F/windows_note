#ifndef CTL_CODE
#define CTL_CODE(DeviceType,Function,Method,Access)(\
	((DeviceType)<<16) | ((Access) << 14) | ((Function) << 2 ) | (Method)\
	)
#endif

#define IOCTL_UserInputBuffer CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x800,\
	METHOD_BUFFERED,FILE_ANY_ACCESS)
//��԰�ȫ�� pIrp->AssociatedIrp.SystemBuffer Ϊ�����������������  ��UCHARָ��ָ��
//�������������PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation( pIrp );
//ULONG inputlen = stack->parameters.DeviceIoControl.InputBufferLength;����   �����ں�ֱ̬�Ӳ����û�̬�ڴ��ַ������

// #define IOCTL_MethodNeither CTL_CODE(\
// 	FILE_DEVICE_UNKNOWN,\
// 	0x801,\
// 	METHOD_NEITHER,\
// 	FILE_ANY_ACCESS)
//��userbuffer��ȫ��ͬ��ֱ�Ӳ���R3�������ڴ�stack->param.deviceiocontrol.type3inputbuffer ���뻺���ַ ʹ��ǰ�� ProbeForRead
//pirp->userbuffer ���������  ProbeForWrite( 


#define IOCTL_IN_DRECT CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x802,\
	METHOD_IN_DIRECT,\
	FILE_ANY_ACCESS)
//ϵͳ��Ȼ��R3���� ���л��壬����R3���û�л��� �ں���������R3���������IO֮ǰ�����޷����ʵ� �����userbuffһ��
// Mmgetsystemaddressformdlsafe(pirp->mdladdress) ӳ����ں��ڴ��ַ Ȼ�����ֱ��д�� ���ս������
