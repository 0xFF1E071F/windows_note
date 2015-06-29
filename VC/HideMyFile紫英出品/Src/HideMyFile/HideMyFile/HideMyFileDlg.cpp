
// HideMyFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HideMyFile.h"
#include "HideMyFileDlg.h"
#include "afxdialogex.h"

#include "ConfigManager.h"
#include "FileManager.h"
#include "DriverManager.h"
#include "LoginDlg.h"


CConfigManager g_ConfigManager;
CFileManager g_FileManager;
CDriverManager g_DriverManager;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHideMyFileDlg �Ի���




CHideMyFileDlg::CHideMyFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHideMyFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN_ICON);
    m_bIsVisable = TRUE;
    m_IsShowLoginDlg = FALSE;
}

void CHideMyFileDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_CONTROL, m_BtnControl);
    DDX_Control(pDX, IDC_BTN_ADD, m_BtnAdd);
    DDX_Control(pDX, IDC_BTN_MIN, m_BtnMinimize);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_FOLDER_PATH, m_FilePath);
    DDX_Control(pDX, IDC_BTN_SEARCH, m_BtnSearch);
}

BEGIN_MESSAGE_MAP(CHideMyFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_NCHITTEST()
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_NOTIFYICON, OnNotifyIcon)
    ON_STN_CLICKED(IDC_BTN_CLOSE, &CHideMyFileDlg::OnStnClickedBtnClose)
    ON_STN_CLICKED(IDC_BTN_MIN, &CHideMyFileDlg::OnStnClickedBtnMin)
    ON_STN_CLICKED(IDC_BTN_CONTROL, &CHideMyFileDlg::OnStnClickedBtnControl)
    ON_STN_CLICKED(IDC_BTN_ADD, &CHideMyFileDlg::OnStnClickedBtnAdd)
    ON_WM_WINDOWPOSCHANGING()
    ON_COMMAND(IDM_EXIT, &CHideMyFileDlg::OnExit)
    ON_NOTIFY(NM_RCLICK, IDC_FOLDER_LIST, &CHideMyFileDlg::OnNMRClickFolderList)
    ON_COMMAND(IDM_HIDE_FILE, &CHideMyFileDlg::OnHideFile)
    ON_COMMAND(IDM_SHOW_FILE, &CHideMyFileDlg::OnShowFile)
    ON_COMMAND(IDM_CANCEL_HIDE, &CHideMyFileDlg::OnCancelHide)
    ON_COMMAND(IDM_REFRESH_LIST, &CHideMyFileDlg::OnRefreshList)
    ON_COMMAND(IDM_EXPLOER_FILE, &CHideMyFileDlg::OnExploerFile)
    ON_STN_CLICKED(IDC_BTN_SEARCH, &CHideMyFileDlg::OnStnClickedBtnSearch)
END_MESSAGE_MAP()


// CHideMyFileDlg ��Ϣ�������

BOOL CHideMyFileDlg::OnInitDialog()
{
    //��ʵ������
    CheckSingleton();

	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

    EnableDebugPrv();

    //��ʼ���ؼ�
    InitializeControls();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CHideMyFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHideMyFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHideMyFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CHideMyFileDlg::OnNcHitTest(CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    UINT nHitTest = CDialog::OnNcHitTest(point);

    if (nHitTest == HTCLIENT)
    {
        return HTCAPTION;
    }
    else
    {
        return nHitTest;
    }
    //return CDialogEx::OnNcHitTest(point);
}


HBRUSH CHideMyFileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����
    if (pWnd == this)
    {
        return m_BkBrush;
    }

    if (CTLCOLOR_STATIC == nCtlColor)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)(GetStockObject(NULL_BRUSH));
    }

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}

