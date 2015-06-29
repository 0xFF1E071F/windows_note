/**************************************************************************************************/
//�õ�DLL�к����ĵ�ַ,��ȡ�ļ�����PE�ṹ��ʵ��
DWORD GetDllFunctionAddress(char* lpFunctionName,PUNICODE_STRING pDllName)
{
	HANDLE hThread, hSection, hFile, hMod;
	SECTION_IMAGE_INFORMATION sii;
	IMAGE_DOS_HEADER* dosheader;
	IMAGE_OPTIONAL_HEADER* opthdr;
	IMAGE_EXPORT_DIRECTORY* pExportTable;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
    //DWORD functionOrdinal;
	WORD functionOrdinal;
	DWORD Base, x, functionAddress;
	char* functionName;
	STRING ntFunctionName, ntFunctionNameSearch;
	PVOID BaseAddress = NULL;
	SIZE_T size=0;
	OBJECT_ATTRIBUTES oa = {sizeof oa, 0, pDllName, OBJ_CASE_INSENSITIVE};
	IO_STATUS_BLOCK iosb;
	
	ZwOpenFile(&hFile, FILE_EXECUTE | SYNCHRONIZE, &oa, &iosb, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
	oa.ObjectName = 0;

	ZwCreateSection(&hSection, SECTION_ALL_ACCESS, &oa, 0,PAGE_EXECUTE, SEC_IMAGE, hFile);

	ZwMapViewOfSection(hSection, NtCurrentProcess(), &BaseAddress, 0, 1000, 0, &size, (SECTION_INHERIT)1, MEM_TOP_DOWN, PAGE_READWRITE);

	ZwClose(hFile);
	/*

	IMAGE_NT_HEADERS STRUCT
        Signature DWORD ? ��PE �ļ���ʶ
        FileHeader IMAGE_FILE_HEADER <>
        OptionalHeader IMAGE_OPTIONAL_HEADER32 <>
    IMAGE_NT_HEADERS ENDS


	IMAGE_OPTIONAL_HEADER32 STRUCT
		Magic WORD ? ;0018h 107h��ROM Image,10Bh=exe Image
		MajorLinkerVersion BYTE ? ;001ah �������汾��
		MinorLinkerVersion BYTE ? ;001bh
		SizeOfCode DWORD ? ;001ch ���к�����Ľڵ��ܴ�С
		SizeOfInitializedData DWORD? ;0020h ���к��ѳ�ʼ�����ݵĽڵ��ܴ�С
		SizeOfUninitializedData DWORD ? ;0024h ���к�δ��ʼ�����ݵĽڵĴ�С
		AddressOfEntryPoint DWORD ? ;0028h ����ִ�����RVA
		BaseOfCode DWORD ? ;002ch ����Ľڵ���ʼRVA
		BaseOfData DWORD ? ;0030h ���ݵĽڵ���ʼRVA
		ImageBase DWORD ? ;0034h ����Ľ���װ�ص�ַ
		SectionAlignment DWORD ? ;0038h �ڴ��еĽڵĶ�������
		FileAlignment DWORD ? ;003ch �ļ��еĽڵĶ�������
		MajorOperatingSystemVersion WORD ? ;0040h ����ϵͳ���汾��
		MinorOperatingSystemVersion WORD ? ;0042h ����ϵͳ���汾��
		MajorImageVersion WORD ? ;0044h �������ڲ���ϵͳ����С�汾��
		MinorImageVersion WORD ? ;0046h
		MajorSubsystemVersion WORD ?;0048h �������ڲ���ϵͳ����С�Ӱ汾��
		MinorSubsystemVersion WORD ? ;004ah
		Win32VersionValue DWORD ? ;004ch δ��
		SizeOfImage DWORD ? ;0050h �ڴ�������PE ӳ��ߴ�
		SizeOfHeaders DWORD ? ;0054h ����ͷ���ڱ�Ĵ�С
		CheckSum DWORD ? ;0058h
		Subsystem WORD ? ;005ch �ļ�����ϵͳ
		DllCharacteristics WORD ? ;005eh
		SizeOfStackReserve DWORD ? ;0060h ��ʼ��ʱ�Ķ�ջ��С
		SizeOfStackCommit DWORD ? ;0064h ��ʼ��ʱʵ���ύ�Ķ�ջ��С
		SizeOfHeapReserve DWORD ? ;0068h ��ʼ��ʱ�����ĶѴ�С
		SizeOfHeapCommit DWORD ? ;006ch ��ʼ��ʱʵ���ύ�ĶѴ�С
		LoaderFlags DWORD ? ;0070h δ��
		NumberOfRvaAndSizes DWORD ? ;0074h ���������Ŀ¼�ṹ������
		DataDirectory IMAGE_DATA_DIRECTORY 16 dup(<>) ;0078h
	IMAGE_OPTIONAL_HEADER32 ENDS

	DataDirectory �ֶ�

	����ֶο���˵������Ҫ���ֶ�֮һ������16 ����ͬ��IMAGE_DATA_DIRECTORY �ṹ��ɣ���ȻPE �ļ���
	�������ǰ���װ���ڴ���ҳ���Թ���������ڲ�ͬ�Ľ��еģ�������Щ���ڸ������е����ݰ�����;��
	�Ա���Ϊ�������������Դ���ض�λ������ݿ飬��16 ��IMAGE_DATA_DIRECTORY �ṹ������������
	���ֲ�ͬ��;�����ݿ�ġ�IMAGE_DATA_DIRECTORY �ṹ�Ķ���ܼ򵥣�������ָ����ĳ
	�����ݿ��λ�úͳ��ȡ�
	IMAGE_DATA_DIRECTORY STRUCT
	   VirtualAddress DWORD ? �����ݵ���ʼRVA
	   Size DWORD ? �����ݿ�ĳ���
	IMAGE_DATA_DIRECTORY ENDS

	һ����ʮ����IMAGE_DATA_DIRECTORYENDS �ṹ
	IMAGE_DIRECTORY_ENTRY_EXPORT ������
	IMAGE_DIRECTORY_ENTRY_IMPORT �����
	IMAGE_DIRECTORY_ENTRY_RESOURCE ��Դ
	IMAGE_DIRECTORY_ENTRY_EXCEPTION �쳣���������ϲ��꣩
	IMAGE_DIRECTORY_ENTRY_SECURITY ��ȫ���������ϲ��꣩
	IMAGE_DIRECTORY_ENTRY_BASERELOC �ض�λ��
	IMAGE_DIRECTORY_ENTRY_DEBUG ������Ϣ
	IMAGE_DIRECTORY_ENTRY_ARCHITECTURE ��Ȩ��Ϣ
	IMAGE_DIRECTORY_ENTRY_GLOBALPTR �������ϲ���
	IMAGE_DIRECTORY_ENTRY_TLS Thread Local Storage
	IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG �������ϲ���
	IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT �������ϲ���
	IMAGE_DIRECTORY_ENTRY_IAT ���뺯����ַ��
	IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT �������ϲ���
	IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR �������ϲ���
	δʹ�ñ���

	Public Type IMAGE_EXPORT_DIRECTORY
		Characteristics As Long
		TimeDateStamp As Long
		MajorVersion As Integer
		MinorVersion As Integer
		Name1 As Long
		Base As Long
		NumberOfFunctions As Long
		NumberOfNames As Long
		AddressOfFunctions As Long
		AddressOfNames As Long
		AddressOfNameOrdinals As Long
    End Type

	*/
	hMod = BaseAddress;//����ַ
	dosheader = (IMAGE_DOS_HEADER *)hMod;//DOS�ļ�ͷ
	opthdr =(IMAGE_OPTIONAL_HEADER *) ((BYTE*)hMod+dosheader->e_lfanew+24);//hMod+dosheader->e_lfanewָ��PE�ļ�ͷ    +24ָ�� IMAGE_OPTIONAL_HEADER
	pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*) hMod + opthdr->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress);
	
	arrayOfFunctionAddresses = (DWORD*)( (BYTE*)hMod + pExportTable->AddressOfFunctions);
	arrayOfFunctionNames = (DWORD*)( (BYTE*)hMod + pExportTable->AddressOfNames);
	arrayOfFunctionOrdinals = (WORD*)( (BYTE*)hMod + pExportTable->AddressOfNameOrdinals);
	Base = pExportTable->Base;
	RtlInitString(&ntFunctionNameSearch, lpFunctionName);
	for(x = 0; x < pExportTable->NumberOfFunctions; x++)
	{
		functionName = (char*)( (BYTE*)hMod + arrayOfFunctionNames[x]);
		RtlInitString(&ntFunctionName, functionName);
		functionOrdinal = (WORD)(arrayOfFunctionOrdinals[x] + Base - 1); 
		functionAddress = (DWORD)( (BYTE*)hMod + arrayOfFunctionAddresses[functionOrdinal]);
		if (RtlCompareString(&ntFunctionName, &ntFunctionNameSearch, TRUE) == 0)
		{
			ZwClose(hSection);
			return functionAddress;
		}
	}
	ZwClose(hSection);
	return 0;
}
//�õ�������ַ
void GetSpecialFunctionAddr()
{
	UNICODE_STRING DllName;
	RtlInitUnicodeString(&DllName,L"\\Device\\HarddiskVolume1\\Windows\\System32\\kernel32.dll");

	g_pBufferForFunc = (PVOID)ExAllocatePoolWithTag(0, 0xDCu,'kdD');
	pPoolAddr = (int)g_pBufferForFunc;
	if ( pPoolAddr ) 
	{
		*(DWORD *)(pPoolAddr) = GetDllFunctionAddress( "LoadLibraryA", &DllName);
		DbgPrint("LoadLibraryA== %08X\n", *(DWORD *)(pPoolAddr));
	}
}