// MulAppExeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMulAppExeDlg �Ի���
class CMulAppExeDlg : public CDialog
{
// ����
public:
	CMulAppExeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULAPPEXE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void RunAppWithHook(LPTSTR tzFilePathName, LPTSTR tzDllPath);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	CRichEditCtrl m_RichEditMessage;
	afx_msg void OnBnClickedButtonFile();
	CEdit m_EditFile;
	afx_msg void OnBnClickedButtonExecute();
};
