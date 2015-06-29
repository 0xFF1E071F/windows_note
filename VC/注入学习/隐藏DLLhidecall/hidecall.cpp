// hidecall.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

DWORD AlignUp(DWORD n, DWORD A)
{
  return ((n / A) +  ((n % A) ? 1 : 0)) * A;
}
HMODULE LoadStandardDLL(LPCSTR pszDLLName)
{
  char pwzFullPath[MAX_PATH*2];
//���DLL��ȫ��·��
  memset(pwzFullPath,0,MAX_PATH*2);
  GetSystemDirectory(pwzFullPath,MAX_PATH*2);
  strcat(pwzFullPath,pszDLLName);//C:\WINDOWS\system32\KERNEL32.dll
//ӳ���ļ����ڴ�
  HANDLE hFile=CreateFile(pwzFullPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(INVALID_HANDLE_VALUE==hFile)
  {
    return NULL;
  }
  HANDLE hMapFile=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,GetFileSize(hFile,NULL),NULL);
  if(NULL==hMapFile)
  {
    return NULL;
  }   
  PBYTE lpMapAddress=(PBYTE)MapViewOfFile(hMapFile,FILE_MAP_READ,0,0,0);    
  if(NULL==lpMapAddress)
  {
    return NULL;
  }
//����PE�ļ���ʽ
  PIMAGE_DOS_HEADER pDosHeader=(PIMAGE_DOS_HEADER)lpMapAddress;
  PIMAGE_NT_HEADERS pNtHeader=(PIMAGE_NT_HEADERS)((DWORD)pDosHeader+pDosHeader->e_lfanew);
  IMAGE_OPTIONAL_HEADER32 pOptionalHead = pNtHeader->OptionalHeader;
  PIMAGE_SECTION_HEADER pSecHeader=(PIMAGE_SECTION_HEADER)((DWORD)pNtHeader+sizeof(IMAGE_OPTIONAL_HEADER)+sizeof(IMAGE_FILE_HEADER)+sizeof(DWORD));
  DWORD dwSecAlign=pOptionalHead.SectionAlignment;
//�������ڴ�PE�Ŀռ�
  PBYTE pMemPE=(PBYTE)VirtualAlloc(NULL,pOptionalHead.SizeOfImage,MEM_COMMIT,PAGE_READWRITE);
  if(NULL==pMemPE)
  {
    return NULL;
  }
//����.���ڷ���ֵ
  PBYTE pbBase=(PBYTE)pMemPE;
//����PEͷ�������ڴ�
  memcpy(pMemPE,lpMapAddress,pOptionalHead.SizeOfHeaders);
  pMemPE+=4096;
//����������Ϣ.�����ڴ��е������ַ,���Ƶ������е������ַ.�����ַ�Ĵ�С�����������С����.
  for(int Index=0;Index<pNtHeader->FileHeader.NumberOfSections;Index++) //ע��PE�����ڴ��С����.
  {
    DWORD RawSize=pSecHeader[Index].SizeOfRawData;
    DWORD VirtualSize=pSecHeader[Index].Misc.VirtualSize;
    DWORD Align=AlignUp(VirtualSize,dwSecAlign);
    PBYTE pbPhysicalPE = (PBYTE)(pSecHeader[Index].PointerToRawData+lpMapAddress);
    memcpy(pMemPE,pbPhysicalPE,RawSize);
    pMemPE+=Align;
  }
//��������
  PIMAGE_EXPORT_DIRECTORY pExportTable=(PIMAGE_EXPORT_DIRECTORY)
    (pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + pbBase);

  DWORD dwExportSize=pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
  if(false!=dwExportSize && NULL!=pExportTable)
  {
    PBYTE *pbAddressArray=(PBYTE*)(pExportTable->AddressOfFunctions+pbBase);
    PBYTE *pbSerialIndex=(PBYTE*)(pExportTable->AddressOfNameOrdinals+pbBase);
    for(WORD Index = reinterpret_cast<WORD>(pbSerialIndex[0*2]); Index<pExportTable->NumberOfFunctions; Index++)
    {
      pbAddressArray[Index*1] = (pbAddressArray[Index*1]+(DWORD)pbBase);
    }
  }
//��������
  PIMAGE_IMPORT_DESCRIPTOR pImportTable=(PIMAGE_IMPORT_DESCRIPTOR)
    (pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress+pbBase);

  DWORD dwImportSize=pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
  if(false!=dwImportSize&&NULL!=pImportTable)
  {
    for(DWORD DllIndex=0;DllIndex<dwImportSize/sizeof(IMAGE_IMPORT_DESCRIPTOR);DllIndex++)
    {
      PBYTE *pbAddressArray=(PBYTE*)pImportTable->FirstThunk;
      if(NULL==pbAddressArray)
      {
        break;
      }
      pbAddressArray=(PBYTE*)(reinterpret_cast<DWORD>(pbAddressArray)+pbBase);
      LPCSTR pbNameArray=(LPCSTR)pImportTable->Name;
      if(NULL==pbNameArray)
      {
        break;
      }
      pbNameArray=(LPCSTR)((DWORD)pbNameArray+pbBase);
      PIMAGE_THUNK_DATA32 pThunkData=(PIMAGE_THUNK_DATA32)pImportTable->OriginalFirstThunk;
      if(NULL==pThunkData)
      {
        break;
      }
      pThunkData=(PIMAGE_THUNK_DATA32)((DWORD)pThunkData+pbBase);
      HMODULE hModule=LoadLibraryA(pbNameArray);
      PWORD pdwOrdinal=(PWORD)pThunkData->u1.Ordinal;
      for(DWORD NameIndex=0;false!=pdwOrdinal;NameIndex++)
      {
        PSZ pszProcName=(PSZ)((DWORD)pThunkData->u1.Function + pbBase+2);
        pbAddressArray[NameIndex*1]=(PBYTE)GetProcAddress(hModule,pszProcName);
        pThunkData++;
        pdwOrdinal=(PWORD)pThunkData->u1.Ordinal;
      }
      pImportTable++;
    }
  }
//��������
  if(lpMapAddress)
  {
    UnmapViewOfFile(lpMapAddress);
  }
  if(hMapFile)
  {
    CloseHandle(hMapFile);
  }
  if(hFile)
  {
    CloseHandle(hFile);
  }
  return (HMODULE)pbBase;
}
FARPROC GetStandardProc(HMODULE hModule,PSZ pszProcName)
{
  PBYTE pbBase=(PBYTE)hModule;
  PIMAGE_DOS_HEADER pDosHeader=(PIMAGE_DOS_HEADER)pbBase;
  PIMAGE_NT_HEADERS pNtHeader=(PIMAGE_NT_HEADERS)((DWORD)pDosHeader+pDosHeader->e_lfanew);
  IMAGE_OPTIONAL_HEADER32 pOptionalHead=pNtHeader->OptionalHeader;
  PIMAGE_EXPORT_DIRECTORY pExportTable=(PIMAGE_EXPORT_DIRECTORY)
    (pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress+pbBase);
  if(NULL==pExportTable)
  {
    return NULL;
  }
//���������Ҫ�ĸ�����Ϣ
  PBYTE *pbNameArray=(PBYTE*)(pExportTable->AddressOfNames+pbBase);
  PBYTE *pbAddressArray=(PBYTE*)(pExportTable->AddressOfFunctions+pbBase);
  PBYTE *pbSerialIndex=(PBYTE*)(pExportTable->AddressOfNameOrdinals+pbBase);
//�����������е�API��ַ.
  for(WORD Index=reinterpret_cast<WORD>(pbSerialIndex[0*2]);Index<pExportTable->NumberOfFunctions;Index++)
  {
    PSZ pszNameBase=(PSZ)pbNameArray[Index*1];
    pszNameBase=(reinterpret_cast<DWORD>(pbBase)+pszNameBase);
    if(false == stricmp(pszNameBase,pszProcName))
    {
      return (FARPROC)pbAddressArray[Index*1];
    }
  }
  return NULL;
}/*
ULONG FullImportTable()
{
  DWORD OldProtect;
  DWORD NewProtect;
  char pwzRelativeName[MAX_PATH];
  memset(pwzRelativeName,0,MAX_PATH);
  PBYTE pbBase=(PBYTE)GetModuleHandle(NULL);
  PIMAGE_DOS_HEADER pDosHeader=(PIMAGE_DOS_HEADER)pbBase;
  PIMAGE_NT_HEADERS pNtHeader=(PIMAGE_NT_HEADERS)((DWORD)pDosHeader+pDosHeader->e_lfanew);
  IMAGE_OPTIONAL_HEADER32 pOptionalHead=pNtHeader->OptionalHeader;
  PIMAGE_IMPORT_DESCRIPTOR pImportTable=(PIMAGE_IMPORT_DESCRIPTOR)
    (pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress+pbBase);

  DWORD dwImportSize = pOptionalHead.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
  DWORD SizeImage=pOptionalHead.SizeOfImage;
  VirtualProtect(pbBase,SizeImage,PAGE_EXECUTE_READWRITE,&OldProtect);
  if(NULL==pImportTable)
  {
    return GetLastError();
  }
//��õ�����ÿ��DLL����ĺ���.�������Ϊ�ڴ��еĵ�ַ.
  for(DWORD DllIndex=0;DllIndex<dwImportSize/sizeof(IMAGE_IMPORT_DESCRIPTOR);DllIndex++)
  {
    PBYTE *pbAddressArray=(PBYTE*)pImportTable->FirstThunk;
    if(NULL==pbAddressArray)
    {
      break;
    }
    pbAddressArray=(PBYTE*)(reinterpret_cast<DWORD>(pbAddressArray)+pbBase);
    LPCSTR pbNameArray=(LPCSTR)pImportTable->Name;
    if(NULL==pbNameArray)
    {
      break;
    }
    pbNameArray=(LPCSTR)((DWORD)pbNameArray+pbBase);
    PIMAGE_THUNK_DATA32 pThunkData=(PIMAGE_THUNK_DATA32)pImportTable->OriginalFirstThunk;
    if(NULL==pThunkData)
    {
      break;
    }
    pThunkData=(PIMAGE_THUNK_DATA32)((DWORD)pThunkData+pbBase);
    pwzRelativeName[0]=L'\\';
    for(BYTE LengthIndex=0,Index=1;LengthIndex<strlen(pbNameArray);LengthIndex++,Index++)
    {
      pwzRelativeName[Index*1]=pbNameArray[LengthIndex*1];
    }
    HMODULE hModule=LoadStandardDLL(pwzRelativeName);
    if(NULL!=hModule)
    {
      PWORD pdwOrdinal=(PWORD)pThunkData->u1.Ordinal;
      for(DWORD NameIndex=0;false!=pdwOrdinal;NameIndex++)
      {
        PSZ pszProcName=(PSZ)((DWORD)pThunkData->u1.Function +pbBase+2);
        pbAddressArray[NameIndex*1]=(PBYTE)GetStandardProc(hModule,pszProcName);
        pThunkData++;
        pdwOrdinal=(PWORD)pThunkData->u1.Ordinal;
      }
    }
    memset(pwzRelativeName,0,MAX_PATH);
    pImportTable++;
  }
  VirtualProtect(pbBase,SizeImage,OldProtect,&NewProtect);
  return false;
}*/
int  main()
{  
	//   FullImportTable();
	//   HMODULE hMod=LoadStandardDLL("\\Kernel32.dll");

  // void (WINAPI *MyExitProcess)(DWORD ExitCode);

  //*(DWORD*)&MyExitProcess=(DWORD)GetStandardProc(LoadStandardDLL("\\Kernel32.dll"),"ExitProcess");

 // MessageBox(NULL,"�볢��Dump","��ʾ",MB_OK);

 //MyExitProcess(0);

	typedef int (WINAPI *MYMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

	MYMessageBox MyMessageBox = (MYMessageBox)GetStandardProc(LoadStandardDLL("\\user32.dll"),"MessageBoxA");

	MyMessageBox(NULL,"OK","OK",MB_OK);
  return false;
}
