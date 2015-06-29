// SendToAspDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SendToAsp.h"
#include "SendToAspDlg.h"
//�������һ��ͷ�ļ�
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendToAspDlg dialog

CSendToAspDlg::CSendToAspDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendToAspDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendToAspDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendToAspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendToAspDlg)
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSendToAspDlg, CDialog)
	//{{AFX_MSG_MAP(CSendToAspDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendToAspDlg message handlers

BOOL CSendToAspDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CSendToAspDlg::OnPaint() 
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

HCURSOR CSendToAspDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSendToAspDlg::OnOK() 
{
	// �������Լ�д�Ĵ��벿�֣�������ȫΪMFC����
	CString Url="";
	CString content="";
	CInternetSession session; 
	INTERNET_PORT nPort = 80;
	CString serverName ="";

	m_Edit1.GetWindowText(Url);
	m_Edit2.GetWindowText(content);

	if(Url==""||content=="")
	{
		MessageBox("URL�����ݲ���Ϊ�գ�лл��","ע��",MB_OK);
		return;
	}

	//��װURL
	Url=Url+"?content="+content;

	serverName=_T(Url);
	//�����·��ͼ�������URL������Ϊ���ɱ༭����ʾ״̬��Ϣ:���ڷ���
	m_Edit1.SetReadOnly(TRUE);
	m_Edit2.SetReadOnly(TRUE);
	m_Edit3.SetWindowText("���ڷ�����Ϣ�����Ժ�");
	try{
		//��URL��������
		CHttpConnection* pConnection = session.GetHttpConnection(serverName,nPort); 
		//����ҳ��
		session.OpenURL(serverName,1,INTERNET_FLAG_TRANSFER_ASCII,NULL,0);
		//�ر�
		session.Close();
	}
	catch (CInternetException * e)
	{
		MessageBox("����������������Ƿ�����!���Ӳ��Ϸ�����.","ע��",MB_OK);
		return;
	}
	//��������ɺ�����URL������Ϊ�ɱ༭�����״̬��Ϣ
	m_Edit1.SetReadOnly(FALSE);
	m_Edit2.SetReadOnly(FALSE);
	m_Edit3.SetWindowText("");
	MessageBox("��Ϣ�Ѿ����ͳɹ�","��ϲ",MB_OK);
	m_Edit2.SetWindowText("");
}
