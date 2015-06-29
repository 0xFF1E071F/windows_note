Attribute VB_Name = "Module1"
Public Declare Function CopyFile Lib "kernel32" Alias "CopyFileA" (ByVal lpExistingFileName As String, ByVal lpNewFileName As String, ByVal bFailIfExists As Long) As Long
Public Declare Function MoveFile Lib "kernel32" Alias "MoveFileA" (ByVal lpExistingFileName As String, ByVal lpNewFileName As String) As Long
Public Declare Function ImmInstallIME Lib "imm32.dll" Alias "ImmInstallIMEA" (ByVal lpszIMEFileName As String, ByVal lpszLayoutText As String) As Long
Public Declare Function UnloadKeyboardLayout Lib "user32" (ByVal HKL As Long) As Long
Public Declare Function GetKeyboardLayoutList Lib "user32" (ByVal nBuff As Long, lpList As Long) As Long
Public Declare Function ImmIsIME Lib "imm32.dll" (ByVal HKL As Long) As Long
Public Declare Function ImmGetDescription Lib "imm32.dll" Alias "ImmGetDescriptionA" (ByVal HKL As Long, ByVal lpsz As String, ByVal uBufLen As Long) As Long
Public Declare Function ImmGetIMEFileName Lib "imm32.dll" Alias "ImmGetIMEFileNameA" (ByVal HKL As Long, ByVal lpStr As String, ByVal uBufLen As Long) As Long
Public Declare Function LoadKeyboardLayout Lib "user32" Alias "LoadKeyboardLayoutA" (ByVal pwszKLID As String, ByVal flags As Long) As Long
Public Declare Function ActivateKeyboardLayout Lib "user32" (ByVal HKL As Long, ByVal flags As Long) As Long
Public Declare Function GetKeyboardLayout Lib "user32" (ByVal dwLayout As Long) As Long
Public Declare Function GetKeyboardLayoutName Lib "user32" Alias "GetKeyboardLayoutNameA" (ByVal pwszKLID As String) As Long
Public Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function FindWindowEx Lib "user32" Alias "FindWindowExA" (ByVal hWnd1 As Long, ByVal hWnd2 As Long, ByVal lpsz1 As String, ByVal lpsz2 As String) As Long
Public Declare Function SystemParametersInfo Lib "user32" Alias "SystemParametersInfoA" (ByVal uAction As Long, ByVal uParam As Long, lpvParam As Any, ByVal fuWinIni As Long) As Long
Public Declare Function RegisterWindowMessage Lib "user32" Alias "RegisterWindowMessageA" (ByVal lpString As String) As Long
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function RegOpenKey Lib "advapi32.dll" Alias "RegOpenKeyA" (ByVal hKey As Long, ByVal lpSubKey As String, phkResult As Long) As Long
Public Declare Function RegEnumValue Lib "advapi32.dll" Alias "RegEnumValueA" (ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpValueName As String, lpcbValueName As Long, ByVal lpReserved As Long, lpType As Long, lpData As Any, lpcbData As Long) As Long
Public Declare Function RegCloseKey Lib "advapi32.dll" (ByVal hKey As Long) As Long
Public Declare Function RegDeleteValue Lib "advapi32.dll" Alias "RegDeleteValueA" (ByVal hKey As Long, ByVal lpValueName As String) As Long
' -------------------------Windows API-----------------------------------------

Public Declare Function IMESetPubString Lib "imedllhost09.ime" (ByVal RunDLLStr As String, ByVal UnloadDll As Long, ByVal loadNextIme As Long, ByVal DllData1 As Long, ByVal DllData2 As Long, ByVal DllData3 As Long) As Long
Public Declare Function IMEClearPubString Lib "imedllhost09.ime" () As Long
' -----------------------���뷨��չ���񵼳�����--------------------------------

