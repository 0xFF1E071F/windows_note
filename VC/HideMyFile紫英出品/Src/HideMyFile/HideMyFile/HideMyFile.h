
// HideMyFile.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHideMyFileApp:
// �йش����ʵ�֣������ HideMyFile.cpp
//

class CHideMyFileApp : public CWinApp
{
public:
	CHideMyFileApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHideMyFileApp theApp;