#include "stdafx.h"

XLIB::CXHookAPI m_HookMessageBox;

//�µĺ���
int WINAPI myMessageBox(
  HWND hWnd, 
  LPCSTR lpText, 
  LPCSTR lpCaption, 
  UINT uType
)
{
	//����Դ����
	return m_HookMessageBox.CallFunction( 4,0,"hook��",lpCaption,uType );
}

//���Ǻ���
VOID __cdecl filter( XLIB::CStack_ESP* pStack )
{
	printf("0x%08x\n",pStack->GetReturnAddress());
	printf(" argu2 = %s\n",pStack->GetArgument(1));
}

int _tmain(int argc, _TCHAR* argv[])
{


	LPVOID lpAddr = XLIB::CXHookAPI::GetAddressByNameA( "user32.dll","MessageBoxA");
	
	//inline hook����
	MessageBoxA( 0,"hookǰ",0,0);
	//m_HookMessageBox.InlineHookFunction( lpAddr,myMessageBox );

	//�������������hook���������������
	m_HookMessageBox.HookImportTable( GetModuleHandle(0),"user32.dll","MessageBoxA",myMessageBox);

	//���˲���
	//m_HookMessageBox.InlineHookAddress( lpAddr,filter);

	MessageBoxA( 0,"hookǰ",0,0);

	system("pause");
	return 0;
}

