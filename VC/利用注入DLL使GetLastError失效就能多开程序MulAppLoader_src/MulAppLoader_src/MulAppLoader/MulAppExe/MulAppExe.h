// MulAppExe.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMulAppExeApp:
// �йش����ʵ�֣������ MulAppExe.cpp
//

class CMulAppExeApp : public CWinApp
{
public:
	CMulAppExeApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMulAppExeApp theApp;