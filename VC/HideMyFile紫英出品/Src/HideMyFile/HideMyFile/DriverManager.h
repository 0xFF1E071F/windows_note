#pragma once
#include <WinIoCtl.h>

typedef enum _SystemVer
{
    VER_WINDOWS_XP = 0,
    VER_WINDOWS_7 =1
}SystemVer;

#define CDM_SERVICE_START_TYPE SERVICE_DEMAND_START
#define CDM_SERVICE_NAME L"HideSys"
#define CDM_DISPLAY_NAME L"HideSys"

//������ļ�
#define IOCTL_ADD_HIDDEN_FILE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x810,METHOD_BUFFERED, FILE_ANY_ACCESS)

//�����ļ�����
#define IOCTL_SET_FILE_HIDDEN CTL_CODE(FILE_DEVICE_UNKNOWN, 0x811,METHOD_BUFFERED, FILE_ANY_ACCESS)

//�����ļ���ʾ
#define IOCTL_SET_FILE_SHOWN CTL_CODE(FILE_DEVICE_UNKNOWN, 0x812,METHOD_BUFFERED, FILE_ANY_ACCESS)

//����ļ�����
#define IOCTL_CLEAR_FILE_COUNT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x813,METHOD_BUFFERED, FILE_ANY_ACCESS)

//���ؽ���
#define IOCTL_HIDE_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x815,METHOD_BUFFERED, FILE_ANY_ACCESS)

//��ʾ����
#define IOCTL_SHOW_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x816,METHOD_BUFFERED, FILE_ANY_ACCESS)


class CDriverManager
{
public:
    CDriverManager(void);
    ~CDriverManager(void);

public:
    void Initialize();

    //������������
    BOOL StartDriverService();

    //�ر���������
    BOOL StopDriverService();

    //��װ����
    BOOL InstallDriver();

    //ж������
    BOOL UninstallDriver();

    //����ļ�
    BOOL AddNewHiddenFile(LPCTSTR strFileName);

    //�����ļ�����״̬
    BOOL SetFileDriverHiddenState(LPCTSTR strFileName, BOOL bState);

    //���ý�������״̬
    BOOL SetProcessHiddenState(LPCTSTR strProcessName, BOOL bState);

    //����ļ�����
    BOOL ClearFileCount();

    //��ȡ��ǰ��������
    CString GetCurrentProcessName();

private:
    void InitializeDriver();

    SystemVer CheckWindowsVersion();

    BOOL DriverIoControl(DWORD dwControlCode, LPCTSTR InputBuffer, DWORD dwInputSize, LPDWORD dwRet);

    //�ͷ������ļ�
    BOOL ExtractSysFile();

    //��ȡϵͳĿ¼
    CString GetSystemDir();
private:
    CString m_strDriverPath;
};

