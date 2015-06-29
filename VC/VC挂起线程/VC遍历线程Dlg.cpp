// VC�����߳�Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC�����߳�.h"
#include "VC�����߳�Dlg.h"


#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x400
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>
#include <PSAPI.H>
#include <Tlhelp32.h>
#include <string.h>
#pragma comment (lib, "psapi.lib")









CListBox *List;//����һ�� �����ָ�� List


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCDlg dialog

CVCDlg::CVCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVCDlg, CDialog)
	//{{AFX_MSG_MAP(CVCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCDlg message handlers

BOOL CVCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}








#pragma comment (lib, "psapi.lib")

//
// Thread Information Classes
//

typedef enum _THREADINFOCLASS {
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger,
    ThreadBreakOnTermination,
    MaxThreadInfoClass
    } THREADINFOCLASS;

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

typedef struct _THREAD_BASIC_INFORMATION { // Information Class 0
    LONG     ExitStatus;
    PVOID    TebBaseAddress;
    CLIENT_ID ClientId;
    LONG AffinityMask;
    LONG Priority;
    LONG BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

extern "C" LONG (__stdcall *ZwQueryInformationThread) (
    IN HANDLE ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    ) = NULL;


extern "C" LONG (__stdcall *RtlNtStatusToDosError) (
    IN  ULONG status) = NULL;

BOOL ShowThreadInfo (DWORD tid,DWORD pid, LPTSTR mymodname)
    {
        THREAD_BASIC_INFORMATION    tbi;
        PVOID                       startaddr;
        LONG                        status;
        HANDLE                      thread, process;
        
			typedef HANDLE (WINAPI *OPENTHREAD) (DWORD dwFlag, BOOL bUnknow, DWORD dwThreadId);  
OPENTHREAD OpenThread_1 = (OPENTHREAD)GetProcAddress(LoadLibrary("kernel32.dll"),"OpenThread");  


    

		//	HANDLE_2 = OpenThread_1(THREAD_SUSPEND_RESUME,true, THREADENTRY32_1.th32ThreadID);

        thread = OpenThread_1 (THREAD_ALL_ACCESS, FALSE, tid);
        if (thread == NULL)
            return FALSE;

        status = ZwQueryInformationThread (thread, 
            ThreadQuerySetWin32StartAddress, 
            &startaddr, 
            sizeof (startaddr), 
            NULL);

        if (status < 0)
        {
            CloseHandle (thread);
            SetLastError (RtlNtStatusToDosError (status));
            return FALSE;
        };

        _tprintf (TEXT ("�߳� %08x ����ʼ��ַΪ %p\n"), 
            tid, 
            startaddr);

        status = ZwQueryInformationThread (thread, 
            ThreadBasicInformation, 
            &tbi, 
            sizeof (tbi), 
            NULL);

        if (status < 0)
        {
            CloseHandle (thread);
            SetLastError (RtlNtStatusToDosError (status));
            return FALSE;
        };

        _tprintf (TEXT ("�߳� %08x ���ڽ���IDΪ %08x\n"), 
            tid, 
            (DWORD)tbi.ClientId.UniqueProcess);

        process = ::OpenProcess (PROCESS_ALL_ACCESS, 
            FALSE, 
            (DWORD)tbi.ClientId.UniqueProcess);

        if (process == NULL)
        {
            DWORD error = ::GetLastError ();
            CloseHandle (thread);
            SetLastError (error);
            return FALSE;
        };

       TCHAR modname [0x100];
       ::GetModuleFileNameEx (process, NULL, modname, 0x100);

       _tprintf (TEXT ("�߳� %08x ���ڽ���ӳ��Ϊ %s\n"), 
           tid, 
           modname);



typedef DWORD (WINAPI *MyGetMappedFileName)(HANDLE,LPVOID,LPTSTR,DWORD); //�̺߳��� 
 MyGetMappedFileName GetMappedFileName =(MyGetMappedFileName)GetProcAddress(LoadLibrary("psapi.dll"),"GetMappedFileNameA");

        GetMappedFileName(process, 
            startaddr, 
            modname, 
            0x100);

        _tprintf (TEXT ("�߳� %08x ��ִ�д�������ģ��Ϊ %s\n"), 
            tid, 
            modname);

		if (pid==(DWORD)tbi.ClientId.UniqueProcess && strstr(modname,mymodname)>0) //���������жϣ�����ϵͳ���е��̶߳�����ʾ������
		{
char  myb[260];        //�ַ���������ַ���
   sprintf(myb,"�߳�ID %i  �߳���ڵ�ַ%08X ����ģ��%s ",tid, startaddr,modname);   //ת���Ľ�����浽�ַ���myb

		SuspendThread(thread);//�����߳�



                List ->AddString(myb); //������������ָ�� �еĺ���AddString������ַ���


		
			
                //ResumeThread(thread);
		}

        CloseHandle (process);
        CloseHandle (thread);

        return TRUE;
    };


DWORD EnumProcess(char *ProcessName)
{
	PROCESSENTRY32 PROCESSENTRY32_1,PROCESSENTRY32_2;//�ṹ��(����ID,���Ƶ�10��)
	HANDLE HANDLE_1;
	BOOL BOOL_1;
	char *char_1;
	PROCESSENTRY32_1.dwSize = sizeof(PROCESSENTRY32_1);//��ʹ������ṹǰ�����������Ĵ�С
	HANDLE_1 = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//��ϵͳ�����еĽ����ĸ�����
	if (HANDLE_1 == INVALID_HANDLE_VALUE)//����INVALID_HANDLE_VALUE��ʾʧ��
	{
		printf("CreateToolhelp32Snapshot ����ʧ��.\n");
	 	return -1;//��ʾʧ��
	}
	BOOL_1 = ::Process32First(HANDLE_1,&PROCESSENTRY32_1);//ȡ�õ�һ�����յĽṹ��
	while (BOOL_1)//�������̿��գ�������ʾÿ�����̵���Ϣ
	{
	//	printf("�������ƣ�%s\n",PROCESSENTRY32_1.szExeFile);
	//	printf("����ID��%u\n\n",PROCESSENTRY32_1.th32ProcessID);
		char_1=PROCESSENTRY32_1.szExeFile;
		if (strcmp(char_1,ProcessName)==0)
		{
			PROCESSENTRY32_2.th32ParentProcessID=PROCESSENTRY32_1.th32ProcessID;
				return PROCESSENTRY32_2.th32ParentProcessID;
		}
		BOOL_1 = ::Process32Next(HANDLE_1,&PROCESSENTRY32_1);//ȡ����һ���յĽṹ��
	}
	::CloseHandle(HANDLE_1);//��Ҫ���������snapshot����
	return 0;
}





BOOL  GuaQiXianCheng (DWORD myPID, char *OKmodName)
{
       HINSTANCE hNTDLL = ::GetModuleHandle (TEXT ("ntdll"));

        (FARPROC&)ZwQueryInformationThread  =
            ::GetProcAddress (hNTDLL, "ZwQueryInformationThread");

        (FARPROC&)RtlNtStatusToDosError     =
            ::GetProcAddress (hNTDLL, "RtlNtStatusToDosError");

        HANDLE h = CreateToolhelp32Snapshot (TH32CS_SNAPTHREAD  , myPID);//
        THREADENTRY32 te;
        te.dwSize = sizeof (te);
        if (Thread32First (h, &te)) //�����߳�ID
        {
            do
            {
                if (ShowThreadInfo (te.th32ThreadID,myPID,OKmodName))//�����߳�ID����ȡ�̵߳���Ϣ
                {
                }
                else
                {
                    _tprintf (TEXT("�޷�����߳� %08x �������Ϣ���������Ϊ %d\n"), 
                        te.th32ThreadID, GetLastError ());
                };
            } while (Thread32Next (h, &te));
        };
        CloseHandle (h);
	return 0;
}





void CVCDlg::OnButton1() 
{
         List=(CListBox*)GetDlgItem(IDC_LIST1);//ʵ������������ָ�� List
        char mystr[260];//����һ���ַ������� mystr
        GetDlgItemText(IDC_EDIT1 ,mystr,260);//��ȡ �༭���ϵ����� ���浽mystr����ַ�����������

			 char okstr[260];//����һ���ַ������� mystr
        GetDlgItemText(IDC_EDIT2 ,okstr,260);//��ȡ �༭���ϵ����� ���浽mystr����ַ�����������


       	DWORD PID=EnumProcess(mystr);//���ݽ���������ȡ���̵�PIDֵ
    GuaQiXianCheng (PID,okstr);//���Ŀ����̵��߳�


  
}
