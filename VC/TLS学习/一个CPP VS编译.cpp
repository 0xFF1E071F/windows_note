#include "stdafx.h"
#include <windows.h>
#include <winnt.h>
//�������и�����������PE�ļ���Ҫ����TLSĿ¼
#pragma comment(linker, "/INCLUDE:__tls_used")
/*����PIMAGE_TLS_CALLBACK()������ԭ�ͣ����е�һ���͵����������������ڶ��������������������������*/
void NTAPI my_tls_callback(PVOID h, DWORD reason, PVOID pv)
{
/*һ�����ĸ�ѡ��DLL_PROCESS_ATTACH��DLL_THREAD_ATTACH��DLL_THREAD_DETACH��DLL_PROCESS_DETACH�����΢�����ġ�Microsoft Portable Executable and Common Object File Format Specification v8��*/
//���ڽ��̳�ʼ���������߳�ʱִ�еĴ���
	if( reason == DLL_PROCESS_ATTACH ){
		MessageBox(NULL,L"hi,this is tls callback",L"title",MB_OK);
	}
	return;
}
/*��������Ǵ���һ��tls��
".CRT$XLB"�ĺ����ǣ�
.CRT������ʹ��C RunTime����
$�����XLB��
X��ʾ����ı�ʶ
L��ʾ��TLS callback section
B���Ա�����B��Y������һ����ĸ�����ǲ���ʹ��".CRT$XLA"��".CRT$XLZ"
��Ϊ".CRT$XLA"��".CRT$XLZ"������tlssup.obj��
*/
#pragma data_seg(".CRT$XLB")
/*���Ҫ������TLS_CallBack���������԰��������д�ɣ�
PIMAGE_TLS_CALLBACK p_thread_callback [] = {tls_callback_A, tls_callback_B, tls_callback_C,0};
����tls_callback_B��tls_callback_CӦ�����㶨��õ�����TLS_callBack����
*/
PIMAGE_TLS_CALLBACK p_thread_callback = my_tls_callback;
#pragma data_seg()

int main(void)
{
	MessageBox(NULL,L"hi,this is main()",L"title",MB_OK);
  return 0;
}