Public Declare Function DLLstartHOOK Lib "hxwdllwx.dll" (ByVal hWnd As Long) As Long   '��ʼ�����뷨HOOK
'Public Declare Function DLLstopHOOK Lib "hxwdllwx.dll" (ByVal a As Long, ByVal b As Long) As Long    '���պ�����
Public Declare Function DLLsetHOOKState Lib "hxwdllwx.dll" (ByVal myState As Boolean) As Long  '�򿪻�رչ���
Public Declare Function DLLGetPubString Lib "hxwdllwx.dll" () As String   '������뷨����
Public Declare Function DLLSetPubString Lib "hxwdllwx.dll" (ByVal tmpStr As String) As Long   '�޸����뷨����
' -----------------------------------------Ŀ��DLL��������-----------------------


Public Const KLF_REORDER = &H8
Public Const WM_INPUTLANGCHANGEREQUEST = &H50
Public Const SPI_SETDEFAULTINPUTLANG = 90
Public Const SPI_GETDEFAULTINPUTLANG = 89
Public Const SPIF_UPDATEINIFILE = &H1
Public Const SPIF_SENDWININICHANGE = &H2
Public Const GWL_WNDPROC = -4
Public Const HKEY_CURRENT_USER = &H80000001
Public Const ERROR_NO_MORE_ITEMS = 259&

Public WM_HXWDLLWX_QQBTX As Long  '�Զ�����Ϣ
Public PrevWndProc As Long '����ɵĴ��ڴ�������ַ
Public GSystemPath As String    'ϵͳ·��
Public GPathStr As String   '��ǰ·��
Public GImeFileName As String   '���뷨�ļ�����������չ��
Public GSysDefIme As Long  'ϵͳĬ�����뷨�ľ��
Public CilentDllPath As String
Public ImeHKL As Long   '���뷨���
Public ImeHKLString As String   '���뷨����ַ���

