// ����ע��Dlg.h : header file
//
#include <windows.h>
#if !defined(AFX_DLG_H__E3C4CFCE_2C0B_40D0_8CD1_DA0297054D1E__INCLUDED_)
#define AFX_DLG_H__E3C4CFCE_2C0B_40D0_8CD1_DA0297054D1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog


struct RemoteStruct //Զ�̽ṹ��
{
	char MyText[255];		//����
	char MyCaption[255];	//����
	PROC Address;			//����ض�λ�Ժ��MessageBox��ַ
	DWORD PID;
};
typedef struct tag_zhuru
{
	DWORD PID;
	char szName[255];
}zhuru;

typedef int (WINAPI*MyMessageBox)(HWND,LPCTSTR,LPCTSTR,UINT); //����ԭ��

class CMyDlg : public CDialog
{
// Construction
public:
	CRect m_rect;
	void UpdateMain();
	CMyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
	CListCtrl	m_list;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void Onzhuru();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL PromoteProcessPrivileges();//������ǰ����Ȩ��
	static void WINAPI MyThread(void *MyArg); //Զ�̽���

	zhuru zhu[100];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__E3C4CFCE_2C0B_40D0_8CD1_DA0297054D1E__INCLUDED_)
