#include "stdafx.h"


#define CFM_SECTION_PREFIX L"FSection"
#define CFM_KEY_ID L"nID"
#define CFM_KEY_FOLDER_NAME L"FileName"
#define CFM_KEY_FOLDER_PATH L"FilePath"
#define CFM_KEY_HIDDEN L"Hidden"

const int MAX_FOLDER_NUM  = 100;

class CFileItem
{
public:
    UINT m_nID;
    CString m_strFileName;
    CString m_strFilePath;
    UINT m_bHidden;

public:
    CFileItem()
    {
        m_nID = 0;
        m_strFileName = L"";
        m_strFilePath = L"";
        m_bHidden = 0;
    }
};


class CFileManager
{
public:
    CFileManager(void);
    ~CFileManager(void);

public:
    void Initialize();

    //���һ���ļ���
    void AddFileItem(LPCTSTR strFilePath);

    //�����ļ��б���ʾ״̬
    void SetFileListHiddenState(UINT nID, BOOL bHidden);

    //ɾ��һ���ļ���Ŀ
    void DeleteFileItem(UINT nIndex);

    //��ȡ�ļ���
    CString GetItemFileName(UINT nIndex);

    //��ȡ�ļ�·��
    CString GetItemFilePath(UINT nIndex);

    //��ȡID
    UINT    GetItemFileId(UINT nIndex);

    //��ȡ�ļ���ʾ״̬
    UINT    GetItemFileState(UINT nIndex);

    //��ȡ�ļ��б���
    UINT GetFileItemCount();

private:
    
    void AddFileItem(CFileItem newItem);

    //��ȡ�ļ��б�
    void ReadFileList();

    //�����ļ��б�
    void SaveFileList();

    //ˢ���ļ��б�����
    void RefreshFileItemIndex();

    //���¼����ļ��б�
    void ReloadFileList();

    //����INI�ڵ�
    BOOL WriteFileListSection(LPCTSTR strSectionName);

    //д��String
    BOOL WriteFileListDataString(LPCTSTR strSectionName,LPCTSTR strKey, LPCTSTR strValue);

    //д����ֵ
    BOOL WriteFileListDataInt(LPCTSTR strSectionName, LPCTSTR strKey, UINT nValue);

    //��ȡString
    CString GetFileListDataString(LPCTSTR strSectionName, LPCTSTR strKey);

    //��ȡ��ֵ
    UINT GetFileListDataInt(LPCTSTR strSectionName, LPCTSTR strKey);

    //���ļ�·����ȡ�ļ���
    CString GetFileName(CString strFilePath);

    //��ȡϵͳĿ¼
    CString GetSystemDir();


private:
    CArray<CFileItem, CFileItem> m_FileArray;
    CString m_strFileListFilePath;
};

