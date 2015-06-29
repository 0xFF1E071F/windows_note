#pragma once
#include <Windows.h>
#include <stddef.h>
#include <assert.h>

struct _InjectData 
{
	char cLoad[13];                      //����LoadLibraryA�ַ���
	char cGetAddr[15];                   //����GetProcAddress�ַ���
	char cKernel32[13];                  //����kernel32.dll�ַ���
	wchar_t wcKernel32[13];              //����kernel32.dll���ֽ��ַ���
	DWORD dwImageBase;
};

class CInjectDllWithOutModule
{
public:
	CInjectDllWithOutModule(const char * pName = NULL);
	~CInjectDllWithOutModule(void);
	//����PE�������ļ��ط�ʽ���м��ر�ע���DLL���ڴ档
	bool LoadDll(const char * pName = NULL, BOOL bReadOnly = FALSE);
	//�ж��Ƿ���DLL
	BOOL IsDll(const char * pName);
	BOOL InjectDllAndRun(DWORD dwProcessID, const char * pName);
private:
	//�����ض�λ����
	void RelocateImage(DWORD dwRelocOffset);
	DWORD AlignmentNum(DWORD dwOperateNum, DWORD dwAlignment);
	//��õ���������RVA
	DWORD GetExportFuncAddr(const char *pFuncName);
	LPVOID m_pMem;   //���汾�ؼ���DLL����ڴ��ַ
	char m_cName[MAX_PATH]; //���汻���ص�DLL������·��
	DWORD m_dwImageSize;    //����DLL�����غ���ռ�ڴ��ܴ�С
	BOOL m_bReadOnly;       //�Ƿ���ֻ���ķ�ʽ����
};
