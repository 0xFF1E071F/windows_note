// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HideMyFile.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "ConfigManager.h"

extern CConfigManager g_ConfigManager;

// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LOGIN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_LOGIN_OK, m_BtnOK);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
    ON_WM_NCHITTEST()
    ON_STN_CLICKED(IDC_LOGIN_CLOSE, &CLoginDlg::OnStnClickedLoginClose)
    ON_STN_CLICKED(IDC_LOGIN_OK, &CLoginDlg::OnStnClickedLoginOk)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������


BOOL CLoginDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //�Ի��򱳾�
    CBitmap bitmapBackgroud;
    bitmapBackgroud.LoadBitmapW(IDB_LOGIN_BG);
    m_BkBrush.CreatePatternBrush(&bitmapBackgroud);
    bitmapBackgroud.DeleteObject();

    //·��������
    m_EditFont.CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, L"Microsoft YaHei");

    CEdit *pEdit = (CEdit*)(GetDlgItem(IDC_LOGIN_PWD));
    pEdit->SetFont(&m_EditFont);
    pEdit->SetFocus();

    m_BtnClose.SetButtonImages(IDB_CLOSE_NORMAL, IDB_CLOSE_ACTIVE);
    m_BtnOK.SetButtonImages(IDB_LOGIN_OK_NORMAL, IDB_LOGIN_OK_ACTIVE);

    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (pWnd == this)
    {
        return m_BkBrush;
    }

    if (CTLCOLOR_STATIC == nCtlColor)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)(GetStockObject(NULL_BRUSH));
    }

    return hbr;
}


void CLoginDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
    UINT nHitTest = CDialog::OnNcHitTest(point);

    if (nHitTest == HTCLIENT)
    {
        return HTCAPTION;
    }

    return nHitTest;
   
    //return CDialogEx::OnNcHitTest(point);
}


void CLoginDlg::OnStnClickedLoginClose()
{
    OnCancel();
}


void CLoginDlg::OnStnClickedLoginOk()
{
    CString strPwd;
    strPwd = g_ConfigManager.GetConfigValueString(CCM_TYPE_CONFIGPWD);
    strPwd = g_ConfigManager.XORPassword(strPwd);


    CString strInputPwd;
    GetDlgItemText(IDC_LOGIN_PWD, strInputPwd.GetBuffer(100), 100);
    strInputPwd.ReleaseBuffer();

    if (strInputPwd == strPwd)
    {
        OnOK();
    }
    else
    {
        MessageBox(L"���벻��Ŷ~����ϸ����Ӵ~��^_^", L"����", MB_OK|MB_ICONERROR);
    }
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���

    if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
        {
            if (GetFocus() == GetDlgItem(IDC_LOGIN_PWD))
            {
                OnStnClickedLoginOk();
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
