Attribute VB_Name = "Module1"
Public Declare Function RegisterWindowMessage Lib "user32" Alias "RegisterWindowMessageA" (ByVal lpString As String) As Long
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
' ------------------------DLL��������-----------------------------
Public Declare Function DLLstartHOOK Lib "hxwdllwx.dll" (ByVal hwnd As Long) As Long   '��ʼ������
Public Declare Function DLLstopHOOK Lib "hxwdllwx.dll" () As Long   'ж�ع���
Public Declare Function DLLsetHOOKState Lib "hxwdllwx.dll" (ByVal myState As Boolean) As Long  '�򿪻�رչ���
Public Declare Function DLLGetPubString Lib "hxwdllwx.dll" () As String   '������뷨����
Public Declare Function DLLSetPubString Lib "hxwdllwx.dll" (ByVal tmpstr As String) As Long   '�޸����뷨����
' ----------------------------------------------------------------
Public Const GWL_WNDPROC = -4

Public WM_HXWDLLWX_QQBTX As Long  '�Զ�����Ϣ
Public WM_HXWDLLWX_HOOKKEY As Long
Public PrevWndProc As Long '����ɵĴ��ڴ�������ַ


Public Function SubWndProc(ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Static LastIMEInput As String, NowImeCharIndex As Long   '���һ�����뷨���룬IME��Ϣ������
Dim tmpS As String
If Msg = WM_HXWDLLWX_QQBTX Then
'����յ������뷨������Ϣ
    tmpS = DLLGetPubString() '������뷨����
    Form1.Text1.Text = Form1.Text1.Text & "���ص����뷨������Ϣ��" & tmpS & vbCrLf
    'tmpS = tmpS & "�����޸ģ�"
    'DLLSetPubString tmpS   '�޸����뷨����
    LastIMEInput = tmpS   '���һ�����뷨����
    NowImeCharIndex = 0  'WM_IME_CHAR��Ϣ����
End If
If Msg = WM_HXWDLLWX_HOOKKEY Then
'����յ�����WM_IME_CHAR����Ϣ
    NowImeCharIndex = NowImeCharIndex + 1
    Form1.Text1.Text = Form1.Text1.Text & "���ص�IME��Ϣ���ַ���" & Chr(wParam)
    If Chr(wParam) <> Mid(LastIMEInput, NowImeCharIndex, 1) Then
        LastIMEInput = ""
        Form1.Text1.Text = Form1.Text1.Text & " (�������������)"
    End If
    Form1.Text1.Text = Form1.Text1.Text & vbCrLf
End If
SubWndProc = CallWindowProc(PrevWndProc, hwnd, Msg, wParam, lParam)   '����Ϣ�����ɵĴ��ں�����������
End Function
