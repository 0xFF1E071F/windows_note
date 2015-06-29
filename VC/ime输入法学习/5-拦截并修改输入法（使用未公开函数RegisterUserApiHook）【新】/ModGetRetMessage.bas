Attribute VB_Name = "ModGetRetMessage"
Option Explicit
'*************************************************************************
'**ģ �� ����ModGetRetMessage
'**˵    ��������API����ֵ,�����Ӧ���ַ���˵��.����API����.
'**�� �� �ˣ��໽е�����
'**��    �ڣ�2009��4��16��
'**��    ע: ��ˮ�������� ��Ȩ����
'**          ����ģ��/��ģ���������վ:  http://www.m5home.com
'**��    ����V1.0
'*************************************************************************

Private Declare Function FormatMessage Lib "kernel32.dll" Alias "FormatMessageA" ( _
     ByVal dwFlags As Long, _
     ByRef lpSource As Any, _
     ByVal dwMessageId As Long, _
     ByVal dwLanguageId As Long, _
     ByVal lpBuffer As String, _
     ByVal nSize As Long, _
     ByRef Arguments As Long) As Long

'Private Const FORMAT_MESSAGE_FROM_HMODULE As Long = &H800
'Private Const FORMAT_MESSAGE_FROM_STRING As Long = &H400
'Private Const FORMAT_MESSAGE_FROM_SYSTEM As Long = &H1000

Public Enum MESSAGE_TYPE            'һ��ֻ�õ�������ֵ.�и�����Ҫ���Լ��Ӱ�.
    FORMAT_MESSAGE_FROM_HMODULE = &H800
    FORMAT_MESSAGE_FROM_STRING = &H400
    FORMAT_MESSAGE_FROM_SYSTEM = &H1000
End Enum

Public Function GetRetMessage(ByVal lRetValue As Long, ByVal lType As MESSAGE_TYPE) As String
    '����API����ֵ,�����Ӧ���ַ���˵��.
    'lRetValue - API�ķ���ֵ
    'lType - ��ʲô���͵���Ϣ����ַ���
    '����ֵ:
    '       �ɹ����صó����ַ���,ʧ�ܷ��ؿ��ַ���.
    Dim lRet As Long, sBuffer As String
    
    sBuffer = Space(255)
    lRet = FormatMessage(lType, ByVal 0&, lRetValue, 0, sBuffer, 255, ByVal 0&)
    GetRetMessage = Mid(sBuffer, 1, lRet)
End Function
