// ILHook1.cpp: implementation of the CILHook class.
//
//////////////////////////////////////////////////////////////////////

#include "ILHook.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CILHook::CILHook()
{
	//��ʼ��
	m_pfnOrig = NULL;
	ZeroMemory(m_bOldByte, 5);
	ZeroMemory(m_bNewByte, 5);
}

CILHook::~CILHook()
{
	UnHook();
}

BOOL CILHook::Hook(LPSTR pszMoudleName, LPSTR pszFuncName, PROC pfnMyFuncNAme)
{
	BOOL bRet = FALSE;

	//��ȡ������ַ
	m_pfnOrig = (PROC)GetProcAddress(GetModuleHandle(pszMoudleName), pszFuncName);
	if (m_pfnOrig == NULL)
	{
		MessageBox(NULL, "��ȡ��ַʧ��", "Error", 0);
		return FALSE;
	}
	
	//��ȡ������ǰ����ֽ�
	DWORD dwNum = 0;
	ReadProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bOldByte, 5, &dwNum);

	//����jmp XXXXXXXX
	m_bNewByte[0] = '\xE9';
	*(DWORD*)(m_bNewByte + 1) = (DWORD)pfnMyFuncNAme - (DWORD)m_pfnOrig - 5;

	//д�뵽����ǰ����ֽ�
	WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bNewByte, 5, &dwNum);
	
	bRet = TRUE;
	return bRet;
}

void CILHook::UnHook()
{
	DWORD dwNum = 0;
	WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bOldByte, 5, &dwNum);
}

BOOL CILHook::ReHook()
{
	BOOL bRet = FALSE;
	DWORD dwNum = 0;
	
	WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bNewByte, 5, &dwNum);
	bRet = TRUE;
	return bRet;
}