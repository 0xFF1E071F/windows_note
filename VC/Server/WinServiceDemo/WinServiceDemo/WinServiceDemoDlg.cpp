// WinServiceDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinServiceDemo.h"
#include "WinServiceDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWinServiceDemoDlg �Ի���




CWinServiceDemoDlg::CWinServiceDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinServiceDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinServiceDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinServiceDemoDlg, CDialog)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CWinServiceDemoDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDOK, &CWinServiceDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWinServiceDemoDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SHOWTASK, &CWinServiceDemoDlg::OnShowTask)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CWinServiceDemoDlg ��Ϣ�������

BOOL CWinServiceDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_bMsbox = FALSE;
	this->SetWindowText("���񱣻�����");
	memset(m_szTipShowInfo,0,sizeof(m_szTipShowInfo));
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	wsprintf(m_szTipShowInfo, "���񱣻�����\n%04d-%02d-%02d %02d:%02d Start", 
	sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//void CWinServiceDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialog::OnSysCommand(nID, lParam);
//	}
//}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWinServiceDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CWinServiceDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWinServiceDemoDlg::OnBnClickedButtonCheck()
{
	this->UpdateData();
	CString strService;
	this->GetDlgItem(IDC_EDIT_SERVICE_NAME)->GetWindowText(strService);
	if(strService=="")
	{
		MessageBox("��������������ƣ�");
		return;
	}
	BYTE bRet = m_Service.CheckServiceStatus(strService);
	switch(bRet)
	{
	case m_Service.SEV_ERROR:
		MessageBox("�������");
		break;
	case m_Service.SEV_NO:
		MessageBox("û�и÷���");
		break;
	case m_Service.SEV_RUNING:
		MessageBox("������������");
		break;
	case m_Service.SEV_STOPED:
		MessageBox("����ֹͣ");
		break;
	default:
			break;
	}
}

void CWinServiceDemoDlg::OnBnClickedOk()
{
	m_bMsbox = FALSE;
	this->UpdateData();
	CString strTime;
	this->GetDlgItem(IDC_EDIT_CHECK_TIME)->GetWindowText(strTime);
	int nTime = 5000;
	
	if(atof(strTime)>0.001)
	{
		nTime = (int)(atof(strTime) *1000);
	}
	
	KillTimer(1);
	SetTimer(1,nTime,NULL);
}

void CWinServiceDemoDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	int iRet =MessageBox("�Ƿ�Ҫ�˳����񱣻�����",0,MB_YESNO);

	if(iRet == 7)
	{
		return;
	}
	KillTimer(1);
	OnCancel();
}

void CWinServiceDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	this->UpdateData();
	CString strService;
	this->GetDlgItem(IDC_EDIT_SERVICE_NAME)->GetWindowText(strService);
	if(strService=="")
	{
		MessageBox("��������������ƣ�");
		return;
	}
	if(m_Service.SEV_NO ==  m_Service.CheckServiceStatus(strService))
	{
		if(!m_bMsbox)
		{
			MessageBox("û�и÷������Ʋ��ܱ�����");
			m_bMsbox = TRUE;
		}
		return;
	}
	m_bMsbox = FALSE;
	if(m_Service.SEV_STOPED == m_Service.CheckServiceStatus(strService))
	{
		m_Service.StartSevice(strService);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CWinServiceDemoDlg::ShowIconToTip(BOOL bShow)
{
	if(bShow)
	{
		memset(&m_TipIcon, 0, sizeof(NOTIFYICONDATA));
		m_TipIcon.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
		m_TipIcon.hWnd=this->m_hWnd;
		m_TipIcon.uID=IDR_MAINFRAME;
		m_TipIcon.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
		m_TipIcon.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ����
		m_TipIcon.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		strcpy(m_TipIcon.szTip,m_szTipShowInfo);
		Shell_NotifyIcon(NIM_ADD,&m_TipIcon);//�����������ͼ��
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_SHOWNORMAL);
		Shell_NotifyIcon(NIM_DELETE,&m_TipIcon);
	}

}
void CWinServiceDemoDlg::ShowBalloonTip(const char *pMsg, const char *pTitle, UINT uTimeout, DWORD dwInfoFlags)
{
	m_TipIcon.cbSize=sizeof(NOTIFYICONDATA);
	m_TipIcon.uFlags = NIF_INFO;
	m_TipIcon.uTimeout = uTimeout;
	m_TipIcon.dwInfoFlags = 1;
	strcpy(m_TipIcon.szInfo,pMsg);
	strcpy(m_TipIcon.szInfoTitle,pTitle);
	Shell_NotifyIcon(NIM_MODIFY,&m_TipIcon);//�����������ͼ��
}


LRESULT CWinServiceDemoDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ
{
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�
		{
			ShowIconToTip(FALSE);
		}
		break;
	case WM_LBUTTONDBLCLK://˫������Ĵ���
		{
			ShowIconToTip(FALSE);
		}
		break;
	}
	return 0;
}
void CWinServiceDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	
	if (nID == SC_MINIMIZE)
	{
		ShowIconToTip(TRUE);
		SendMessage(WM_SHOWTASK,0,0);
		ShowBalloonTip("��С��������");
		
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}

	
}
