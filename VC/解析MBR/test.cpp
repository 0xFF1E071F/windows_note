// ParseMbr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>


#define BOOTRECORDSIZE 440

typedef struct _BOOTRECORD
{
    unsigned char BootRecord[BOOTRECORDSIZE];
}BOOTRECORD, *PBOOTRECORD;

#define DPTSIZE 64

typedef struct _DPT
{
    unsigned char Dpt[DPTSIZE];
}DPT, *PDPT;

#define DPTNUMBER 4

typedef struct _DP
{
    unsigned char BootSign;         // ������־
    unsigned char StartHsc[3];
    unsigned char PartitionType;    // ��������
    unsigned char EndHsc[3];
    ULONG         SectorsPreceding;     // ������֮ǰʹ�õ�������
    ULONG         SectorsInPartition;   // ��������������
}DP, *PDP;

typedef struct _MBR
{
    BOOTRECORD BootRecord;                  // ��������
    unsigned char ulSigned[4];              // Windows����ǩ��
    unsigned char sReserve[2];              // ����λ
    DPT        Dpt;                         // ������
    unsigned char EndSign[2];               // ������־
}MBR, *PMBR;

// ��ʾMBR����
VOID ShowMbr(HANDLE hDevice, PMBR pMbr)
{    
    DWORD dwRead = 0;
    ReadFile(hDevice, (LPVOID)pMbr, sizeof(MBR), &dwRead, NULL);
    
    for ( int i = 0; i < 512; i ++ )
    {
        printf("%02X ", ((BYTE *)pMbr)[i]);
        if ( ( i + 1 ) % 16 == 0 )
        {
            printf("\r\n");
        }
    }
}

// ����MBR
VOID ParseMbr(MBR Mbr)
{
    printf("������¼: \r\n");

    for ( int i = 0; i < BOOTRECORDSIZE; i ++ )
    {
        printf("%02X ", Mbr.BootRecord.BootRecord[i]);
        if ( ( i + 1 ) % 16 == 0 )
        {
            printf("\r\n");
        }
    }

    printf("\r\n");

    printf("����ǩ��: \r\n");
    for ( i = 0; i < 4; i ++ )
    {
        printf("%02X ", Mbr.ulSigned[i]);
    }

    printf("\r\n");
    
    printf("����������: \r\n");
    for ( i = 0; i < DPTSIZE; i ++ )
    {
        printf("%02X ", Mbr.Dpt.Dpt[i]);
        if ( ( i + 1 ) % 16 == 0 )
        {
            printf("\r\n");
        }
    }

    printf("\r\n");

    PDP pDp = (PDP)&(Mbr.Dpt.Dpt);
    for ( i = 0; i < DPTNUMBER; i ++ )
    {
        printf("������־: %02X ", pDp[i].BootSign);
        printf("��������: %02X", pDp[i].PartitionType);
        printf("\r\n");
        printf("������֮ǰ������: %d ", pDp[i].SectorsPreceding);
        printf("����������������: %d", pDp[i].SectorsInPartition);
        printf("\r\n");        
        printf("�÷����Ĵ�С: %f \r\n", (double)pDp[i].SectorsInPartition / 1024 * 512 / 1024 / 1024 );

        printf("\r\n \r\n");
    }


    printf("������־: \r\n");
    for ( i = 0; i < 2; i ++ )
    {
        printf("%02X ", Mbr.EndSign[i]);
    }

    printf("\r\n");
}

int main(int argc, char* argv[])
{
    // ������Ӳ���豸
    HANDLE hDevice = CreateFile("\\\\.\\PhysicalDrive0",
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    NULL);
    if ( hDevice == INVALID_HANDLE_VALUE )
    {
        printf("CreateFile Error %d \r\n", GetLastError());
        return -1;
    }

    MBR Mbr = { 0 };
    ShowMbr(hDevice, &Mbr);
    ParseMbr(Mbr);

    CloseHandle(hDevice);

	return 0;
}
