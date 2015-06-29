#pragma once

//��ֵ����
enum CCMConfigType
{
    CCM_TYPE_AUTOSTART = 0,
    CCM_TYPE_SHOWTRAY = 1,
    CCM_TYPE_CLOSESTATE = 2,
    CCM_TYPE_HIDEPROCESS = 3,
    CCM_TYPE_CONFIGPWD = 4
};

//INI��ֵ��
#define CCM_APP_NAME L"HideConfig"
#define CCM_KEY_AUTOSTART L"AutoStart "
#define CCM_KEY_SHOWTRAY L"ShowTrayOnStart"
#define CCM_KEY_CLOSTSTATE L"CloseState"
#define CCM_KEY_HIDEPROCESS L"HideProcess"
#define CCM_KEY_CONFIGPWD L"ConfigPwd"

class CConfigManager
{
public:
    CConfigManager(void);
    ~CConfigManager(void);

public:

    void Initialize();

     UINT GetConfigValueInt(CCMConfigType nCCMConfigType);

     CString GetConfigValueString(CCMConfigType nCCMConfigType);

     BOOL SetConfigValueInt(CCMConfigType nCCMConfigType, int nValue);

     BOOL SetConfigValueString(CCMConfigType nCCMConfigType, CString strValue);

     //XOR���ܽ�������
     CString XORPassword(CString &strSrc);

     //��ȡϵͳ�ļ���·�� 
     CString GetSystemDir();
private:
    //��ʼ�������ļ�
    BOOL InitializeConfigFile();

    //���������ļ�
    BOOL LoadConfigFile();

private:
    CString m_ConfigFilePath; //�����ļ�·��
    int m_bAutoStart;//��������
    int m_bShowTrayOnStart;//��ʾ����ͼ��
    int m_bHideProcess;//���ؽ���
    int m_bCloseState;//�رհ�ťʱ����
    CString m_ConfigPwd;
};

