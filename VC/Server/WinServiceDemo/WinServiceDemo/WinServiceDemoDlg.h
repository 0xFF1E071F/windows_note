// WinServiceDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "WindowsService.h"
#define WM_SHOWTASK  WM_USER +100
// CWinServiceDemoDlg �Ի���
class CWinServiceDemoDlg : public CDialog
{
// ����
public:
	CWinServiceDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINSERVICEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CWindowsService m_Service;
	NOTIFYICONDATA m_TipIcon;
	char m_szTipShowInfo[200];
	BYTE m_bMsbox;
public:
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ShowIconToTip(BOOL bShow = TRUE);
	void ShowBalloonTip(const char* pMsg, const char*pTitle ="���񱣻�����", UINT uTimeout=20, DWORD dwInfoFlags=1);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
