#include "SampleDLL.h"

extern "C" {

	SAMPLEDLL_API BOOL DeleteDirectory(LPCTSTR lpszDir)
	{
		if(lpszDir == NULL)
		{
			return FALSE;
		}
		WIN32_FIND_DATA fData = {0};
		TCHAR tcDirectory[MAX_PATH] = {0}, tcScanFileType[MAX_PATH] = {0};
		int iLength = strlen(lpszDir);
		if (iLength > MAX_PATH) //���Ŀ¼·��������ʧ��
		{
			return FALSE;
		}
		strcpy(tcScanFileType, lpszDir);  //����ɨ���ļ���
		if ('\\' == tcScanFileType[iLength - 1])
			strcat(tcScanFileType, "*.*");
		else
			strcat(tcScanFileType, "\\*.*"); //����ɨ���ļ�������
		HANDLE hFindHandle = FindFirstFile(tcScanFileType, &fData);
		if (hFindHandle != INVALID_HANDLE_VALUE)
		{
			do 
			{
				if ('.' == fData.cFileName[0])
				{   //�����ϲ�Ŀ¼
					continue;
				}
				strcpy(tcDirectory, lpszDir);  //����ɨ���ļ���
				if ( '\\' != tcDirectory[iLength - 1])
					strcat(tcDirectory, "\\");

				if (iLength + strlen(fData.cFileName) > MAX_PATH) //�ļ���������ʧ��
				{
					break;
				}
				strcat(tcDirectory, fData.cFileName); //���ļ��������ļ�����
				fData.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY;
				if (FILE_ATTRIBUTE_DIRECTORY == fData.dwFileAttributes)//�ж��Ƿ���Ŀ¼
				{
					DeleteDirectory(tcDirectory);  //�ݹ�ɾ����Ŀ¼
				}
				else
				{
					DeleteFile(tcDirectory);   //ɾ��Ŀ¼�е��ļ���
				}
			} while (FindNextFile(hFindHandle, &fData));
			FindClose(hFindHandle);
		}
		return RemoveDirectory(lpszDir); //ɾ��Ŀ¼
	}
}