void CHideMyFileDlg::InitializeControls()
{
    //��ʼ�����
    g_ConfigManager.Initialize();
    g_DriverManager.Initialize();
    g_FileManager.Initialize();

    //�������

    CString strCmdLine = L"";
    strCmdLine.Format(L"%s",AfxGetApp()->m_lpCmdLine);//��ȡ��������

    if (strCmdLine == L"/autorun")//���������������,���ش���
    {
        m_bIsVisable = FALSE;
        ShowWindow(SW_HIDE);
    }
    else
    {
        if (!CheckLogin()) //������֤
        {
            m_bIsVisable = FALSE;
            ShowWindow(SW_HIDE);
        }
    }

    //��������
    EnableDebugPrv();
    g_DriverManager.InstallDriver();
    g_DriverManager.StartDriverService();


    //��ӵ�����
    if (g_ConfigManager.GetConfigValueInt(CCM_TYPE_AUTOSTART) == 1)
    {
        if (g_ConfigManager.GetConfigValueInt(CCM_TYPE_SHOWTRAY) == 1)
        {
            AddToTrayIcon();
        }
    }


    //��������
    if (g_ConfigManager.GetConfigValueInt(CCM_TYPE_HIDEPROCESS) == 1)
    {
        CString strCurProcessName = g_DriverManager.GetCurrentProcessName();
        g_DriverManager.SetProcessHiddenState(strCurProcessName, TRUE);
    }

    //���ð�ť��ʾͼ��
    m_BtnClose.SetButtonImages(IDB_CLOSE_NORMAL, IDB_CLOSE_ACTIVE);
    m_BtnMinimize.SetButtonImages(IDB_MIN_NORMAL, IDB_MIN_ACTIVE);
    m_BtnControl.SetButtonImages(IDB_CONTROL_NORMAL, IDB_CONTROL_ACTIVE);
    m_BtnAdd.SetButtonImages(IDB_BTN_ADD_NORMAL, IDB_BTN_ADD_HOVER);
    m_BtnSearch.SetButtonImages(IDB_SEARCH_NORMAL, IDB_SEARCH_ACTIVE);

    //�Ի��򱳾�
    CBitmap bitmapBackgroud;
    bitmapBackgroud.LoadBitmapW(IDB_BACKGROUND);
    m_BkBrush.CreatePatternBrush(&bitmapBackgroud);
    bitmapBackgroud.DeleteObject();

    //·��������
    m_EditFont.CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, L"Microsoft YaHei");

    CEdit *pEdit = (CEdit*)GetDlgItem(IDC_FOLDER_PATH);
    pEdit->SetFont(&m_EditFont);
    SetDlgItemText(IDC_FOLDER_PATH,L"��ѡ��Ҫ���ص��ļ����ļ���...");
    pEdit->SetSel(-1,0);

    //�ļ����б��

    //����
    m_ListFont.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, L"Microsoft YaHei");

    CListCtrl *pFileList = (CListCtrl *)GetDlgItem(IDC_FOLDER_LIST);
    pFileList->SetFont(&m_ListFont);

    //��ͷ
    TCHAR tItemList[4][10] = {L"���",L"�ļ�����",L"�ļ�·��",L"��ǰ״̬"};
    int iListSize[4]={60,170,300,60};
    LV_COLUMN lvcolumn;
    pFileList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    for(int i=0;i<4;i++)
    {
        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn.fmt = LVCFMT_LEFT;
        lvcolumn.pszText = tItemList[i];
        lvcolumn.iSubItem = i;
        lvcolumn.iOrder = i;
        lvcolumn.cx = iListSize[i];
        pFileList->InsertColumn(i, &lvcolumn);
    }	
    
    //ע��ϵͳ�ȼ�  Ctrl + Shift + 'T'
    RegisterHotKey(GetSafeHwnd(), WM_HIDEHOTKEY, MOD_CONTROL|MOD_SHIFT, 'T');


    //��ȡ�ļ��б�
    ReadFileList();

}


void CHideMyFileDlg::OnStnClickedBtnClose()
{
    int nValue = g_ConfigManager.GetConfigValueInt(CCM_TYPE_CLOSESTATE);
    if (nValue == 0)
    {
        if (g_ConfigManager.GetConfigValueInt(CCM_TYPE_SHOWTRAY) == 1)
        {
            AddToTrayIcon();
        }
        ShowWindow(SW_HIDE);
    }
    else if (nValue == 1)
    {
        if (CheckLogin())//������֤
        {
            OnOK();
        }
    }
}


void CHideMyFileDlg::OnStnClickedBtnMin()
{
    ShowWindow(SW_MINIMIZE);
}


void CHideMyFileDlg::OnStnClickedBtnControl()
{
    m_ControlDlg = new CControlDlg();
    m_ControlDlg->Create(IDD_DLG_CONTROL, NULL);
    m_ControlDlg->ShowWindow(SW_SHOW);
}

void CHideMyFileDlg::OnStnClickedBtnSearch()
{
    TCHAR szBuffer[MAX_PATH] = {0};
    BROWSEINFO bInfo;
    bInfo.hwndOwner = this->GetSafeHwnd();
    bInfo.pidlRoot = NULL;
    bInfo.pszDisplayName = szBuffer;
    bInfo.lpszTitle = L"ѡ��Ҫ���ص��ļ����ļ���";
    bInfo.ulFlags = BIF_BROWSEINCLUDEFILES;
    bInfo.lpfn = NULL;
    bInfo.lParam = NULL;
    bInfo.iImage = NULL;

    ITEMIDLIST *idl;
    CoInitialize(NULL);
    idl = SHBrowseForFolder(&bInfo);
    if (idl)
    {
        TCHAR szPath[MAX_PATH] = {0};
        SHGetPathFromIDList(idl, szPath);
        SetDlgItemText(IDC_FOLDER_PATH, szPath);

        IMalloc *im;
        SHGetMalloc(&im);
        im->Free(idl);
    }
    CoUninitialize();
}

