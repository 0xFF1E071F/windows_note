#include "AntiVM.h"
#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <Tlhelp32.h>

#pragma comment(lib,"Iphlpapi.lib")

AntiVM::AntiVM(void)
{
}

AntiVM::~AntiVM(void)
{
}



/*ͨ��ִ����Ȩָ������������
VmwareΪ�������������֮���ṩ���໥��ͨ��ͨѶ���ƣ���ʹ�á�IN��ָ������ȡ�ض��˿ڵ������Խ�������ͨѶ��
������INָ��������Ȩָ��ڴ��ڱ���ģʽ�µ������ִ�д�ָ��ʱ������Ȩ������
���򽫻ᴥ������Ϊ��EXCEPTION_PRIV_INSTRUCTION�����쳣������������в����ᷢ���쳣��
��ָ�����ܺ�0A����ȡVMware�汾��������£�������EBX�з�����汾�š�VMXH�����������ܺ�Ϊ0x14ʱ��
�����ڻ�ȡVMware�ڴ��С��������0ʱ��˵������������С�VMDetect��������ǰһ�ַ��������VMware�Ĵ��ڣ��������������£�*/


BOOL AntiVM::IsInsideVMWare_By_In()
{
	bool rc = true;

	__try
	{
		__asm
		{
				push   edx
				push   ecx
				push   ebx

				mov    eax, 'VMXh'
				mov    ebx, 0  // ��ebx����Ϊ�ǻ�����VMXH��������ֵ
				mov    ecx, 10 // ָ�����ܺţ����ڻ�ȡVMWare�汾������Ϊ0x14ʱ���ڻ�ȡVMware�ڴ��С
				mov    edx, 'VX' // �˿ں�
				in     eax, dx // �Ӷ˿�dx��ȡVMware�汾��eax
				//������ָ�����ܺ�Ϊ0x14ʱ����ͨ���ж�eax�е�ֵ�Ƿ����0��������˵�������������
				cmp    ebx, 'VMXh' // �ж�ebx���Ƿ����VMware�汾��VMXh�������������������
				setz   [rc] // ���÷���ֵ rc = ZF

			pop    ebx
				pop    ecx
				pop    edx
		}
	}
	__except(1)  //���δ����VMware�У��򴥷����쳣 EXCEPTION_EXECUTE_HANDLER = 1
	{
		rc = false;
	}

	return rc;
}

// ԭ��RDTSCָ�������ڽ����������������CPU������������ŵ�EDX��EAX���棬����EDX�Ǹ�λ����EAX�ǵ�λ
// ���������Զ����������
// ��������ֻ���õ�λ����
// Ҳ����anti debug ԭ������GetTickCount
BOOL AntiVM::IsInsideVmVare_By_RDTSC()
{
	BOOL bRet = FALSE;
	int value = 0;// xchg ecx, eaxָ������ʱ��
	__asm
	{
		RDTSC
			xchg ecx, eax // �������������ĵ�λ��ecx
			RDTSC
			sub eax, ecx // ����֮��  ��λ-��λ
			cmp eax, 0FFh // �ж��Ƿ�С��FF
			mov value, eax	// ���Կ�����С
			jl NotInVM	// С��0xFF bRet����0
			mov bRet,1	//����0xFF,bRet����1
NotInVM:	
	}

	printf("time: %d\n", value);
	return bRet;

}

