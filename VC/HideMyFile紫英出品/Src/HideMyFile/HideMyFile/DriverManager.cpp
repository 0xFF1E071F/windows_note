#include "StdAfx.h"
#include "DriverManager.h"
#include "ServiceControl.h"
#include "resource.h"

CDriverManager::CDriverManager(void)
{

}


CDriverManager::~CDriverManager(void)
{
}

BOOL CDriverManager::DriverIoControl( DWORD dwControlCode, LPCTSTR InputBuffer, DWORD dwInputSize, LPDWORD dwRet )
{
    //char szInputBuffer[MAX_PATH];

    BOOL bRet = FALSE;
    HANDLE hDevice = CreateFile(L"\\\\.\\HideSys", GENERIC_READ|GENERIC_WRITE, 0,
                                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hDevice)
    {
        MessageBox(NULL, L"Failed To Open Device!", NULL, MB_OK);
        return FALSE;
    }

    //WCharToMByte(InputBuffer, szInputBuffer, MAX_PATH);

    bRet = DeviceIoControl(hDevice, dwControlCode, (LPVOID)InputBuffer, dwInputSize, NULL, NULL, dwRet, NULL);

    CloseHandle(hDevice);

    return bRet;
}

BOOL CDriverManager::StartDriverService()
{
    BOOL bRet;
    bRet = StartServiceForUser(CDM_SERVICE_NAME);

    return bRet;
}

BOOL CDriverManager::StopDriverService()
{
    BOOL bRet;
    bRet = StopServiceForUser(CDM_SERVICE_NAME);

    return bRet;
}

BOOL CDriverManager::AddNewHiddenFile( LPCTSTR strFileName )
{
    DWORD dwSize= (wcslen(strFileName) + 1) * sizeof(WCHAR);
    DWORD dwRet;
    BOOL bRet;

    bRet = DriverIoControl(IOCTL_ADD_HIDDEN_FILE, strFileName, dwSize, &dwRet);

    return bRet;
}

BOOL CDriverManager::SetFileDriverHiddenState( LPCTSTR strFileName, BOOL bState )
{
    DWORD dwSize= (wcslen(strFileName) + 1) * sizeof(WCHAR);
    DWORD dwRet;
    BOOL    bRet;

    if (bState)
    {
        bRet = DriverIoControl(IOCTL_SET_FILE_HIDDEN, strFileName, dwSize, &dwRet);
    }
    else
    {
        bRet = DriverIoControl(IOCTL_SET_FILE_SHOWN, strFileName, dwSize, &dwRet);
    }
    return bRet;
}

BOOL CDriverManager::InstallDriver()
{
    BOOL bRet ;
    bRet = InstallServiceToSCM(m_strDriverPath, CDM_SERVICE_NAME, 
                                          CDM_DISPLAY_NAME, CDM_SERVICE_START_TYPE);

    return bRet;
}

BOOL CDriverManager::UninstallDriver()
{
    BOOL bRet;
    bRet = UninstallServiceFromSCM(CDM_SERVICE_NAME);

    return bRet;
}

BOOL CDriverManager::ExtractSysFile()
{
    HMODULE hInstance = ::GetModuleHandle(NULL);//�õ�����ʵ�����

    HRSRC hResID;
    SystemVer SysVer = CheckWindowsVersion();
    if ( SysVer == VER_WINDOWS_XP)
    {
        hResID = ::FindResource(hInstance,MAKEINTRESOURCE(IDR_KERNEL_XP_SYS),L"KERNEL");//������Դ
    }
    else if (SysVer == VER_WINDOWS_7)
    {
        hResID = ::FindResource(hInstance,MAKEINTRESOURCE(IDR_KERNEL_WIN7_SYS),L"KERNEL");//������Դ
    }

    HGLOBAL hRes = ::LoadResource(hInstance,hResID);//������Դ
    LPVOID pRes = ::LockResource(hRes);//������Դ

    if (pRes == NULL)//����ʧ��
    {
        return FALSE;
    }
    DWORD dwResSize = ::SizeofResource(hInstance,hResID);//�õ����ͷ���Դ�ļ���С
    HANDLE hResFile = CreateFile(m_strDriverPath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);//�����ļ�

    if (INVALID_HANDLE_VALUE == hResFile)
    {
        //TRACE("�����ļ�ʧ�ܣ�");
        return FALSE;
    }

    DWORD dwWritten = 0;//д���ļ��Ĵ�С	
    WriteFile(hResFile,pRes,dwResSize,&dwWritten,NULL);//д���ļ�
    CloseHandle(hResFile);//�ر��ļ����

    return (dwResSize == dwWritten);//��д���С�����ļ���С�����سɹ�������ʧ��
}

void CDriverManager::InitializeDriver()
{
    m_strDriverPath = GetSystemDir();
    m_strDriverPath += L"\\HideSys.sys";

    ExtractSysFile();
}

CString CDriverManager::GetSystemDir()
{
    CString strSystemPath;
    GetWindowsDirectory(strSystemPath.GetBuffer(MAX_PATH+1), MAX_PATH);
    strSystemPath.ReleaseBuffer();

    return strSystemPath;
}

BOOL CDriverManager::ClearFileCount()
{
    BOOL bRet;
    DWORD dwRet;

    bRet = DriverIoControl(IOCTL_CLEAR_FILE_COUNT, NULL, 0, &dwRet);

    return bRet;
}

BOOL CDriverManager::SetProcessHiddenState( LPCTSTR strProcessName, BOOL bState )
{
    BOOL bRet;
    DWORD dwRet;
    
    if (bState)
    {
        bRet = DriverIoControl(IOCTL_HIDE_PROCESS, strProcessName, (wcslen(strProcessName)+1)*sizeof(WCHAR), &dwRet);
    }
    else
    {
        bRet = DriverIoControl(IOCTL_SHOW_PROCESS, strProcessName, (wcslen(strProcessName)+1)*sizeof(WCHAR), &dwRet);
    }

    return bRet;
}

CString CDriverManager::GetCurrentProcessName()
{
    CString strCurProcessName;
    GetModuleFileName(NULL, strCurProcessName.GetBuffer(MAX_PATH), MAX_PATH);
    strCurProcessName.ReleaseBuffer();

    int nPos = strCurProcessName.ReverseFind(L'\\');
    strCurProcessName = strCurProcessName.Right(strCurProcessName.GetLength() - nPos -1);

    return strCurProcessName;
}

void CDriverManager::Initialize()
{
    InitializeDriver();
}

SystemVer CDriverManager::CheckWindowsVersion()
{
    OSVERSIONINFO OsVerInfo;
    OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&OsVerInfo);

    if (OsVerInfo.dwMajorVersion < 6) //XP System
    {
        return VER_WINDOWS_XP;
    }
    else
    {
       return VER_WINDOWS_7;
    }
}