void CHideMyFileDlg::OnStnClickedBtnAdd()
{
    CString strFilePath;

    do 
    {
        //��ȡ�ļ�·��
        GetDlgItemText(IDC_FOLDER_PATH, strFilePath.GetBuffer(MAX_PATH), MAX_PATH);
        strFilePath.ReleaseBuffer();

        if (PathFileExists(strFilePath) && strFilePath.GetLength() > 3)
        {
            if (CheckExistFileInList(strFilePath))
            {
                MessageBox(L"�ļ��Ѿ��Ѿ����б�����Ӵ~~����һ���ɣ�^_^", L"��ʾ", MB_OK|MB_ICONINFORMATION);
                break;
            }
            //����ļ���Ŀ
            g_FileManager.AddFileItem(strFilePath);

            //ˢ���б�
            OnRefreshList();
        }
        else
        {
            MessageBox(L"�ļ�·��ò�Ʋ��԰ɣ������Կ�����^_^", L"��������", MB_OK|MB_ICONINFORMATION);
        }
    } while (0);
    
    SetDlgItemText(IDC_FOLDER_PATH,L"��ѡ��Ҫ���ص��ļ����ļ���...");
}

LRESULT CHideMyFileDlg::OnNotifyIcon( WPARAM wParam, LPARAM lParam )
{
    if (lParam == WM_LBUTTONDBLCLK)
    {
        if (!CheckLogin())
        {
            m_bIsVisable = FALSE;
            ShowWindow(SW_HIDE);
        }
        else
        {
            m_bIsVisable = TRUE;
            ShowWindow(SW_SHOW);
            CenterWindow();
        }
    }
    else if(lParam == WM_RBUTTONDOWN)
    {
        CMenu menu;
        menu.LoadMenuW(IDR_TRAY_MENU);
        CMenu *pMenu = menu.GetSubMenu(0);

        CPoint pt;
        GetCursorPos(&pt);
        SetForegroundWindow();//����հף��˵���ʧ

        pMenu->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this, 0);//��ʾ�˵�
    }
    return TRUE;
}

void CHideMyFileDlg::AddToTrayIcon()
{
    m_ntIcon.cbSize = sizeof(NOTIFYICONDATA);
    m_ntIcon.hIcon = AfxGetApp()->LoadIconW(IDI_MAIN_ICON);
    m_ntIcon.hWnd = m_hWnd;
    m_ntIcon.uCallbackMessage = WM_NOTIFYICON;
    m_ntIcon.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
    TCHAR tTips[128]=L"С��ר������~By С����(^-^)";
    wcscpy_s(m_ntIcon.szTip,128*sizeof(TCHAR),tTips);

    ::Shell_NotifyIcon(NIM_ADD,&m_ntIcon);
}


void CHideMyFileDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    if (!m_bIsVisable)
    {
        lpwndpos->flags &= ~SWP_SHOWWINDOW; 
    }

    CDialogEx::OnWindowPosChanging(lpwndpos);
}

void CHideMyFileDlg::OnExit()
{
    if (CheckLogin())
    {
        OnOK();
    }
}


void CHideMyFileDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���

    //����ȼ�
    UnregisterHotKey(GetSafeHwnd(), WM_HIDEHOTKEY);

    //ɾ������ͼ��
    Shell_NotifyIcon(NIM_DELETE,&m_ntIcon);

    //ж������
    g_DriverManager.StopDriverService();
    g_DriverManager.UninstallDriver();

    CDialogEx::OnOK();
}

