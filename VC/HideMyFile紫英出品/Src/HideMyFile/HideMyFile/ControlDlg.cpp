// ControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HideMyFile.h"
#include "ControlDlg.h"
#include "afxdialogex.h"

extern CConfigManager g_ConfigManager;
extern CDriverManager g_DriverManager;

// CControlDlg �Ի���

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlDlg::IDD, pParent)
{
    m_bOptionLoaded = FALSE;
}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CCM_CLOSE, m_ConfigClose);
    DDX_Control(pDX, IDC_BTN_SAVE, m_BtnSavePwd);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
    ON_WM_NCHITTEST()
    ON_WM_CTLCOLOR()
    ON_STN_CLICKED(IDC_CCM_CLOSE, &CControlDlg::OnStnClickedConfigClose)
    ON_STN_CLICKED(IDC_CHK_AUTOSTART, &CControlDlg::OnStnClickedChkAutostart)
    ON_STN_CLICKED(IDC_CHK_SHOWTRAY, &CControlDlg::OnStnClickedChkShowtray)
    ON_STN_CLICKED(IDC_OPT_TOTRAY, &CControlDlg::OnStnClickedOptTotray)
    ON_STN_CLICKED(IDC_OPT_CLOSE, &CControlDlg::OnStnClickedOptClose)
    ON_WM_PAINT()
    ON_STN_CLICKED(IDC_BTN_SAVE, &CControlDlg::OnStnClickedBtnSave)
    ON_STN_CLICKED(IDC_CHK_HIDEPROCESS, &CControlDlg::OnStnClickedChkHideprocess)
END_MESSAGE_MAP()


// CControlDlg ��Ϣ�������


void CControlDlg::PostNcDestroy()
{
    // TODO: �ڴ����ר�ô����/����û���

    CDialogEx::PostNcDestroy();

    delete this;
}


void CControlDlg::OnCancel()
{
    // TODO: �ڴ����ר�ô����/����û���
    DestroyWindow();
    //CDialogEx::OnCancel();
}


LRESULT CControlDlg::OnNcHitTest(CPoint point)
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


HBRUSH CControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CControlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

    m_ConfigClose.SetButtonImages(IDB_CLOSE_NORMAL, IDB_CLOSE_ACTIVE);
    m_BtnSavePwd.SetButtonImages(IDB_SAVE_NORMAL, IDB_SAVE_ACTIVE);


    CBitmap bitmapBackgroud;
    bitmapBackgroud.LoadBitmapW(IDB_CONTROL_BG);
    m_BkBrush.CreatePatternBrush(&bitmapBackgroud);
    bitmapBackgroud.DeleteObject();
    
    CenterWindow();

    m_PwdFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, L"Microsoft YaHei");

    CEdit *pPwdEdit = (CEdit*)(GetDlgItem(IDC_EDIT_NEW_PWD));
    pPwdEdit->SetFont(&m_PwdFont);

    //SetDlgOptions();
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CControlDlg::SetCheckState( UINT uID, BOOL bState )
{
    CStatic *pCheckStatic = (CStatic *)(GetDlgItem(uID));
    if (bState)
    {

        CBitmap BitmapActive;
        BitmapActive.LoadBitmap(IDB_CHK_SEL);
        pCheckStatic->SetBitmap((HBITMAP)BitmapActive);
        BitmapActive.DeleteObject();
        UpdateWindow();
        //Invalidate(TRUE);
    }
    else
    {
        CBitmap BitmapNormal;
        BitmapNormal.LoadBitmap(IDB_CHK_UNSEL);
        pCheckStatic->SetBitmap((HBITMAP)BitmapNormal);
        BitmapNormal.DeleteObject();
        UpdateWindow();
        //Invalidate(TRUE);
    }
}

void CControlDlg::SetOptionState( UINT uID1, UINT uID2)
{
    CStatic *pOptionStatic1 = (CStatic *)(GetDlgItem(uID1));
    CStatic *pOptionStatic2 = (CStatic *)(GetDlgItem(uID2));

    CBitmap BitmapSel;
    CBitmap BitmapUnSel;

    BitmapSel.LoadBitmap(IDB_OPT_SEL);
    BitmapUnSel.LoadBitmap(IDB_OPT_UNSEL);

    pOptionStatic1->SetBitmap((HBITMAP)BitmapSel);
    pOptionStatic2->SetBitmap((HBITMAP)BitmapUnSel);

    BitmapSel.DeleteObject();
    BitmapUnSel.DeleteObject();

}


