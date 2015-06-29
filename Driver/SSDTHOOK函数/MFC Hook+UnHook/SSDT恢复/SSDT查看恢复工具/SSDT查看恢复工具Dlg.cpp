// SSDT�鿴�ָ�����Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSDT�鿴�ָ�����.h"
#include "SSDT�鿴�ָ�����Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "..\\EXE\\SSDT.h"


extern ULONG NTBase;		//NT ��ַ
extern ULONG TotalSSDTCount;		//SSDT�������
extern ULONG RealCount;		//ö�����ҵ��ķ������
extern pSSDTSaveTable pSSDTST;	//�����SSDT�����

HANDLE hDriver = NULL;		//�������
BOOL bLoadSysSuccess = TRUE;

/////////////////////////////////////////////////////////////////////////////
// CSSDTDlg dialog

CSSDTDlg::CSSDTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSDTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSDTDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSDTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSDTDlg)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSDTDlg, CDialog)
	//{{AFX_MSG_MAP(CSSDTDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnReShowSSDT)
	ON_BN_CLICKED(IDC_BUTTON2, OnReSSDT)
	ON_BN_CLICKED(IDC_BUTTON3, OnReSSDTAndThrowSpilth)
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_LIST1, OnDrawColorForMyList )		//Ϊ�ı���ɫ��ӵ���Ϣ
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDTDlg message handlers

//��������
BOOL LoadMyDriver()
{
	//�õ������������·��
	TCHAR szDriver[MAX_PATH+1];
	::GetModuleFileName( NULL, szDriver, MAX_PATH );
	lstrcpy( _tcsrchr( szDriver, _T('\\') ) + 1, _T("SSDT.sys") );
	if( !PathFileExists( szDriver ) )
	{
		return FALSE;
	}
	hDriver = LoadDriver( szDriver );
	if( INVALID_HANDLE_VALUE == hDriver )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CSSDTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if( !LoadMyDriver() )
	{
		bLoadSysSuccess = FALSE;
		MessageBox( "�����������ʧ��,��ȷ������ͬһĿ¼��!", "����", MB_OK );
	}
	else
	{
		ShowSSDT();		//��ʾSSDT
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSDTDlg::OnPaint() 
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
	if( !bLoadSysSuccess )		//��������ʧ��
	{
		OnOK();		//�����˳�����
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSSDTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL bIsFirst = TRUE;

void CSSDTDlg::ShowSSDT()
{
	ULONG i;

	//ö��SSDT
	if( !EnumSSDT( hDriver ) )
	{
		//ж������
		if( hDriver )
		{
			UnloadDriver( hDriver );
			hDriver = NULL;
		}
		MessageBox( "ö��SSDTʧ��!", "����", MB_OK );
		OnOK();		//�����˳�����
	}
	if( TotalSSDTCount == -1 )
	{
		return ;
	}

	if( bIsFirst )
	{
		//�����
		m_List.InsertColumn( 0, "�����", LVCFMT_CENTER, 50, -1 );
		m_List.InsertColumn( 1, "��ǰ��ַ", LVCFMT_CENTER, 100, -1 );
		m_List.InsertColumn( 2, "ԭʼ��ַ", LVCFMT_CENTER, 100, -1 );
		m_List.InsertColumn( 3, "��������", LVCFMT_LEFT, 250, -1 );
		m_List.InsertColumn( 4, "ģ����", LVCFMT_LEFT, 300, -1 );
		
		bIsFirst = FALSE;
	}
	//ʹ���ѡ��һ��
	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	//�������
	char szIndex[10];
	char szCurAddr[18];
	char szOrgAddr[18];

	for( i = 0; i < TotalSSDTCount; i ++ )
	{
		sprintf( szIndex, "0x%04X", \
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber & 0xFFFF \
			);
		sprintf( szCurAddr, "0x%08X", \
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			);
		sprintf( szOrgAddr, "0x%08X", \
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			);
		m_List.InsertItem( m_List.GetItemCount(), szIndex );
		m_List.SetItemText( m_List.GetItemCount()-1, 1, szCurAddr );
		m_List.SetItemText( m_List.GetItemCount()-1, 2, szOrgAddr );
		m_List.SetItemText( m_List.GetItemCount()-1, 3, \
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ServiceFunctionName \
			);
		m_List.SetItemText( m_List.GetItemCount()-1, 4, \
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ModuleName \
			);
	}
}

void CSSDTDlg::OnReShowSSDT() 
{
	// TODO: Add your control notification handler code here
	m_List.DeleteAllItems();
	ShowSSDT();
}

BOOL CSSDTDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//ж������
	if( hDriver )
	{
		UnloadDriver( hDriver );
		hDriver = NULL;
	}
	
	return CDialog::DestroyWindow();
}

void CSSDTDlg::OnReSSDT() 
{
	// TODO: Add your control notification handler code here
	//ȷ��
	if( IDYES == MessageBox( "�ò�����һ����Σ����,�Ƿ����?", "�ָ�SSDT��ʾ", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//�ָ�SSDT
		if( ReSSDT( hDriver ) )
		{
			MessageBox( "�ָ�SSDT�ɹ�!", "�ɹ�", MB_OK );
		}
		else
		{
			MessageBox( "�ָ�SSDTʧ��!", "ʧ��", MB_OK );
		}
		//�ػ�
		OnReShowSSDT();
	}
}

void CSSDTDlg::OnReSSDTAndThrowSpilth() 
{
	// TODO: Add your control notification handler code here
	//ȷ��
	if( IDYES == MessageBox( "�ò�����һ����Σ����,�Ƿ����?", "�ָ�SSDT��������ʾ", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//�ָ�SSDT,��ɾ����ϵͳSSDT����
		if( ReSSDTAndThrowSpilth( hDriver ) )
		{
			MessageBox( "�ָ�SSDT��ɾ����ϵͳSSDT�����ɹ�!", "�ɹ�", MB_OK );
		}
		else
		{
			MessageBox( "�ָ�SSDT��ɾ����ϵͳSSDT����ʧ��!", "ʧ��", MB_OK );
		}
		//�ػ�
		OnReShowSSDT();
	}
}
//�ı� m_List �ؼ����е���ɫ
void CSSDTDlg::OnDrawColorForMyList( NMHDR *pNmHdr, LRESULT *pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNmHdr );

    *pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor, clrNewBkColor;
        
		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		//�����ǰSSDT��ַ��ԭʼ��ַ��һ��,����ʾ����Ϊ��ɫ,����Ϊ��ɫ
		if( \
			((pSSDTSaveTable)((ULONG)pSSDTST + nItem * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			!= ((pSSDTSaveTable)((ULONG)pSSDTST + nItem * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			)		//����Ϊ��ɫ
		{
			clrNewTextColor = RGB( 255, 0, 0 );
		}
		else		//����Ϊ��ɫ
		{
			clrNewTextColor = RGB( 0, 0, 0 );
		}

		//���ñ���ɫ
		if( nItem%2 ==0 )
		{
			clrNewBkColor = RGB( 240, 240, 240 );	//ż���б���ɫΪ��ɫ
		}
		else
		{
			clrNewBkColor = RGB( 255, 255, 255 );	//�����б���ɫΪ��ɫ
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

        *pResult = CDRF_DODEFAULT;
	}
}
