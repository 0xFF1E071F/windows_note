VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "���뷨HOOK - �������뷨�����ע��DLL"
   ClientHeight    =   4155
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6840
   LinkTopic       =   "Form1"
   ScaleHeight     =   4155
   ScaleWidth      =   6840
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command1 
      Caption         =   "������"
      Height          =   375
      Left            =   960
      TabIndex        =   2
      Top             =   3600
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "�ر�����"
      Height          =   375
      Left            =   3960
      TabIndex        =   1
      Top             =   3600
      Width           =   1455
   End
   Begin VB.TextBox Text1 
      Height          =   2895
      Left            =   240
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   480
      Width           =   6255
   End
   Begin VB.Label Label1 
      Caption         =   "�ȵ���������ء���ť��Ȼ�����κ��������������뷨���뺺�����Կ���"
      Height          =   255
      Left            =   240
      TabIndex        =   3
      Top             =   120
      Width           =   6135
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
' ---------���ϵͳ·���͵�ǰ·��------
GSystemPath = Environ("windir")
If Right(GSystemPath, 1) <> "\" Then GSystemPath = GSystemPath & "\"
GSystemPath = GSystemPath & "system32\"
If Dir(GSystemPath) = "" Then GSystemPath = Environ("windir") & "\system\"
GPathStr = App.Path
If Right(GPathStr, 1) <> "\" Then GPathStr = GPathStr & "\"
' -----------------------------------
SystemParametersInfo SPI_GETDEFAULTINPUTLANG, 0, GSysDefIme, 0 '�õ�ϵͳĬ�ϵ����뷨�ľ���������˳�ʱ��ԭ
' -----------------------------------
GImeFileName = "imedllhost09"    '�������뷨�ļ�����������չ��
CilentDllPath = GPathStr & "hxwdllwx.dll"    '��ע���Ŀ��DLL����·��
'-----------------------------------
WM_HXWDLLWX_QQBTX = RegisterWindowMessage("WM_HXWDLLWX_QQBTX")  'ע���Զ�����Ϣ
DLLstartHOOK Me.hWnd '��ʼ��Ŀ��DLL
' ---------------------------------
On Error Resume Next
Dim retV As Long
retV = MSetUpIME(GImeFileName)   '��װָ�������뷨
If retV = 0 Then
    retV = MImeFindByName(GImeFileName)   '�����װʧ�ܣ���ѯ�����뷨�Ƿ��Ѿ���װ
End If
If retV <> 0 Then ImeHKL = retV   '�������뷨�����ȫ�ֱ���

If retV <> 0 Then
    '�����װ�ɹ����򽫴����뷨����ΪĬ�����뷨���ó���һ�򿪾ͼ���
    ImeHKLString = MGetIMEHwndString(retV)   'ȡ�����뷨�ľ���ַ���
    'If ImeHKLString <> "" Then MSetIMEIntoFirst ImeHKLString    '�����װ�ɹ�����ָ�����뷨�ƶ������뷨�б�ĵ�һ���һ����������ȥ�������Ӱ���û������뷨˳��
    MSetIMEIsDefInput retV   '��ָ�����뷨����ΪϵͳĬ�����뷨��Ӧ�ó���һ�򿪾ͼ���
End If

If Dir(GSystemPath & GImeFileName & ".ime") <> "" Then
' ������뷨����װ��ϵͳĿ¼�����Խ����ʼ�������䴫����ע���Ŀ��DLL����
    IMESetPubString CilentDllPath, 0, 0, 0, 0, 0
    '�ú����Ĳ������������������:
    '��ע���Ŀ��DLL����·����
    '�����뷨�˳�ʱ���Ƿ�ͬʱж��Ŀ��DLL 0-�ǣ�1-��
    '���л��������뷨ʱ���Ƿ�ֱ���л�����һ�����뷨�����������뷨�ͺ��������ˣ�����С�޶�Ӱ���û������뷨˳�� 0-��1-��
    '���3�������Ǵ��ݸ�Ŀ��DLL�Ļص��������������Ʊ���ΪRunDllHostCallBack���Ĳ���
End If

Dim hIme As Long
If retV <> 0 Then
    '�����װ�ɹ������������Ѿ����еĳ����м�������뷨��������ʵ�������д���ͬʱע��Ŀ��DLL
    MActiveIMEForWindows retV, 1   '��һ������Ϊ���뷨������ڶ�������Ϊ����ǿ�ȣ�0-ֻ�����ڶ��������м��1-�����д����м���
Else
    hIme = MImeFindByName(GImeFileName)
    If hIme <> 0 Then MActiveIMEForWindows hIme, 1
End If

If retV = 0 Then
    'MsgBox "��Ǹ�����뷨��װʧ�ܡ�������û�й���ԱȨ�ޣ����߸����뷨�����Ѿ���װ��", vbCritical
End If
' ---------------------------------
PrevWndProc = SetWindowLong(Me.hWnd, GWL_WNDPROC, AddressOf SubWndProc) '���໯���ڣ��Ա��ܴ���DLL�������Զ�����Ϣ
End Sub

Private Sub Form_Unload(Cancel As Integer)
DLLsetHOOKState False    '�˳�ʱһ��Ҫ��ȷ�ر�Ŀ��DLL��HOOK����Ȼ���г��򶼻������
Call SetWindowLong(Me.hWnd, GWL_WNDPROC, PrevWndProc)  '��ԭ���໯����
' --------------------------
On Error Resume Next
If Dir(GSystemPath & GImeFileName & ".ime") <> "" Then
    ' ������뷨���ã�ֹͣ����ע��
    IMEClearPubString
End If
' --------------------------
Dim retV As Long
MQuitIMEIsDefInput GSysDefIme    '���Ȼ�ԭϵͳĬ�ϵļ��̲���
MActiveIMEForWindows GSysDefIme, 0  '�����ж�����������ΪĬ�����뷨
If ImeHKL <> 0 Then
    retV = MUnLoadIMEByHKL(ImeHKL)
    If retV = 0 Then
        'MsgBox "�������뷨ж��ʧ�ܡ����ܸ����뷨����ʹ���ұ�������", vbCritical
    Else
        'MsgBox "�������뷨�ѳɹ�ж�ء�", vbExclamation
    End If
Else
    'retV = MsgBox("�������뷨���ܲ��Ǳ�������ʱ��װ�ġ��Ƿ�Ҫ����ϵͳ���е����뷨������ж������Ϊ��Windows��׼���뷨��չ���񡱵����뷨��", vbExclamation Or vbYesNo Or vbDefaultButton1)
    retV = vbYes
    If retV = vbYes Then
        retV = MUnLoadIMEByName(GImeFileName)   '����ͨ���ļ���ж�����뷨
        If retV = 0 Then
            'MsgBox "�������뷨ж��ʧ�ܡ����ܸ����뷨����ʹ���ұ�������������뷨������", vbCritical
        Else
            'MsgBox "�������뷨�ѳɹ�ж�ء�", vbExclamation
        End If
    End If
End If
If ImeHKLString <> "" Then
    MDeleteRegIme ImeHKLString   '��ע�����ɾ�����뷨������Ϣ
End If
End Sub

Private Sub Text1_Change()
Text1.SelStart = Len(Text1.Text)
End Sub