void CControlDlg::OnStnClickedConfigClose()
{
    OnOK();
}

void CControlDlg::OnStnClickedChkAutostart()
{
    //ȡ��
    int nValue = !(g_ConfigManager.GetConfigValueInt(CCM_TYPE_AUTOSTART));

    //д�������ļ�
    g_ConfigManager.SetConfigValueInt(CCM_TYPE_AUTOSTART, nValue);

    //�ı�ѡ��״̬
    SetCheckState(IDC_CHK_AUTOSTART, (BOOL)nValue);

    SetAutoStart((BOOL)nValue);
}

void CControlDlg::OnStnClickedChkHideprocess()
{
    //ȡ��
    int nValue = !(g_ConfigManager.GetConfigValueInt(CCM_TYPE_HIDEPROCESS));

    //д�������ļ�
    g_ConfigManager.SetConfigValueInt(CCM_TYPE_HIDEPROCESS, nValue);

    //�ı�ѡ��״̬
    SetCheckState(IDC_CHK_HIDEPROCESS, (BOOL)nValue);

    //ȡ��ǰ������
    CString strCurProcessName = g_DriverManager.GetCurrentProcessName();

    g_DriverManager.SetProcessHiddenState(strCurProcessName, (BOOL)nValue);
}

void CControlDlg::OnStnClickedChkShowtray()
{
    //ȡ��
    int nValue = !(g_ConfigManager.GetConfigValueInt(CCM_TYPE_SHOWTRAY));

    //д�������ļ�
    g_ConfigManager.SetConfigValueInt(CCM_TYPE_SHOWTRAY, nValue);

    //�ı�ѡ��״̬
    SetCheckState(IDC_CHK_SHOWTRAY, (BOOL)nValue);
}


void CControlDlg::OnStnClickedOptTotray()
{
    //ȡ��
    int nValue = !(g_ConfigManager.GetConfigValueInt(CCM_TYPE_CLOSESTATE));

    //д�������ļ�
    g_ConfigManager.SetConfigValueInt(CCM_TYPE_CLOSESTATE, nValue);


    SetOptionState(IDC_OPT_TOTRAY, IDC_OPT_CLOSE);
}

void CControlDlg::OnStnClickedOptClose()
{
    //ȡ��
    //ȡ��
    int nValue = !(g_ConfigManager.GetConfigValueInt(CCM_TYPE_CLOSESTATE));

    //д�������ļ�
    g_ConfigManager.SetConfigValueInt(CCM_TYPE_CLOSESTATE, nValue);

    SetOptionState(IDC_OPT_CLOSE, IDC_OPT_TOTRAY);
}

void CControlDlg::SetDlgOptions()
{
    int bAutoStart = g_ConfigManager.GetConfigValueInt(CCM_TYPE_AUTOSTART);
    int bShowTray = g_ConfigManager.GetConfigValueInt(CCM_TYPE_SHOWTRAY);
    int bCloseState = g_ConfigManager.GetConfigValueInt(CCM_TYPE_CLOSESTATE);
    int bHideProcess = g_ConfigManager.GetConfigValueInt(CCM_TYPE_HIDEPROCESS);
//     wsprintf(szBuf, L"Auto = %d, ShowTray = %d, bHideProcess = %d, bCloseState = %d \r\n", bAutoStart, bHideProcess, bShowTray, bCloseState);
// 
//     OutputDebugString(szBuf);

    SetCheckState(IDC_CHK_AUTOSTART, (BOOL)bAutoStart);
    SetCheckState(IDC_CHK_SHOWTRAY, (BOOL)bShowTray);
    SetCheckState(IDC_CHK_HIDEPROCESS, (BOOL)bHideProcess);

    //��������ؽ���ѡ����ý���Ϊ����
    if (bHideProcess)
    {
        CString strCurProcessName = g_DriverManager.GetCurrentProcessName();
        g_DriverManager.SetProcessHiddenState(strCurProcessName, TRUE);
    }

    if (bCloseState == 0)
    {
       SetOptionState(IDC_OPT_TOTRAY, IDC_OPT_CLOSE);
    }
    else if (bCloseState == 1)
    {
        SetOptionState(IDC_OPT_CLOSE, IDC_OPT_TOTRAY);
    }

}


void CControlDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    
    //�ػ洰��
    SetDlgOptions();
}


void CControlDlg::OnStnClickedBtnSave()
{
    CString strNewPassword;
    CString strEncodePassword;
    CString strPrompot;
    GetDlgItemText(IDC_EDIT_NEW_PWD, strNewPassword.GetBuffer(20), 20);
    strNewPassword.ReleaseBuffer();

    if (strNewPassword == "" || strNewPassword.GetLength() == 0)
    {
        MessageBox(L"���벻��Ϊ��Ӵ~��^_^", L"����", MB_OK|MB_ICONEXCLAMATION);
        return;
    }
    else
    {
        strPrompot.Format(L"�µ��������룺 %s \r\nȷ��Ҫ����Ϊ�µĹ���������", strNewPassword);
        if (MessageBox(strPrompot, L"��ʾ", MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            strEncodePassword = g_ConfigManager.XORPassword(strNewPassword);

            g_ConfigManager.SetConfigValueString(CCM_TYPE_CONFIGPWD, strEncodePassword);

            SetDlgItemText(IDC_EDIT_NEW_PWD, L"");

            MessageBox(L"�µĹ��������޸ĳɹ���", L"�޸ĳɹ�", MB_OK|MB_ICONINFORMATION);
        }
    }
}

void CControlDlg::SetAutoStart( BOOL bFlag )
{
    OSVERSIONINFO OsVerInfo;
    OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&OsVerInfo);

    if (OsVerInfo.dwMajorVersion < 6) //XP System
    {
        SetXPAutoStart(bFlag); //XP����ע�������
    }
    else
    {
        SetWin7AutoStart(bFlag);//Win7���Ǽƻ���������
    }
}

void CControlDlg::SetWin7AutoStart( BOOL bFlag )
{
    TCHAR szCurrentPath[MAX_PATH];
    GetModuleFileName(NULL,szCurrentPath,MAX_PATH);

    TCHAR szCreateCmd[400] ={0};
    TCHAR szDeleteCmd[40] = L"/c SCHTASKS /DELETE /TN HideMyFile /F"; //ɾ���ƻ�����

    if (bFlag)
    {
        //��Ӽƻ�����
        wsprintf(szCreateCmd,L"/c SCHTASKS /CREATE /TN HideMyFile /TR \"%s /autorun\" /SC ONLOGON /RU \"BUILTIN\\Administrators\" /RL HIGHEST",szCurrentPath);
        ShellExecute(NULL,L"OPEN",L"cmd.exe",szCreateCmd,NULL,SW_HIDE);
    }
    else
    {
        ShellExecute(NULL,L"OPEN",L"cmd.exe",szDeleteCmd,NULL,SW_HIDE);
    }
}

void CControlDlg::SetXPAutoStart( BOOL bFlag )
{
    TCHAR szCurrrentPath[MAX_PATH+1];
    GetModuleFileName(NULL,szCurrrentPath,MAX_PATH);
    wcscat_s(szCurrrentPath,L" /autorun");

    HKEY  hKey;
    DWORD cbData = MAX_PATH;

    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_WRITE,&hKey))
    {
        MessageBox(L"��ע���ʧ�ܣ�",L"��ʾ",MB_OK|MB_ICONERROR);
        RegCloseKey(hKey);
        return;
    }

    if (bFlag)
    {
        if (ERROR_SUCCESS !=RegSetValueEx(hKey,L"HideMyFile",0,REG_SZ,(CONST BYTE*)szCurrrentPath, cbData))
        {
            MessageBox(L"���ÿ�������ʧ�ܣ�",L"��ʾ",MB_OK|MB_ICONERROR);
            RegCloseKey(hKey);
            return;
        }
    }
    else//ȡ����������
    {
        if (ERROR_SUCCESS !=RegDeleteValue(hKey,L"HideMyFile"))
        {
            MessageBox(L"ȡ����������ʧ�ܣ�",L"��ʾ",MB_OK|MB_ICONERROR);
            RegCloseKey(hKey);
            return;
        }
    }
    RegCloseKey(hKey);
}






BOOL CControlDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN)
        {
            if (GetFocus() == GetDlgItem(IDC_EDIT_NEW_PWD))
            {
                OnStnClickedBtnSave();
                return TRUE;
            }
        }
        else if (pMsg->wParam == VK_ESCAPE)
        {
            return TRUE;
        }

    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

