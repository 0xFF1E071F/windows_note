
extern"C" __declspec(dllimport)UCHAR*PsGetProcessImageFileName(IN PEPROCESS Process);

#pragma PAGEDCODE
VOID GetPath()
{
	//**********************************************************************************************************************//
	PEPROCESS process;
	CHAR *pName;   //������
	process =PsGetCurrentProcess();
	pName = (CHAR*)PsGetProcessImageFileName(process); //��ȡ������
	KdPrint(("%s\n",pName));
	//**********************************************************************************************************************//
}