// ������е�MACһ����VMwareĬ�ϵ�����MAC��ַǰ׺Ϊ��00-05-69��00-0C-29����00-50-56����
// ��ǰ3������VMware�����Ψһ��ʶ��OUI���Թ��������⻯������ʹ��
// VC6.0û�и��µ�SDK��֧��GetAdaptersInfo����
BOOL AntiVM::IsInsideVmVare_By_MAC()
{
	BOOL bRet = FALSE;
	// ����1�Ľṹ��
	PIP_ADAPTER_INFO tag_AdapterInfo = new IP_ADAPTER_INFO[sizeof(IP_ADAPTER_INFO)];
	PIP_ADAPTER_INFO lpTemp = tag_AdapterInfo;

	// ����2�Ĵ�С
	ULONG lLen = sizeof(IP_ADAPTER_INFO);

	GetAdaptersInfo(tag_AdapterInfo, &lLen);
	GetAdaptersInfo(tag_AdapterInfo, &lLen);

	// ѭ����ȡ�����ϵ�����������Ϣ.
	while(lpTemp)
	{
		// ��ȡMAC����ǰ6���ַ�
		lpTemp->Description[6] = '\0';
		/*printf("MAC: %02X-%02X-%02X-%02X-%02X-%02X\r\n",
		lpTemp->Address[0],
		lpTemp->Address[1],
		lpTemp->Address[2],
		lpTemp->Address[3],
		lpTemp->Address[4],
		lpTemp->Address[5]);*/

		// ǰ6���ַ����ΪVMware����ʵ���Ѿ�֤���������ˣ�
		// ���� �������װ�����
		if (strcmp(lpTemp->Description, "VMware") == 0)
		{
			if ((lpTemp->Address[0] == 0x00 && lpTemp->Address[1] == 0x0C && lpTemp->Address[2] == 0x29) ||
				(lpTemp->Address[0] == 0x00 && lpTemp->Address[1] == 0x05 && lpTemp->Address[2] == 0x69) ||
				(lpTemp->Address[0] == 0x00 && lpTemp->Address[1] == 0x50 && lpTemp->Address[2] == 0x56) )
			{
				bRet = FALSE;
				break;
			}
		}
		// ���ǰ����������VMware
		else
		{
			//MAC�����������3�飬���������
			if ((lpTemp->Address[0] == 0x00 && lpTemp->Address[1] == 0x0C && lpTemp->Address[2] == 0x29) ||
				(lpTemp->Address[0] == 0x00 && lpTemp->Address[1] == 0x05 && lpTemp->Address[2] == 0x69) ||
				(lpTemp->Address[0] == 0x00 && lpTemp->Address[1] == 0x50 && lpTemp->Address[2] == 0x56) )
			{
				bRet = TRUE;
				break;
			}
		}


		lpTemp = lpTemp->Next;
	}


	delete[] tag_AdapterInfo;
	return bRet;
}

// ԭ������ע�����HKEY_LOCAL_MACHINE\software\VMWare, Inc.\VMware tools��
BOOL AntiVM::IsInsideVmVare_By_Reg()
{
	BOOL bRet = FALSE;
	TCHAR szVMTools[] = _T("software\\VMWare, Inc.\\VMware tools");
	HKEY hSubKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szVMTools, 0, KEY_QUERY_VALUE, &hSubKey) == ERROR_SUCCESS)
	{
		bRet = TRUE;
	}
	return bRet;
}

// ԭ��ͨ��������vmtoolsd.exe����
BOOL AntiVM::IsInsideVmVare_By_Process()
{
	BOOL bRet = FALSE;


	TCHAR szVMTool[] = _T("vmtoolsd.exe");
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnap)
	{
		::MessageBox(NULL, _T("��������ʧ��"), _T("����"), MB_ICONERROR);
		return FALSE;
	}
	if (Process32First(hSnap, &pe))
	{
		do
		{
			if (lstrcmpi(szVMTool, pe.szExeFile) == 0)
			{
				bRet = TRUE;
				break;
			}
		} while (Process32Next(hSnap, &pe));
	}

	CloseHandle(hSnap);
	return bRet;

}

// ԭ��ͨ��STRָ���ȡSTR��ַ
// ���Է��֣�28000000Ϊ����� 40000000Ϊ��������������г���
BOOL AntiVM::IsInsideVmVare_By_STR()
{
	BOOL bRet = FALSE;
	unsigned char arr[4] = {0};

	__asm str arr;

	printf ("STR base: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%02X", arr[i]);
	}
	puts("");
	if (arr[0] == 0x28 && arr[1] == 0x00)
	{
		bRet = TRUE;
	}
	return bRet;
}
//�˷����Ѿ�ʧЧ����VMware LDT�Ļ�ַҲ����˺�����һ����0x00000000
BOOL AntiVM::IsInsideVmVare_By_LDT()
{
	BOOL bRet = FALSE;
	unsigned char arr[4] = {0};
	__asm sldt arr;
	printf ("LDT base: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%02X", arr[i]);
	}
	puts("");
	if (arr[0] == 0x00 && arr[1] == 0x00)
	{
		bRet = TRUE;
	}
	return bRet;
}
// ԭ����GDT��ַλ��0xFFXXXXXXʱ˵������������У�����Ϊ��ʵ����
BOOL AntiVM::IsInsideVmVare_By_GDT()
{
	BOOL bRet = FALSE;
	unsigned char arr[4] = {0};

	__asm sgdt arr;
	printf ("GDT base: ");
	for (int i = 0; i < 4; i++)
	{
		printf("%02X", arr[i]);
	}
	puts(""); // ����
	if (arr[0] == 0xff)
	{
		bRet = TRUE;
	}
	return bRet;
}