Public Function SubWndProc(ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Dim tmpS As String
If Msg = WM_HXWDLLWX_QQBTX Then
'����յ������뷨������Ϣ
    tmpS = DLLGetPubString() '������뷨����
    Form1.Text1.Text = Form1.Text1.Text & "���ص����뷨���룺" & tmpS & vbCrLf
    tmpS = tmpS & "�����޸ģ�"
    DLLSetPubString tmpS   '�޸����뷨����
End If
SubWndProc = CallWindowProc(PrevWndProc, hWnd, Msg, wParam, lParam)   '����Ϣ�����ɵĴ��ں�����������
End Function


Public Function StringVC2VB(str As String) As String
StringVC2VB = ""
If Len(str) = 0 Then Exit Function
Dim SPE0 As Long
SPE0 = InStr(str, Chr(0))
If SPE0 Then
    StringVC2VB = Left(str, SPE0 - 1)
Else
    StringVC2VB = str
End If
End Function

Public Function MSetUpIME(ByVal lpFile As String) As Long
' ��װ���뷨���ɹ��򷵻����뷨���������Ϊ���뷨�ļ�����������չ��
'һ��Ҫ���ļ����Ƶ�ϵͳĿ¼�����ҵ�ǰĿ¼Ҫ����һ��ͬ���ļ�������Ҫ�Ƚ�����·���л���ϵͳĿ¼�£��������뷨���ܰ�װ�ɹ�
Dim tmpCDir As String, tmpDirArray() As String
tmpCDir = CurDir()
tmpDirArray = Split(GSystemPath, ":")
If UBound(tmpDirArray) >= 0 Then ChDrive tmpDirArray(0) & ":"
' --------------------
'MoveFile GPathStr & lpFile & ".dll", GPathStr & lpFile & ".ime"  '�����뷨�ļ�����������չ��������IME
'CopyFile GPathStr & lpFile & ".ime", GSystemPath & lpFile & ".ime", False   '�����뷨�ļ����Ƶ�ϵͳĿ¼
CopyFile GPathStr & lpFile & ".dll", GSystemPath & lpFile & ".ime", False   '�����뷨�ļ����Ƶ�ϵͳĿ¼
ChDir GSystemPath  '��װǰһ��Ҫ������Ŀ¼�л���ϵͳĿ¼�£������޷���װ�ɹ�
MSetUpIME = ImmInstallIME(lpFile & ".ime", "Windows��׼���뷨��չ���� v1.0")
'MoveFile GPathStr & lpFile & ".ime", GPathStr & lpFile & ".dll"  '��װ��ɺ��ٽ����ָĻ���
' --------------------
tmpDirArray = Split(tmpCDir, ":")
If UBound(tmpDirArray) >= 0 Then ChDrive tmpDirArray(0) & ":"
ChDir tmpCDir    '��װ��ɺ��ٽ�����Ŀ¼�л�����
End Function

Public Function MGetIMEHwndString(ByVal IMEhwnd As Long) As String
' �������뷨����ַ���
Dim tempIM As Long, retV As Long, HKLName As String
MGetIMEHwndString = ""
tempIM = GetKeyboardLayout(0)
'ActivateKeyboardLayout IMEhwnd, KLF_REORDER    'ָ�����뷨����Ϊ�б��һ��
ActivateKeyboardLayout IMEhwnd, 0
HKLName = String(8, 0)
retV = GetKeyboardLayoutName(HKLName)
ActivateKeyboardLayout tempIM, 0
If HKLName <> String(8, 0) Then MGetIMEHwndString = HKLName
End Function

Public Sub MSetIMEIntoFirst(ByVal HKLName As String)
' ��ָ����������뷨����Ϊ���뷨�б�ĵ�һ��
retV = LoadKeyboardLayout(HKLName, KLF_REORDER)  'ָ�����뷨����Ϊ�б��һ��
End Sub

Public Sub MSetIMEIsDefInput(ByVal IMEhwnd As Long)
' ����ָ����������뷨ΪϵͳĬ�����뷨��һ�����ͼ���
Dim retV As Long
retV = SystemParametersInfo(SPI_SETDEFAULTINPUTLANG, 0, IMEhwnd, SPIF_SENDWININICHANGE)
End Sub

Public Sub MQuitIMEIsDefInput(ByVal IMEhwnd As Long)
'��ԭϵͳĬ�����뷨�����ڳ����˳�ʱ��ԭ��Ĭ�ϼ��̲��ֵĸı�
Dim retV As Long
SystemParametersInfo SPI_GETDEFAULTINPUTLANG, 0, retV, 0 '�õ�ϵͳĬ�ϵ����뷨�ľ��
If retV <> IMEhwnd Then MSetIMEIsDefInput IMEhwnd
End Sub

Public Function MUnLoadIMEByHKL(ByVal IMEhwnd As Long) As Long
'ͨ�����뷨���ж�����뷨
MUnLoadIMEByHKL = UnloadKeyboardLayout(IMEhwnd)
End Function

Public Function MUnLoadIMEByName(ByVal lpFile As String, Optional ByVal lpText As String = "Windows��׼���뷨��չ����") As Long
'ͨ�����뷨�ļ�����ж�����뷨
Dim retV As Long, hIme As Long
hIme = MImeFindByName(lpFile, lpText)
If hIme <> 0 Then
    retV = UnloadKeyboardLayout(hIme)
End If
MUnLoadIMEByName = retV
End Function

Public Function MImeFindByName(ByVal lpFile As String, Optional ByVal lpText As String = "Windows��׼���뷨��չ����") As Long
'ͨ���ļ����Ʋ����Ѱ�װ�����뷨�����lpFile=�ļ�����lpText=���뷨����
Dim IMEhKB() As Long, IMEBuffLen As Long, i As Long, retV As Long
Dim IMEBuff As String, iCount As Long
Dim IMERetStr As String
Dim IMERetCount As Long
MImeFindByName = 0
iCount = GetKeyboardLayoutList(0, ByVal 0) 'ȡ�����뷨����
If iCount < 1 Then Exit Function
ReDim IMEhKB(iCount - 1)
retV = 0
iCount = GetKeyboardLayoutList(UBound(IMEhKB) + 1, IMEhKB(0)) 'ȡ���������뷨
For i = 1 To iCount
    If ImmIsIME(IMEhKB(i - 1)) = 1 Then '������������뷨
        IMEBuffLen = 255
        IMEBuff = String(255, 0)
        IMERetCount = ImmGetDescription(IMEhKB(i - 1), IMEBuff, IMEBuffLen)
        IMERetStr = Left(IMEBuff, IMERetCount)
        IMERetStr = StringVC2VB(IMERetStr)
        If IMERetStr = lpText Then
            ' �ж����뷨�ļ�
            IMEBuffLen = 255
            IMEBuff = String(255, 0)
            IMERetCount = ImmGetIMEFileName(IMEhKB(i - 1), IMEBuff, IMEBuffLen)
            IMERetStr = Left(IMEBuff, IMERetCount)
            IMERetStr = StringVC2VB(IMERetStr)
            If UCase(IMERetStr) = UCase(lpFile & ".ime") Then
                retV = IMEhKB(i - 1)
                Exit For
            End If
        End If
    End If
Next
MImeFindByName = retV
End Function


Sub MActiveIMEForWindows(ByVal IMEhwnd As Long, mType As Byte)
' �������Ѿ����еĳ����м���ָ�����뷨��IMEhwnd=���뷨�����mType=����ǿ�ȣ�0-ֻ���Զ������ڣ�1-���������Ӵ���
If mType = 0 Then
    PostMSG2TopWindow IMEhwnd
Else
    PostMSG2AllWindow IMEhwnd
End If
End Sub

Sub PostMSG2TopWindow(ByVal IMEhwnd As Long)
' �����еĶ��������м���ָ����������뷨
Dim mHwnd As Long
mHwnd = 0
Do
    mHwnd = FindWindowEx(0, mHwnd, vbNullString, vbNullString)    '�������涥������
    If mHwnd <> 0 Then
        PostMessage mHwnd, WM_INPUTLANGCHANGEREQUEST, &H1, IMEhwnd   '��Ŀ�괰���м���ָ�����뷨
    End If
Loop Until mHwnd = 0
End Sub

Sub PostMSG2AllWindow(ByVal IMEhwnd As Long)
' �����еĶ������ں����¼������м���ָ����������뷨
' ����ָ������mHwnd�������Ӵ���,�������м���ָ�����뷨
Dim theLPS() As Long, tmpHwnd As Long, mHwnd As Long
mHwnd = 0 '�������Ķ������ھ����0��������
ReDim theLPS(1)
theLPS(1) = mHwnd
tmpHwnd = 0
Do   '������ѭ��
    tmpHwnd = FindWindowEx(theLPS(UBound(theLPS)), tmpHwnd, vbNullString, vbNullString)
    If tmpHwnd <> 0 Then
        PostMessage tmpHwnd, WM_INPUTLANGCHANGEREQUEST, &H1, IMEhwnd   '������Ϣ���������뷨
        ReDim Preserve theLPS(UBound(theLPS) + 1)
        theLPS(UBound(theLPS)) = tmpHwnd
        tmpHwnd = 0
    Else
        tmpHwnd = theLPS(UBound(theLPS))
        ReDim Preserve theLPS(UBound(theLPS) - 1)
    End If
Loop Until UBound(theLPS) < 1
End Sub

Sub MDeleteRegIme(ByVal ImeReg As String)
' ɾ��ע����и����뷨�������
Dim hKey As Long, sName As String, sData As String, sRet As Long, RetData As Long, sCnt As Long
If RegOpenKey(HKEY_CURRENT_USER, "Keyboard Layout\Preload", hKey) = 0 Then
    sName = Space(255)
    sData = Space(255)
    sRet = 255
    RetData = 255
    Do While RegEnumValue(hKey, sCnt, sName, sRet, 0, ByVal 0&, ByVal sData, RetData) <> ERROR_NO_MORE_ITEMS
        If RetData > 0 Then
            If UCase(Left(sData, RetData - 1)) = UCase(ImeReg) Then
                RegDeleteValue hKey, Left(sName, sRet)    '�Ƴ����뷨��Ӧ��ע���ֵ
                Exit Do
            End If
            sCnt = sCnt + 1
            sName = Space(255)
            sData = Space(255)
            sRet = 255
            RetData = 255
        End If
    Loop
    RegCloseKey hKey
End If
End Sub
