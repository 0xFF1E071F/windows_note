Attribute VB_Name = "Module1"
Public Declare Function RegisterWindowMessage Lib "user32" Alias "RegisterWindowMessageA" (ByVal lpString As String) As Long
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
' ------------------------DLL��������-----------------------------
Public Declare Function DLLstartHOOK Lib "hxwdllwx.dll" (ByVal hwnd As Long) As Long   '��ʼ������
Public Declare Function DLLstopHOOK Lib "hxwdllwx.dll" () As Long   'ж�ع���
Public Declare Function DLLsetHOOKState Lib "hxwdllwx.dll" (ByVal myState As Boolean) As Long  '�򿪻�رչ���
Public Declare Function DLLGetPubString Lib "hxwdllwx.dll" () As String   '������뷨����
Public Declare Function DLLSetPubString Lib "hxwdllwx.dll" (ByVal tmpstr As String) As Long   '�޸����뷨����
Public Declare Function DLLGetPubMsg Lib "hxwdllwx.dll" () As Long   '������ص��ļ�����Ϣ,����һ��lpMSG���͵�ָ��
' ----------------------------------------------------------------
Public Type POINTAPI
        x As Long
        y As Long
End Type

Public Type lpMSG
' ����windows��Ϣ����
  hwnd As Long
  message As Long
  wParam As Long
  lParam As Long
  time As Long
  pt As POINTAPI
End Type


Public Const GWL_WNDPROC = -4
Public Const WM_KEYDOWN = &H100
Public Const WM_CHAR = &H102

Public WM_HXWDLLWX_QQBTX As Long  '�Զ�����Ϣ
Public WM_HXWDLLWX_HOOKKEY As Long
Public PrevWndProc As Long '����ɵĴ��ڴ�������ַ


Public Function SubWndProc(ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Dim tmpS As String, myMSG As lpMSG, MSGPoint As Long
If Msg = WM_HXWDLLWX_QQBTX Then
'����յ������뷨������Ϣ
    tmpS = DLLGetPubString() '������뷨����
    Form1.Text1.Text = Form1.Text1.Text & "���ص����뷨���룺" & tmpS & vbCrLf
    tmpS = tmpS & "�����޸ģ�"
    DLLSetPubString tmpS   '�޸����뷨����
End If
If Msg = WM_HXWDLLWX_HOOKKEY Then
'����յ����Ǽ���������Ϣ
    MSGPoint = DLLGetPubMsg()
    CopyMemory myMSG, ByVal MSGPoint, Len(myMSG) '��ָ��MSGPoint��ָ���ڴ������Ƶ�myMSG�ṹ��
    If myMSG.message = WM_KEYDOWN Then
        Form1.Text1.Text = Form1.Text1.Text & "���ص����̰�����Ϣ��ɨ���룺" & myMSG.lParam & " ������" & Chr(myMSG.wParam) & vbCrLf
        '����ʾ��������޸İ�����Ϣ��������޸���Ϣ���κ����ݣ������޸�myMSG.message
        If myMSG.wParam = Asc("B") Then myMSG.wParam = Asc("A") '�޸İ�����Ϣ��������B��Ϊ���°�����A��
    End If
    If myMSG.message = WM_CHAR Then
        Form1.Text1.Text = Form1.Text1.Text & "���ص�Ӣ���ַ���Ϣ��ɨ���룺" & myMSG.lParam & " �ַ���" & Chr(myMSG.wParam) & vbCrLf
    End If
    CopyMemory ByVal MSGPoint, myMSG, Len(myMSG)  '��myMSG�����ݸ��ƻ�MSGPoint��ָ���ڴ�����
End If
SubWndProc = CallWindowProc(PrevWndProc, hwnd, Msg, wParam, lParam)   '����Ϣ�����ɵĴ��ں�����������
End Function
