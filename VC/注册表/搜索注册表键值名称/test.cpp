#include <windows.h>
#include <stdio.h>

#define MAINKEY HKEY_LOCAL_MACHINE		// ����
int ResultCount = 0;					// ����������ֵ��

// ������δ����Ǵ����ֵ�ģ���һ��������ʹ��API��ע����ж�������ֵ
// �ڶ�����������Ҫ���ҵ�ֵ�����粡����������Ŀ����Ҫ����ʾ��д�ĺܼ�
bool StealReg(char KeyValue[MAX_PATH],char Virus[MAX_PATH])
{	// ������Ҫʵ�ֵľ��幦�ܴ�ҿɸ���ʵ�������������
	if(strcmp(KeyValue,Virus) == 0)
	{
		return true;
	}
	return false;
}

// �ú���Ψһ�Ĳ�����SubKey������"software\\Microsoft"
void EnumReg(char SubKey[MAX_PATH])
{
	char temp[MAX_PATH];
	HKEY hKey = NULL;
	char str[MAX_PATH];	
	DWORD num = sizeof(str),index = 0,rc;
	
	rc = ::RegOpenKeyEx(MAINKEY,SubKey,0,KEY_ALL_ACCESS,&hKey);
	if(rc == ERROR_SUCCESS)
	{
		while( RegEnumValue(hKey,index,str,&num,NULL,NULL,NULL,NULL)==0 )
		{	//���ȱ���ֵ�����д���
			printf("\t%s\n",str);
			if(StealReg(str,"F:\\BCB\\Program Files\\Common Files\\Borland Shared\\BDE\\"))
			{	//����ڶ�������������Ҫ���ҵ�ֵ ��ֵ����
				ResultCount++;////////////////
			}
			index++;
			num = MAX_PATH;
		}
		index = 0;
		while( RegEnumKeyEx(hKey,index,str,&num,NULL,NULL,NULL,NULL)==0 )
		{	//Ȼ������������еݹ�
			printf("%s\n",str);
			strcpy(temp,SubKey);
			strcat(temp,"\\");
			strcat(temp,str);
			EnumReg(temp);			//�ݹ�
			
			index++;
			num = MAX_PATH;
		}
	}
	else
	{
		printf("Can't Open The Key!\n");
	}
	
	RegCloseKey(hKey);
}

void main()
{
	EnumReg("software");
	printf("\n\n����������ֵ���У�%d ��!\n\n",ResultCount);
}