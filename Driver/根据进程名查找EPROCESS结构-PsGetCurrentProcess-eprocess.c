//���ݽ���������EPROCESS�ṹ
NTSTATUS LookUpProcessByName(IN PCHAR pcProcessName,OUT PEPROCESS *pEPROCESS)
{
	NTSTATUS status=STATUS_SUCCESS;
	PEPROCESS pCurrentEprocess=NULL;
	PLIST_ENTRY pStartListEntry=NULL;//����ʼ
	ULONG uCurrentId=0;
	ULONG uStartId=0;
	ULONG uCount=0;
	ULONG uLength=0;
	PEPROCESS pStartEprocess=PsGetCurrentProcess();
	//������ַ��Ч���
	if(!ARGUMENT_PRESENT(pcProcessName) ||! ARGUMENT_PRESENT(pEPROCESS))
	{
		return STATUS_INVALID_PARAMETER;
	}
	uStartId=*(PULONG)((ULONG)pStartEprocess+0x84);
	uCurrentId=uStartId;
	pCurrentEprocess=pStartEprocess;

	uLength = strlen(pcProcessName);//�õ����Ƚ��ַ�������

	while(1)
	{
		if(_strnicmp(pcProcessName,(PVOID)((ULONG)pCurrentEprocess+0x174),uLength)==0)
		{
			*pEPROCESS=pCurrentEprocess;
			DbgPrint("csrss����PID:%x\n",*(PULONG)((ULONG)pCurrentEprocess+0x84));
			status =STATUS_SUCCESS;
			break;
		}
		else if((uCount>=1) && (uCurrentId==uStartId))//�������
		{
			//DbgPrint("�������\n");
			*pEPROCESS = 0x00000000;
			status = STATUS_NOT_FOUND;
			break;
		}
		else
		{
			pStartListEntry=(PLIST_ENTRY *)((ULONG)pCurrentEprocess+0x88);

			DbgPrint("��ǰ�����Ľ�����:%s\n",(ULONG)pCurrentEprocess+0x174);

			(ULONG)pCurrentEprocess=(ULONG)pStartListEntry->Flink;
			(ULONG)pCurrentEprocess=(ULONG)pCurrentEprocess-0x88;
			uCurrentId=*(PULONG)((ULONG)pCurrentEprocess+0x84);
			uCount++;
		}
	}
	return status;
}