#include "stdio.h"
#include "tchar.h"
#include "windows.h"

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO stSi;
	PROCESS_INFORMATION stPi;
	DEBUG_EVENT stDe;
	int nAddrOfBreakPoint = 0x0040E7F3; //��Ҫ�¶ϵ�ĵ�ַ
	//UPX   0040E7F3   .- E9 912BFFFF   jmp mengmeng.00401389 

	int nAddrOfPatch = 0x004010B8;    //��Ҫ�򲹶����ĵ�ַ
	unsigned char cOldByte;        //���ڱ���nAddrOfBreakPoint�ϵ㴦��ԭָ��
	unsigned char cBreakPoint = 0xCC;  //int 3ָ��
	unsigned char cPatchByte = 0x75;  //�����ֽ�
    CONTEXT stThreadContext;
	BOOL bCreated = FALSE;
	BOOL bFinished = FALSE;

	//LPTSTR szCmdLine = _tcsdup(TEXT("mengmeng.exe"));
	LPTSTR szCmdLine = _T("mengmeng.exe");
	GetStartupInfo(&stSi);
	bCreated = CreateProcess(NULL,szCmdLine,NULL,NULL,FALSE,DEBUG_ONLY_THIS_PROCESS,NULL,NULL,&stSi,&stPi);
	if(!bCreated){
		MessageBox(GetActiveWindow(),TEXT("���ܴ�test.exe�ļ���"),TEXT("Result"),MB_OK);
		//���ܴ�test.exeʱ�����Դ
		if(szCmdLine!=NULL)  free(szCmdLine);
		return 0;
	}
	//��ʼ�������У��ȴ������¼�����
	while(WaitForDebugEvent(&stDe,INFINITE)){//�е����¼�����
		switch(stDe.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
			//д�ϵ�(int 3[0xCC])��nAddrOfBreakPoint��֮ǰҪ����ԭʼָ��
			ReadProcessMemory(stPi.hProcess,(LPVOID)nAddrOfBreakPoint,&cOldByte,1,NULL);
			WriteProcessMemory(stPi.hProcess,(LPVOID)nAddrOfBreakPoint,&cBreakPoint,1,NULL);
			break;
		case EXCEPTION_DEBUG_EVENT:
			if(stDe.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
			{//�ϵ��쳣
				//��������nAddrOfBreakPoint���жϣ�������ȴ�
				stThreadContext.ContextFlags = CONTEXT_CONTROL;
				GetThreadContext(stPi.hThread,&stThreadContext);
				if(stThreadContext.Eip != nAddrOfBreakPoint+1) 
					break;        
				//ִ���ڴ油��
				WriteProcessMemory(stPi.hProcess,(LPVOID)0x004010B8,&cPatchByte,1,NULL);
				//�ָ�nAddrOfBreakPointԭʼָ�������ִ�и�ָ��
				WriteProcessMemory(stPi.hProcess,(LPVOID)nAddrOfBreakPoint,&cOldByte,1,NULL);
				stThreadContext.ContextFlags = CONTEXT_FULL;
				GetThreadContext(stPi.hThread,&stThreadContext);
				stThreadContext.Eip = nAddrOfBreakPoint;
				SetThreadContext(stPi.hThread,&stThreadContext);
			}
			break;
		case EXIT_PROCESS_DEBUG_EVENT:  
			bFinished = TRUE;
			
		}
		ContinueDebugEvent(stPi.dwProcessId,stPi.dwThreadId,DBG_CONTINUE);//�����ñ����Եĳ���ִ��
		if(bFinished)break;
	}
	//�ƽ�ɹ�������ڴ���Դ
	if(szCmdLine != NULL)  
		free(szCmdLine);
	CloseHandle(stPi.hThread);
	CloseHandle(stPi.hProcess);
	return 0;
}
