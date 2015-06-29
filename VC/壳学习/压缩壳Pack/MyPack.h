// MyPack.h: interface for the CMyPack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACK_H__6F27A006_7B3E_48E6_ACF1_10CA1C050210__INCLUDED_)
#define AFX_MYPACK_H__6F27A006_7B3E_48E6_ACF1_10CA1C050210__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Windows.h>
#include <string>
#include <tchar.h>
#include "Include/aplib.h"
#include <fstream>
#pragma comment(lib, "lib/aplib.lib")
using namespace std;
/*
*����ѹ�����PE�ڵĽṹ
*VA:ѹ��ǰ(���ѹ��)�ڴ��ַ
*CompessVA:ѹ�����ڴ��ַ
*lpCompessData:ѹ������ָ��
*CompessSize:ѹ�����ݴ�С
*/
typedef struct _CompessSection
{
	DWORD VA;             //ѹ��ǰ(���ѹ��)�ڴ��ַ
	DWORD CompessVA;      //ѹ�����ݼ��غ���ڴ��ַ
	DWORD CompessSize;    //ѹ�������ݴ�С
	LPVOID lpCompessData; //ѹ������ָ��
}CompessSection, *PCompessSection;
//������б��溯����Ϣ�Ľṹ��
typedef struct _FUNC_INFO
{
	TCHAR cFunctionName[256]; //���뺯����
	long lOrdinal;            //���뺯�����
	DWORD *pdwRva;            //���뺯����ַ
	_FUNC_INFO()
	{
		lOrdinal = 0;
		pdwRva = NULL;
		memset(cFunctionName, '\0', 256);
	}
}*pFuncInfo, FUNC_INFO;
//�ṹ���б���DLL��Ϣ�Ľṹ
typedef struct _DLL_INFO
{
	TCHAR cDllName[256];     //�������
	pFuncInfo pFunctionInfo; //�ӱ�DLL�е���ĺ�����Ϣ
	long lFuncNum;           //�ӱ�DLL�е���ĺ�������
	_DLL_INFO()
	{
		pFunctionInfo = NULL;
		lFuncNum = 0;
		memset(cDllName, '\0', 256);
	}
}*pDllInfo, DLL_INFO;
//������б��������������Ϣ�Ľṹ
typedef struct _IMPORT_INFO
{
	pDllInfo pDll_info;    //������е�������п����Ϣ
	long lDllNum;          //������е���Ŀ�ĸ���
}*pImportInfo, IMPORT_INFO;

enum ERRORINFO{SUCCESS, FILE_NOT_EXIST, NOT_PE, LOAD_TO_MEM_ERROR, DEAL_SPILTH_ERROR, 
PARAMETER_ERROR, CREATE_TARGET_FILE_ERROR, ALREADY_PACKED, NOT_LOAD};

//�����������£�
class CPack  
{
public:
	CPack(BOOL bCompressResource = TRUE);
	~CPack();
public:
	//��ȡ���ӿǵĳ����ļ�����PE���������ط�ʽ�����ڴ�
	ERRORINFO LoadPE(TCHAR *pFileName);
	//���ɼӿ��ļ�
	ERRORINFO GetFile(TCHAR *pPathName);

protected:
	//�ж��ļ��Ƿ����
	BOOL IsFileExist(char * strFileName = NULL);
	//�жϱ��ӿǳ����Ƿ�ΪPE�ļ���
	BOOL IsPeFile(char *pFileName = NULL);
	//����PE���صķ�ʽ(�ڴ�������ȶ��뷽ʽ)���ļ���ȡ���ڴ�
	BOOL ReadFileToMem(TCHAR *pFileName = NULL);
	//���渽������
	BOOL DoSpilthData(TCHAR *pFileName = NULL);
	//����ض�λ��Ϣ�������ض�λ��RVA
	void ClearReloc();
	//�ж��ļ��Ƿ�ΪDLL���˿ǲ�֧��DLL�ļ��ӿǣ������DLL�򷵻ؼ١�
	//ѹ������, 
	//ע��ѹ���ǲ���aplibѹ���㷨,ʹ�õ�ʱ��Ӧ�ð�����ͷ�ļ�,�������侲̬��,����: 
	//#include "aplib.h"
	//#pragma comment(lib, "aplib.lib")
	//ѹ�����ݺ���,���ر�ѹ�������������ڴ��ָ��
	PVOID CompressData(PVOID pSource, long lInLength, OUT long &lOutLenght);
	//ѹ�����н�
	void CompressSections();
	BOOL IsDll();
	//���Ŀ¼����Ϣ����,����Ŀ¼���С,����dwDataDirectoryOffset����Ŀ¼���RVA
	DWORD GetDataDirectoryInfo(DWORD dwDataDirectory, DWORD &dwDataDirectoryOffset);
	//��dwAlignment ����dwOperateNum ֵ,Ҳ������dwOperateNumΪdwAlignment��������
	DWORD AlignmentNum(DWORD dwOperateNum, DWORD dwAlignment);
	//���쵼���
	void BuildImportTable(DWORD dwRva, pImportInfo pImport, PBYTE pMem);
	//����ָ��Ҫ����ĵ����Ĵ�С
	DWORD ImportTableSize(pImportInfo pImport);
	//���ָ��RVA�����ڵ�ָ��
	IMAGE_SECTION_HEADER *GetSectionPtrByRva(DWORD dwRva);
	//��ȡָ��Ŀ¼���ڽڵ�����
	int GetSectionPos(DWORD dwDataDirectory);
	//����ָ��Ŀ¼���Ŀ¼��������Ϣ
	IMAGE_SECTION_HEADER GetAppointSection(DWORD dwDataDirectory);
private:
	TCHAR *m_pMemPointer ;  //PE�ļ������ڴ��ĵ�ַ

	LPVOID m_pSplithData; //ָ�򸽼����������ڴ��ָ��
	DWORD m_dwSplitDataSize; //�������ݵĴ�С
	TCHAR m_cFileName[256]; //PE�ļ�������·����
	IMAGE_DOS_HEADER *m_pPeDosHeader; //ָ��Dosͷ��ָ��
	IMAGE_NT_HEADERS *m_pPeNtHeader; //ָ��PEͷ��ָ��	
	IMAGE_SECTION_HEADER *m_pPeSectionHeader;	//ָ��ڱ�ͷ��ָ��
	int m_iAllSecMemSize; //���нڼ��غ��ڴ��ռ�Ĵ�С
	unsigned int m_iSecSize; //PE�ṹ�ڱ���ܴ�С
	unsigned int m_iDosSize; //PE�ṹDosͷ��Dosʵ��Ĵ�С
	unsigned int m_iNtSize; //PE�ṹNtͷ�Ĵ�С
	unsigned int m_iPeSize; //PE�ṹPEͷ���ܴ�С

	int m_iMemAlignment;	//PE�ṹ�ڴ��������
	int m_iFileAlignment;	//PE�ṹ�ļ���������	
	unsigned int m_iSecNum; //PE�ṹ�ڵĸ���
	bool m_bLoadSuccess;    //�Ƿ�ɹ�����
	IMAGE_SECTION_HEADER m_Resource; //������Դ����Ϣ
	BOOL m_bCompressResource;  //�Ƿ�ѹ����Դ��
	CompessSection *m_pComSec; //ѹ����ָ��
	int m_iComSecNum;          //ѹ���ڸ���
	DWORD m_dwRelocationTableRva; //�ض�λ���RVA
};


#endif // !defined(AFX_MYPACK_H__6F27A006_7B3E_48E6_ACF1_10CA1C050210__INCLUDED_)