void CHideMyFileDlg::ReadFileList()
{
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);
    int nItemCount = g_FileManager.GetFileItemCount();
    CString strID;
    CString strFileName;
    CString strFilePath;
    int  nHidden;

    g_DriverManager.ClearFileCount();

    int nIdx = 0;
    for (nIdx = 0; nIdx < nItemCount; ++nIdx)
    {
       strID.Format(L"%d", g_FileManager.GetItemFileId(nIdx));
       strFileName = g_FileManager.GetItemFileName(nIdx);
       strFilePath = g_FileManager.GetItemFilePath(nIdx);
       nHidden = g_FileManager.GetItemFileState(nIdx);

       pListCtrl->InsertItem(nIdx, strID);
       pListCtrl->SetItemText(nIdx, 1, strFileName);
       pListCtrl->SetItemText(nIdx, 2, strFilePath);
       if (nHidden == 1)
       {
            pListCtrl->SetItemText(nIdx, 3, L"����");
       }
       else if (nHidden == 0)
       {
            pListCtrl->SetItemText(nIdx, 3, L"��ʾ");
       }

       g_DriverManager.AddNewHiddenFile(strFileName);
       g_DriverManager.SetFileDriverHiddenState(strFileName, (BOOL)nHidden);
    }
}


void CHideMyFileDlg::OnNMRClickFolderList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    CMenu m_PopMenu;
    m_PopMenu.LoadMenuW(IDR_MAIN_MENU);
    CMenu *pMenu = m_PopMenu.GetSubMenu(0);
    
    CString strHidden;
    int nPos = pNMItemActivate->iItem;
    if (nPos != -1)
    {
        CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);
        pListCtrl->GetItemText(nPos, 3, strHidden.GetBuffer(20), 20);
        strHidden.ReleaseBuffer();
        if (strHidden == L"����")
        {
            pMenu->EnableMenuItem(IDM_HIDE_FILE, MF_BYCOMMAND|MF_GRAYED);
        }
        else if (strHidden == L"��ʾ")
        {
             pMenu->EnableMenuItem(IDM_SHOW_FILE, MF_BYCOMMAND|MF_GRAYED);
        }

    }
    else if (nPos == -1)
    {
        pMenu->EnableMenuItem(IDM_HIDE_FILE, MF_BYCOMMAND|MF_GRAYED);
        pMenu->EnableMenuItem(IDM_SHOW_FILE, MF_BYCOMMAND|MF_GRAYED);
        pMenu->EnableMenuItem(IDM_CANCEL_HIDE, MF_BYCOMMAND|MF_GRAYED);
        pMenu->EnableMenuItem(IDM_EXPLOER_FILE, MF_BYCOMMAND|MF_GRAYED);
    }

    CPoint pt;
    GetCursorPos(&pt);

    pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this, 0);

    *pResult = 0;
}


void CHideMyFileDlg::OnHideFile()
{
    //��ȡ�б��������ļ���Ŀ����
    int nIndex = GetSelectItemIndex();
    int nID  = GetFileItemID(nIndex);

     //�����б��е���ʾ
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);

    g_FileManager.SetFileListHiddenState(nID, TRUE);
    pListCtrl->SetItemText(nIndex, 3, L"����");

     //ȡ�ļ����� ֪ͨ������ʾ/�����ļ�
    CString strFileName;
    pListCtrl->GetItemText(nIndex, 1, strFileName.GetBuffer(MAX_PATH), MAX_PATH);
    strFileName.ReleaseBuffer();


    g_DriverManager.SetFileDriverHiddenState(strFileName, TRUE);

}


void CHideMyFileDlg::OnShowFile()
{
    //��ȡ�б��������ļ���Ŀ����
   int nIndex = GetSelectItemIndex();
   int nID  = GetFileItemID(nIndex);

   //�����б��е���ʾ
   CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);

    g_FileManager.SetFileListHiddenState(nID, FALSE);
    pListCtrl->SetItemText(nIndex, 3, L"��ʾ");

    //ȡ�ļ����� ֪ͨ������ʾ/�����ļ�
    CString strFileName;
    pListCtrl->GetItemText(nIndex, 1, strFileName.GetBuffer(MAX_PATH), MAX_PATH);
    strFileName.ReleaseBuffer();
    
    g_DriverManager.SetFileDriverHiddenState(strFileName, FALSE);
}


void CHideMyFileDlg::OnCancelHide()
{
    //��ȡ�б��������ļ���Ŀ����
    int nIndex = GetSelectItemIndex();
    int nID  = GetFileItemID(nIndex);
   
    //ɾ���ļ���Ŀ
    g_FileManager.DeleteFileItem(nID);

    //ˢ���б�
    OnRefreshList();
}


void CHideMyFileDlg::OnRefreshList()
{
    //����б�
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);
    pListCtrl->DeleteAllItems();

    //���¶�ȡ�б�
    ReadFileList();
}

int CHideMyFileDlg::GetSelectItemIndex()
{
    int nIndex;
    TCHAR szBuf[20] = {0};
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);
    for (nIndex = 0; nIndex < pListCtrl->GetItemCount(); ++nIndex)
    {
        if (pListCtrl->GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED)
        {
            return nIndex;
        }
    }

    return -1;
}

