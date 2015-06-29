// ILHook1.h: interface for the CILHook class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>

#if !defined(AFX_ILHOOK1_H__4762079F_D5A0_4E05_B5F3_896E8CF5B58A__INCLUDED_)
#define AFX_ILHOOK1_H__4762079F_D5A0_4E05_B5F3_896E8CF5B58A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CILHook  
{
public:
	CILHook();
	virtual ~CILHook();
	BOOL Hook(LPSTR pszMoudleName, LPSTR pszFuncName, PROC pfnMyFuncNAme);
	void UnHook();
	BOOL ReHook();

private:
	BYTE m_bNewByte[5];	//�޸ĵĺ�����ַǰ����ֽ�
	BYTE m_bOldByte[5];	//�ɵĺ�����ַǰ����ֽ�
	PROC m_pfnOrig;	//Ҫhook�ĺ�����ַ
};

#endif // !defined(AFX_ILHOOK1_H__4762079F_D5A0_4E05_B5F3_896E8CF5B58A__INCLUDED_)
