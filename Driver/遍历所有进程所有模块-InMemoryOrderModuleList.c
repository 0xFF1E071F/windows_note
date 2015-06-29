
//�����ļ�   #include <ntifs.h>   #include <WinDef.h>  ��Ҫ�� #include <ntddk.h> ֮ǰ  ��ô�Ͳ������

typedef PPEB (__stdcall *PFNPsGetProcessPeb)(PEPROCESS pEProcess);    
  
  
typedef ULONG   PPS_POST_PROCESS_INIT_ROUTINE;      
  
  
typedef struct _PEB_LDR_DATA {    
    BYTE       Reserved1[8];    
    PVOID      Reserved2[3];    
    LIST_ENTRY InMemoryOrderModuleList;    
} PEB_LDR_DATA, *PPEB_LDR_DATA;    
  
  
typedef struct _RTL_USER_PROCESS_PARAMETERS {    
    BYTE           Reserved1[16];    
    PVOID          Reserved2[10];    
    UNICODE_STRING ImagePathName;    
    UNICODE_STRING CommandLine;    
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;    
  
  
typedef struct _PEB {    
    BYTE                          Reserved1[2];    
    BYTE                          BeingDebugged;    
    BYTE                          Reserved2[1];    
    PVOID                         Reserved3[2];    
    PPEB_LDR_DATA                 Ldr;    
    PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;    
    BYTE                          Reserved4[104];    
    PVOID                         Reserved5[52];    
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;    
    BYTE                          Reserved6[128];    
    PVOID                         Reserved7[1];    
    ULONG                         SessionId;    
} PEB, *PPEB;    
  
  
typedef struct _LDR_DATA_TABLE_ENTRY      
{      
    LIST_ENTRY InLoadOrderLinks;      
    LIST_ENTRY InMemoryOrderLinks;      
    LIST_ENTRY InInitializationOrderLinks;      
    PVOID DllBase;      
    PVOID EntryPoint;      
    DWORD SizeOfImage;      
    UNICODE_STRING FullDllName;      
    UNICODE_STRING BaseDllName;      
    DWORD Flags;      
    WORD LoadCount;      
    WORD TlsIndex;      
    LIST_ENTRY HashLinks;      
    PVOID SectionPointer;      
    DWORD CheckSum;      
    DWORD TimeDateStamp;      
    PVOID LoadedImports;      
    PVOID EntryPointActivationContext;      
    PVOID PatchInformation;      
}LDR_DATA_TABLE_ENTRY,*PLDR_DATA_TABLE_ENTRY;   

NTSTATUS ScanModulebypid(ULONG ulProcessId)    
{    
   
    //��ȡ���̵�EPROCESS�ṹָ��    
    NTSTATUS nStatus;    
    PEPROCESS  pEProcess = NULL;    
    nStatus = PsLookupProcessByProcessId((HANDLE)ulProcessId, &pEProcess);    
    if (!NT_SUCCESS(nStatus))    
    {    
        return STATUS_UNSUCCESSFUL;    
    }    
  
    //���Һ�����ַ        
    UNICODE_STRING uniFunctionName;    
    RtlInitUnicodeString(&uniFunctionName, L"PsGetProcessPeb");    
  
    //����ָ��    
    PFNPsGetProcessPeb  PsGetProcessPeb = NULL;   
  
    PsGetProcessPeb = (PFNPsGetProcessPeb)MmGetSystemRoutineAddress(&uniFunctionName);    
    if (PsGetProcessPeb == NULL)    
    {    
        KdPrint(("Get PsGetProcessPeb Failed~!\n"));    
        return STATUS_UNSUCCESSFUL;    
    }    
  
    //��ȡPEBָ��      
    PPEB pPEB = NULL;    
    pPEB = PsGetProcessPeb(pEProcess);    
  
    if (pPEB == NULL)    
    {    
        KdPrint(("Get pPEB Failed~!\n"));    
        return STATUS_UNSUCCESSFUL;    
    }    
  
    //����APC״̬  APC����Asynchronous procedure call���첽�������  
    KAPC_STATE KAPC ={0};    
    //���ӵ�����    
    KeStackAttachProcess(pEProcess, &KAPC);    
  
    //�Ƿ��Ѿ����ӵ�����    
    BOOLEAN bIsAttached = FALSE;    
  
    bIsAttached = TRUE;    
  
    //ָ��LDR    
    //LDR���ݽṹ    
    PPEB_LDR_DATA pPebLdrData = NULL;     
    pPebLdrData = pPEB->Ldr;    
  
    //����ͷ�ڵ㡢β�ڵ�    
    PLIST_ENTRY pListEntryStart = NULL;    
    PLIST_ENTRY pListEntryEnd = NULL;    
    //ͷ�ڵ㡢β�ڵ�    
    pListEntryStart = pListEntryEnd = pPebLdrData->InMemoryOrderModuleList.Flink;    
  
  
    //��ʼ����_LDR_DATA_TABLE_ENTRY    
    //LDR�������    
    PLDR_DATA_TABLE_ENTRY pLdrDataEntry = NULL;     
    do      
    {      
        //ͨ��_LIST_ENTRY��Flink��Ա��ȡ_LDR_DATA_TABLE_ENTRY�ṹ     
  
        pLdrDataEntry = (PLDR_DATA_TABLE_ENTRY)CONTAINING_RECORD(pListEntryStart,LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);      
  
        //���DLLȫ·��    
        KdPrint(("%wZ \n", &pLdrDataEntry->FullDllName));    
  
        pListEntryStart = pListEntryStart->Flink;      
  
    }while(pListEntryStart != pListEntryEnd);      
  
    KdPrint(("\n\n"));  
    //Detach����    
    if (bIsAttached != FALSE)    
    {    
        KeUnstackDetachProcess(&KAPC);    
    }    
  
    //�������ü���    
    if (pEProcess != NULL)    
    {    
        ObDereferenceObject(pEProcess);    
        pEProcess = NULL;    
    }    
  
    return STATUS_SUCCESS;    
}    
  
NTSTATUS ScanProcessModule()  
{  
    PEPROCESS eprocess,Firsteprocess;  
    ULONG ulProcessName;  
    ULONG ulProcessID;  
  
    KdPrint(("start ScanProcessModile\n"));  
    //__asm int 3;  
    eprocess = PsGetCurrentProcess();  
  
    Firsteprocess = eprocess;  
    if (eprocess == 0)  
    {  
        KdPrint(("PsGetCurrentProcess error"));  
        return STATUS_SUCCESS;  
    }  
    while (eprocess != NULL)  
    {  
        ulProcessName = (ULONG)eprocess + 0x174;        //   +0x174 ImageFileName    : [16] UChar  
        ulProcessID = *(ULONG*)((ULONG)eprocess + 0x84);//   +0x084 UniqueProcessId  : Ptr32 Void  
        KdPrint(("processname = %s,processid = %d \r \n",ulProcessName,ulProcessID));  
        //if (!strcmp( (char*)(ULONG*)ulProcessName ,"notepad.exe"))  
        //{  
            ScanModulebypid(ulProcessID);  
        //}  
          
                                                        //   +0x088 ActiveProcessLinks : _LIST_ENTRY  
        eprocess = (PEPROCESS)(*(ULONG*)((ULONG)eprocess + 0x88) - 0x88);//-0x88�ǵõ� eprocess�ṹ  
  
        if (eprocess == Firsteprocess || (*(LONG*)( (LONG)eprocess + 0x84) ) <0)//�������������ϻ��߽���PIDС��0�����  
        {  
            break;  
        }  
    }  
    return STATUS_SUCCESS;  
}  