VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "�������뷨�����ע��DLL"
   ClientHeight    =   5220
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6840
   LinkTopic       =   "Form1"
   ScaleHeight     =   5220
   ScaleWidth      =   6840
   StartUpPosition =   3  '����ȱʡ
   Begin VB.TextBox Text6 
      Height          =   270
      Left            =   5520
      TabIndex        =   17
      Text            =   "0"
      Top             =   2680
      Width           =   375
   End
   Begin VB.CommandButton Command4 
      Caption         =   "ж���Ѱ�װ�ķ������뷨"
      Height          =   375
      Left            =   2880
      TabIndex        =   15
      Top             =   4680
      Width           =   2295
   End
   Begin VB.TextBox Text5 
      Height          =   270
      Left            =   4680
      TabIndex        =   13
      Text            =   "0"
      Top             =   2400
      Width           =   375
   End
   Begin VB.CommandButton Command3 
      Caption         =   "ֹͣ����ע��"
      Height          =   375
      Left            =   240
      TabIndex        =   11
      Top             =   4680
      Width           =   1935
   End
   Begin VB.TextBox Text4 
      Height          =   270
      Left            =   6000
      TabIndex        =   10
      Text            =   "0"
      Top             =   2160
      Width           =   495
   End
   Begin VB.TextBox Text3 
      Height          =   270
      Left            =   6000
      TabIndex        =   8
      Text            =   "0"
      Top             =   1680
      Width           =   495
   End
   Begin VB.TextBox Text2 
      Height          =   270
      Left            =   6000
      TabIndex        =   6
      Text            =   "0"
      Top             =   1275
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   1800
      TabIndex        =   3
      Text            =   "hxwdllwx.dll"
      Top             =   840
      Width           =   4695
   End
   Begin VB.CommandButton Command1 
      Caption         =   "��һ����ָ��Ҫע���DLL����ص���������"
      Height          =   495
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   3975
   End
   Begin VB.CommandButton Command2 
      Caption         =   "�ڶ�������װ�������뷨��ע��ָ��DLL"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   3480
      Width           =   4095
   End
   Begin VB.Label Label8 
      Caption         =   "���л���Ŀ�����뷨ʱ�Զ��л�����һ�����뷨��0-��1-�ǣ���"
      Height          =   255
      Left            =   240
      TabIndex        =   16
      Top             =   2760
      Width           =   5295
   End
   Begin VB.Label Label7 
      Caption         =   "������������ѡ����"
      Height          =   255
      Left            =   240
      TabIndex        =   14
      Top             =   4320
      Width           =   3495
   End
   Begin VB.Label Label6 
      Caption         =   "���뷨�˳�ʱ�Ƿ�ͬʱж��ע���DLL��0-�ǣ�1-�񣩣�"
      Height          =   255
      Left            =   240
      TabIndex        =   12
      Top             =   2450
      Width           =   4455
   End
   Begin VB.Label Label5 
      Caption         =   "����3��"
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   5280
      TabIndex        =   9
      Top             =   2205
      Width           =   735
   End
   Begin VB.Label Label4 
      Caption         =   "����2��"
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   5280
      TabIndex        =   7
      Top             =   1725
      Width           =   735
   End
   Begin VB.Label Label3 
      Caption         =   "����1��"
      ForeColor       =   &H00FF0000&
      Height          =   255
      Left            =   5280
      TabIndex        =   5
      Top             =   1320
      Width           =   735
   End
   Begin VB.Label Label2 
      Caption         =   $"Form1.frx":0000
      Height          =   975
      Left            =   240
      TabIndex        =   4
      Top             =   1200
      Width           =   4935
   End
   Begin VB.Label Label1 
      Caption         =   "DLL���Ƽ�·����"
      Height          =   255
      Left            =   240
      TabIndex        =   2
      Top             =   840
      Width           =   1455
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Command1_Click()
If Text1.Text = "" Then
    MsgBox "������DLL�ļ�����"
    Text1.SetFocus
    Exit Sub
End If
MsgBox "������ϣ�"
End Sub

Private Sub Command2_Click()
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
    If ImeHKLString <> "" Then MSetIMEIntoFirst ImeHKLString    '�����װ�ɹ�����ָ�����뷨�ƶ������뷨�б�ĵ�һ��
    MSetIMEIsDefInput retV   '��ָ�����뷨����ΪϵͳĬ�����뷨��Ӧ�ó���һ�򿪾ͼ���
End If

If Dir(GSystemPath & GImeFileName & ".ime") <> "" Then
' ������뷨����װ��ϵͳĿ¼�����Խ����ʼ��
    IMESetPubString Text1.Text, Val(Text5.Text), Val(Text6.Text), Val(Text2.Text), Val(Text3.Text), Val(Text4.Text)
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
    MsgBox "��Ǹ�����뷨��װʧ�ܡ�������û�й���ԱȨ�ޣ����߸����뷨�����Ѿ���װ��", vbCritical
Else
    MsgBox "���뷨�����ѳɹ���װ��ָ��DLL�ѿ�ʼ����ע�����д��ڳ���" & vbCrLf & "���뷨����ǣ�" & retV, vbExclamation
End If
End Sub

Private Sub Command3_Click()
On Error Resume Next
If Dir(GSystemPath & "imedllhost09.ime") <> "" Then
' ������뷨���ã�ֹͣ����ע��
    IMEClearPubString
End If
End Sub

Private Sub Command4_Click()
Dim retV As Long
MQuitIMEIsDefInput GSysDefIme    '���Ȼ�ԭϵͳĬ�ϵļ��̲���
MActiveIMEForWindows GSysDefIme, 0  '�����ж�����������ΪĬ�����뷨
If ImeHKL <> 0 Then
    retV = MUnLoadIMEByHKL(ImeHKL)
    If retV = 0 Then
        MsgBox "�������뷨ж��ʧ�ܡ����ܸ����뷨����ʹ���ұ�������", vbCritical
    Else
        MsgBox "�������뷨�ѳɹ�ж�ء�", vbExclamation
    End If
Else
    retV = MsgBox("�������뷨���ܲ��Ǳ�������ʱ��װ�ġ��Ƿ�Ҫ����ϵͳ���е����뷨������ж������Ϊ��Windows��׼���뷨��չ���񡱵����뷨��", vbExclamation Or vbYesNo Or vbDefaultButton1)
    If retV = vbYes Then
        retV = MUnLoadIMEByName(GImeFileName)   '����ͨ���ļ���ж�����뷨
        If retV = 0 Then
            MsgBox "�������뷨ж��ʧ�ܡ����ܸ����뷨����ʹ���ұ�������������뷨������", vbCritical
        Else
            MsgBox "�������뷨�ѳɹ�ж�ء�", vbExclamation
        End If
    End If
End If
If ImeHKLString <> "" Then
    MDeleteRegIme ImeHKLString
End If
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
Text1.Text = GPathStr & "hxwdllwx.dll"
End Sub
