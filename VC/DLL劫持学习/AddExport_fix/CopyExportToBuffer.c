#include <WINDOWS.H>
#include "AddExport.h"

#define NFIND_ORD 0x0;
#define FIND_ORD 0x1;

/*
������
lpFileָ���ڴ�ӳ���ļ�
lpExportָ���ڴ�ӳ���ļ��ĵ�����
lpBufferָ����Ҫ���ڴ�ӳ���ļ��ĵ����������ݿ��������ڴ����򣬸��ڴ���������µ�PE�ļ��ڴ�ӳ��ĵ������ַ
lpTransitNameָ����תDLL��
RvaΪ�µ�PE�ļ����ڴ�ӳ��ĵ������ַ��Rva��Ҳ����lpBuffer��Rva
���ܣ���lpFileָ����ڴ�ӳ���ļ��ĵ�������������Ϣ���������ļ����ڴ�ӳ����
����ֵ������NULLΪ�������򷵻�FALSE
*/

BOOL WINAPI CopyExportToBuffer(PIMAGE_DOS_HEADER lpFile,DWORD lpExport,DWORD lpBuffer,LPSTR lpTransitName,DWORD Rva)
{
	PIMAGE_EXPORT_DIRECTORY lpExp;	//����ָ�򵼳����ָ��
	PIMAGE_EXPORT_DIRECTORY lpBuff;	//�����µ������ָ��
	PCHAR lpTmp;	//��ʱָ�������ָ����һ����Ҫ��д��lpBuffer
	DWORD *lpFunRva;	//���ָ�����ļ��ĺ�����ַRva����ָ��
	DWORD *lpNameRva;	//���ָ�����ļ��ĺ�������Rva�����ָ��
	WORD *lpNameOrd;	//���ָ�����ļ��ĺ�����������ָ��

	DWORD *lpOldFunRva;	//���ָ�򱻿����ļ��ĺ�����ַRva����ָ��
	DWORD *lpOldNameRva;	//���ָ�򱻿����ļ��ĺ�������Rva�����ָ��
	WORD *lpOldNameOrd;	//���ָ�򱻿����ļ��ĺ�����������ָ��
	DWORD lpName;	//��ʱ���ָ���ַ�����ָ��
	DWORD nBase;	//��ű������ļ��ĵ���������ʼ���
	DWORD i;
	DWORD m;
	DWORD f;
//	static char szFormat[] = "Bommon.%s";
//	static char szFormatOrd[] = "Bommon.#%d";
	static char szFormat[] = "%.259s.%.259s";	//��ʽ��otherdll.exportname
	static char szFormatOrd[] = "%.259s.#%d";
	static char szBuffer[MAX_PATH * 2 + 2];

//	wsprintfA(szBuffer1,szFormat,lpTransitName);
//	wsprintfA(szBuffer2,szFormatOrd,lpTransitName);

	lpBuff = lpBuffer;
	lpExp = lpExport;
	//���������ṹ��Ա
	lpBuff->Base = lpExp->Base;
	lpBuff->NumberOfFunctions = lpExp->NumberOfFunctions;
	lpBuff->NumberOfNames = lpExp->NumberOfNames;
	//Ԥ��AddressOfFunctions��AddressOfNames��AddressOfNameOrdinals�ռ�
	lpTmp = lpBuff;		//���µ������ַ������ʱָ�����
	lpTmp += sizeof (IMAGE_EXPORT_DIRECTORY);	//�ƶ���ʱָ�뱣��������ṹ����ռ�
	lpFunRva = lpTmp;	//����ָ������ַ��Rva����ָ��
	lpBuff->AddressOfFunctions = lpTmp-lpBuff+Rva;	//������������Rva��AddressOfFunctions
	lpTmp += ((lpExp->NumberOfFunctions)*(sizeof (DWORD)));	//�ƶ�ָ�뱣��������ַRva��������ռ�
	lpNameRva = lpTmp;	//����ָ�������ֵ�Rva����ָ��
	lpBuff->AddressOfNames = lpTmp-lpBuff+Rva;	//������������Rva��AddressOfNames
	lpTmp += ((lpExp->NumberOfNames)*(sizeof (DWORD)));	//�ƶ�ָ�뱣����������Rva��������ռ�
	lpNameOrd = lpTmp;	//���溯����ŵ�����ָ��
	lpBuff->AddressOfNameOrdinals = lpTmp-lpBuff+Rva;	//������������Rva��AddressOfNameOrdinals
	lpTmp += (lpExp->NumberOfNames)*(sizeof (WORD));	//�ƶ�ָ�뱣�����������������ռ�

	//�õ�ԭʼ������ַ����ȵ�offset
	lpOldFunRva = RvaToOffset(lpFile,lpExp->AddressOfFunctions);
	lpOldNameRva = RvaToOffset(lpFile,lpExp->AddressOfNames);
	lpOldNameOrd = RvaToOffset(lpFile,lpExp->AddressOfNameOrdinals);
	nBase = lpExp->Base;
	//ѭ������������ڵ�ַRva����Rvaָ����ת�ַ�����Dll.Name1
	//����ֻ���Ե�����ŵ����ĺ������򽫵�������Rvaָ��DllName.#19������ʽ����ת�ַ���
	for (i=0;lpExp->NumberOfFunctions > i;i++)	//�����к�����Ŀ��Ϊ���ѭ������
	{
		f = NFIND_ORD;	//���ñ�־��NFIND_ORDΪδ�ҵ��������ֶ�Ӧ��Ord��Ҳ����ֻ������ŵ����ĺ�������֮
		for (m=0;m < lpExp->NumberOfNames;m++)
		{
			if (i==lpOldNameOrd[m])
			{
				f = FIND_ORD;
				break;
			}
		}
		if (!f)
		{
			//��������ַ���
			lpFunRva[i] = lpTmp-lpBuff+Rva;
			wsprintfA(szBuffer,szFormatOrd,lpTransitName,i+nBase);
			lstrcpyA(lpTmp,szBuffer);
			lpTmp += lstrlenA(szBuffer);
			lpTmp++;
			continue;
		}

		//������ת�ַ���
		lpFunRva[i] = lpTmp-lpBuff+Rva;
		lpName = RvaToOffset(lpFile,lpOldNameRva[m]);
		wsprintfA(szBuffer,szFormat,lpTransitName,lpName);
		lstrcpyA(lpTmp,szBuffer);
		lpTmp += lstrlenA(szBuffer);
		lpTmp++;
	}
	
	//����ԭʼ������������дRva
	for (i=0;lpExp->NumberOfNames > i;i++)
	{
		lpNameRva[i] = lpTmp-lpBuff+Rva;
		lpName = RvaToOffset(lpFile,lpOldNameRva[i]);
		//wsprintfA(szBuffer,szFormat,lpName);
		lstrcpyA(lpTmp,lpName);
		lpTmp += lstrlenA(lpName);
		lpTmp++;
	}

	//����NameOrdinals����
	for (i=0;lpExp->NumberOfNames > i;i++)
		lpNameOrd[i] = lpOldNameOrd[i];
	//����ԭʼDLL����
	lpName = RvaToOffset(lpFile,lpExp->Name);
	lstrcpyA(lpTmp,lpName);
	lpBuff->Name = lpTmp-lpBuff+Rva;

	return TRUE;
}
