// WinServiceDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWinServiceDemoApp:
// �йش����ʵ�֣������ WinServiceDemo.cpp
//

class CWinServiceDemoApp : public CWinApp
{
public:
	CWinServiceDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWinServiceDemoApp theApp;