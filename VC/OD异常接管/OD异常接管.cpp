#include <windows.h>
#include <stdio.h>

int main(void)
{
	//�����쳣
	_try
	{
		FARPROC proAPI = GetProcAddress(LoadLibrary("user32.dll"), "MessageBoxA");

		//�޸����ڴ�����
		DWORD dwOldProtect = 0;
		VirtualProtect(proAPI, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);

		//�޸�Ϊ0xCC
		*(BYTE *)proAPI = 0xCC; //������޸��ڴ����ԣ�����ʾ�����쳣
		
		//��ʱ�����쳣
		MessageBoxA(NULL, "�����쳣", "-(", MB_ICONERROR);

	}

	//�����쳣
	_except(1)
	{
		MessageBoxW(NULL, L"�����쳣", L"-)", MB_ICONINFORMATION);
	}
	return 0;
}