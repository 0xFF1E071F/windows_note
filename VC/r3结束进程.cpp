1.ģ�ⰴ��alt+f4:
���ȵ����ñ��رմ����ö���Ȼ��ģ�ⰴ��alt+f4

HWND hwin = FindWindow(NULL,"notepad");

SetForegroundWindow(hwin); 

keybd_event(18,MapVirtualKey(18,0),0,0); 
keybd_event(115,MapVirtualKey(115,0),0,0); 
keybd_event(115,MapVirtualKey(115,0),KEYEVENTF_KEYUP,0); 
keybd_event(115,MapVirtualKey(115,0),0,0); 
keybd_event(115,MapVirtualKey(115,0),KEYEVENTF_KEYUP,0); 
keybd_event(18,MapVirtualKey(18,0),KEYEVENTF_KEYUP,0); 
2.����WM_CLOSE��Ϣ�رմ���

HWND hWin = FindWindow(NULL, "notepad");
SendMessage(hWin, WM_CLOSE, 0, 0);
return 0;

3.���� TerminateProcess(HANDLE, ExitCode)��������,TerminateThread(HANDLE, ExitCode)�����߳�

4.�ҵ����̵��̣߳�PostThraedMessage ����

��������ַ������ȶ�Ҫ��ý��̾���������߳̾��
���̾���Ļ�ȡ
a.֪������pid: HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
b.֪����������notepad��:

1��ʹ��ToolHelp����ȡ
2��ʹ��PSAPI����ȡ
3) ʹ�� ntdll.dll �е�δ�ĵ����� NtQuerySystemInformation ֮��� API ����ȡ
ʹ��ToolHelp��ȡ��
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

DWORD WINAPI GetPIDbyName(LPTSTR lpName)
{
    HANDLE m_Snap = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe = {sizeof(pe)};
    DWORD ret = NULL;


    m_Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
    if (m_Snap == INVALID_HANDLE_VALUE) goto finally; 
    if (!Process32First(m_Snap, &pe)) goto finally;
    do 
    {
        if(!lstrcmpi(pe.szExeFile,lpName)) 
        {
           ret = pe.th32ProcessID;
           goto finally;
        }
    }while (Process32Next(m_Snap, &pe)); 
}

finally: {
if (m_Snap != INVALID_HANDLE_VALUE) 
CloseHandle(m_Snap);
}
return ret;
}

int main(int argc, char *argv[])
{
   DWORD Pid = 0;
   Pid = GetPIDbyName("notepad.exe");
   if(Pid <= 0)
      return -1;
}
ͬ����ȡ�̵߳�PIDҲ����ʹ������ķ�����Ȼ�����OpenThread ��ȡ�߳̾��