int CHideMyFileDlg::GetFileItemID( int nIndex )
{
    int nID;
    TCHAR szBuf[20] = {0};
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);

    if (nIndex != -1)
    {
        pListCtrl->GetItemText(nIndex, 0, szBuf, 20);
        nID = _wtoi(szBuf);

        return nID;
    }

    return 0;
}


void CHideMyFileDlg::OnExploerFile()
{
    //��ȡ�б��������ļ���Ŀ����
    int nIndex = GetSelectItemIndex();
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);

    //ȡ�ļ�·��
    CString strFilePath;
    pListCtrl->GetItemText(nIndex, 2, strFilePath.GetBuffer(MAX_PATH), MAX_PATH);
    strFilePath.ReleaseBuffer();

    CString strCmd;
    if (PathIsDirectory(strFilePath))
    {
        strCmd.Format(L"/c start %s", strFilePath);
        ShellExecute(NULL,L"OPEN",L"cmd.exe",strCmd,NULL,SW_HIDE);
    }
    else
    {
        strCmd.Format(L"/e,/select,%s", strFilePath);
        ShellExecute(NULL,L"OPEN",L"explorer.exe",strCmd,NULL,SW_SHOW);
    }

}

BOOL CHideMyFileDlg::EnableDebugPrv()
{
    HANDLE hToken;
    LUID SeDebugNameValue;
    TOKEN_PRIVILEGES tkp;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))//�򿪽�������
    {
        //cout<<"Open Token Failed!"<<endl;
        return FALSE;
    }
    if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&SeDebugNameValue))//����Ȩ����Ϣ
    {
        CloseHandle(hToken);
        //cout<<"Look up Privilege Failed!"<<endl;
        return FALSE;
    }
    tkp.PrivilegeCount = 1;//Ȩ������
    tkp.Privileges[0].Luid = SeDebugNameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,NULL))//����Ȩ��
    {
        CloseHandle(hToken);
        //cout<<"Adjust Token Privileges Failed!"<<endl;
        return FALSE;
    }

    OutputDebugString(L"Done!\n");
    return TRUE;
}

LRESULT CHideMyFileDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (message == WM_HOTKEY)
    {
        if (wParam == WM_HIDEHOTKEY)
        {
            if (!CheckLogin())
            {
                m_bIsVisable = FALSE;
                ShowWindow(SW_HIDE);
            }
            else
            {
                m_bIsVisable = TRUE;
                SetForegroundWindow();
                ShowWindow(SW_SHOW);
                CenterWindow();
            }
        }
    }

    return CDialogEx::DefWindowProc(message, wParam, lParam);
}

BOOL CHideMyFileDlg::CheckLogin()
{
    if (m_IsShowLoginDlg == FALSE)
    {
        m_IsShowLoginDlg = TRUE;
        CLoginDlg dlg;

        if (dlg.DoModal() ==  IDOK)
        {
            m_IsShowLoginDlg = FALSE;
            return TRUE;
        }
        else
        {
            m_IsShowLoginDlg = FALSE;
        }
    }
    
    return FALSE;
}

void CHideMyFileDlg::CheckSingleton()
{
    HANDLE hObject  = CreateMutex(NULL,FALSE,L"HideMyFileSingleton");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hObject);
        MessageBox(L"�����Ѿ����У�\r\n��ʹ���ȼ� Ctrl + Shift + T ����������", L"��ʾ", MB_OK|MB_ICONINFORMATION);
        ExitProcess(0);
    }
}

BOOL CHideMyFileDlg::CheckExistFileInList(CString strFilePath)
{
    CString strPath;
    CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_FOLDER_LIST);
    int nIdx = 0;
    int nCount = pListCtrl->GetItemCount();

    for (nIdx = 0; nIdx < nCount; ++nIdx)
    {
        pListCtrl->GetItemText(nIdx, 2, strPath.GetBuffer(100), 100);
        strPath.ReleaseBuffer();

        if (strFilePath == strPath) //�ļ��б����Ѿ�����
        {
            return TRUE;
        }
    }

    return FALSE;
}


BOOL CHideMyFileDlg::PreTranslateMessage(MSG* pMsg)
{
   if (pMsg->message == WM_KEYDOWN) //����ESC�ͻس�
   {
       if (pMsg->wParam == VK_ESCAPE ||
           pMsg->wParam == VK_RETURN)
       {
           return TRUE;
       }
   }
    return CDialogEx::PreTranslateMessage(pMsg);
}
