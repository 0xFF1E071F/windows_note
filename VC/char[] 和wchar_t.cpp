WideCharToMultiByte:Unicodeתchar *(���ֽ�ת���ֽ�) 
MSDN:http://msdn.microsoft.com/en-us/library/dd374130(VS.85).aspx 
C++����  �ղش���
#include "stdafx.h"  
#include "stdio.h"  
#include "windows.h"  
  
int _tmain(int argc, _TCHAR* argv[])  
{  
    TCHAR strWideChar[] = _T("zerosoul");   //Unicode���ֽڱ����ַ���strWideChar  
    //���ת������ַ�������n_len  
    int n_len = WideCharToMultiByte(CP_ACP,NULL,strWideChar,-1,NULL,0,NULL,NULL);  
    char* strMultiByte = new char[n_len];   //ANSI���ֽڱ����ַ���strMultiByte  
    WideCharToMultiByte(CP_ACP,NULL,strWideChar,-1,strMultiByte,n_len,NULL,NULL);  //��ʼת��  
  
    printf("strMultiByte : %s\n",strMultiByte);  
    return 0;  
}  


MultiByteToWideChar:char *תUnicode(���ֽ�ת���ֽ�) 
MSDN:http://msdn.microsoft.com/en-us/library/dd319072(VS.85).aspx 
C++����  �ղش���
#include "stdafx.h"  
#include "stdio.h"  
#include "windows.h"  
  
int _tmain(int argc, _TCHAR* argv[])  
{  
    char strMultiByte[] = "zerosoul";   //ANSI���ֽڱ����ַ���strMultiByte  
    //���ת������ַ�������n_len  
    int n_len = MultiByteToWideChar(CP_ACP,NULL,strMultiByte,-1,NULL,0);  
    TCHAR* strWideChar = new TCHAR[n_len];  //Unicode���ֽڱ����ַ���strWideChar  
    MultiByteToWideChar(CP_ACP,NULL,strMultiByte,-1,strWideChar,n_len); //��ʼת��  
  
    _tprintf(_T("strWideChar : %s\n"),strWideChar);  
    return 0;  
}  
////////////////////////////////////////////////////////////////////////////
	DWORD dwNum;

	setlocale(LC_ALL,"");

	char sText[] = "���ֽ��ַ���ת���ַ���";
	dwNum = MultiByteToWideChar(CP_ACP,0,sText,-1,NULL,0);

	wchar_t *pwText;
	pwText = new wchar_t[dwNum];
	if(!pwText)
	{
		delete[] pwText;
		return 1;
	}
	MultiByteToWideChar(CP_ACP,0,sText,-1,pwText,dwNum);
	wprintf(L"M->W: %s\n", pwText);
	MessageBoxW(NULL,pwText,L"Test",MB_OK);
	delete[] pwText;
////////////////////////////////////////////////////////////////////////////
	wchar_t wTest[] = L"���ַ���ת���ֽ��ַ���";
	dwNum = WideCharToMultiByte(CP_OEMCP,0,wTest,-1,NULL,0,NULL,NULL);

	char *psText;
	psText = new char[dwNum];
	if(!psText)
	{
		delete[] psText;
		return 2;
	}
	WideCharToMultiByte(CP_OEMCP,0,wTest,-1,psText,dwNum,NULL,NULL);
	printf("W->M: %s\n",psText);
	MessageBoxA(NULL,psText,"Test",MB_OK);
	delete[] psText;

	system("pause");
	return 0;