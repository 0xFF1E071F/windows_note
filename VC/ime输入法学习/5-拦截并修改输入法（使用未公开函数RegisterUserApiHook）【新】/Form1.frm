VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3885
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6795
   LinkTopic       =   "Form1"
   ScaleHeight     =   3885
   ScaleWidth      =   6795
   StartUpPosition =   3  '����ȱʡ
   Begin VB.TextBox Text1 
      Height          =   2895
      Left            =   240
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   2
      Top             =   240
      Width           =   6255
   End
   Begin VB.CommandButton Command2 
      Caption         =   "�ر�����"
      Height          =   375
      Left            =   3960
      TabIndex        =   1
      Top             =   3360
      Width           =   1455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "������"
      Height          =   375
      Left            =   960
      TabIndex        =   0
      Top             =   3360
      Width           =   1455
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
DLLsetHOOKState True
End Sub

Private Sub Command2_Click()
DLLsetHOOKState False
End Sub

Private Sub Form_Load()
OSVERSIONINFOX.dwOSVersionInfoSize = Len(OSVERSIONINFOX)
GetVersionEx OSVERSIONINFOX  'ȡ�ò���ϵͳ�汾
DLLhwnd = LoadLibrary("hxwdllwx.dll")   '����DLL
DLLFunDre = GetProcAddress(DLLhwnd, "DLLstopHOOK")    '��ȡ�ص�������ַ

WM_HXWDLLWX_QQBTX = RegisterWindowMessage("WM_HXWDLLWX_QQBTX")  'ע���Զ�����Ϣ
PrevWndProc = SetWindowLong(Me.hWnd, GWL_WNDPROC, AddressOf SubWndProc) '���໯���ڣ��Ա��ܴ���DLL�������Զ�����Ϣ
' ------------------------
Dim retV As Long, tmpStr As String
tmpStr = App.Path
If Right(tmpStr, 1) <> "\" Then tmpStr = tmpStr & "\"
' ------------------
ShellExecute Me.hWnd, "open", "net", "stop Themes", vbNullString, SW_HIDE
Sleep 2000
' ------------------
If OSVERSIONINFOX.dwMajorVersion = 5 And OSVERSIONINFOX.dwMinorVersion < 2 Then
    '�����Windows XP
    retV = RegisterUserApiHookXP(DLLhwnd, DLLFunDre)    'ע��ص�����
Else
    ' �����Windows 2003
    HookAPIRegInfoX2003.m_size = Len(HookAPIRegInfoX2003)
    HookAPIRegInfoX2003.m_dllname1 = StrPtr("hxwdllwx.dll")
    HookAPIRegInfoX2003.m_dllname2 = StrPtr("hxwdllwx.dll")
    HookAPIRegInfoX2003.m_funname1 = StrPtr("DLLstopHOOK")
    HookAPIRegInfoX2003.m_funname2 = StrPtr("DLLstopHOOK")
    retV = RegisterUserApiHook2003(HookAPIRegInfoX2003)
    If retV = 0 Then
        HookAPIRegInfoX2003.m_size = Len(HookAPIRegInfoX2003)
        HookAPIRegInfoX2003.m_dllname1 = StrPtr(tmpStr & "hxwdllwx.dll")
        HookAPIRegInfoX2003.m_dllname2 = StrPtr(tmpStr & "hxwdllwx.dll")
        HookAPIRegInfoX2003.m_funname1 = StrPtr("DLLstopHOOK")
        HookAPIRegInfoX2003.m_funname2 = StrPtr("DLLstopHOOK")
        retV = RegisterUserApiHook2003(HookAPIRegInfoX2003)
    End If
End If
If retV = 0 Then MsgBox "�����ʼ��ʧ�ܣ�������ϢΪ��" & vbCrLf & GetRetMessage(Err.LastDllError, FORMAT_MESSAGE_FROM_SYSTEM), vbCritical, "ע��ʧ�ܣ�"
DLLstartHOOK Me.hWnd '��ʼ��DLL
End Sub

Private Sub Form_Unload(Cancel As Integer)
DLLsetHOOKState False
UnregisterUserApiHook  '�˳�ʱж��HOOK
Call SetWindowLong(Me.hWnd, GWL_WNDPROC, PrevWndProc)  '��ԭ���໯����
FreeLibrary DLLhwnd
ShellExecute Me.hWnd, "open", "net", "start Themes", vbNullString, SW_HIDE
End Sub

Private Sub Text1_Change()
Text1.SelStart = Len(Text1.Text)
End Sub
