/* 
�������ܣ��ͷ���Դ�ļ� 
 
����˵����DWORD dwResName   ָ��Ҫ�ͷŵ���ԴID�ţ���IDR_EXE 
      LPCSTR lpResType ָ���ͷŵ���Դ����Դ���ͣ���"EXE" 
      LPCSTR lpFilePathName ָ���ͷź��Ŀ���ļ���,��"D:\\Test.exe" 
 
����ֵ���ɹ��򷵻�TRUE,ʧ�ܷ���FALSE 
*/  
BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName )  
{     
    HMODULE hInstance = ::GetModuleHandle(NULL);//�õ�����ʵ�����  
      
    HRSRC hResID = ::FindResource(hInstance,MAKEINTRESOURCE(dwResName),lpResType);//������Դ  
    HGLOBAL hRes = ::LoadResource(hInstance,hResID);//������Դ  
    LPVOID pRes = ::LockResource(hRes);//������Դ  
      
    if (pRes == NULL)//����ʧ��  
    {  
        return FALSE;  
    }  
    DWORD dwResSize = ::SizeofResource(hInstance,hResID);//�õ����ͷ���Դ�ļ���С  
    HANDLE hResFile = CreateFile(lpFilePathName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);//�����ļ�  
      
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