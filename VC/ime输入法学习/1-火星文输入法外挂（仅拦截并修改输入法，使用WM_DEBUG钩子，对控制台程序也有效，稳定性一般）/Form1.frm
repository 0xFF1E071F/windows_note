VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "������ת�����������Ĺ����棩 V2.0"
   ClientHeight    =   6375
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   9975
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6375
   ScaleWidth      =   9975
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command26 
      Caption         =   "����������"
      Height          =   375
      Left            =   360
      TabIndex        =   132
      Top             =   5880
      Width           =   1335
   End
   Begin VB.TextBox Text20 
      Appearance      =   0  'Flat
      BackColor       =   &H8000000F&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "����"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   180
      Left            =   4440
      Locked          =   -1  'True
      MouseIcon       =   "Form1.frx":31BA
      MousePointer    =   99  'Custom
      TabIndex        =   131
      Text            =   "http://www.huoxingyu.com"
      Top             =   5880
      Width           =   2295
   End
   Begin VB.TextBox Text19 
      Appearance      =   0  'Flat
      BackColor       =   &H8000000F&
      BorderStyle     =   0  'None
      ForeColor       =   &H00FF0000&
      Height          =   180
      Left            =   2520
      Locked          =   -1  'True
      MouseIcon       =   "Form1.frx":34C4
      MousePointer    =   99  'Custom
      TabIndex        =   130
      Text            =   "�����Ĺٷ���վ��Ʒ��"
      Top             =   5880
      Width           =   1815
   End
   Begin VB.CommandButton Command25 
      Caption         =   "�鿴����"
      Height          =   375
      Left            =   8160
      TabIndex        =   129
      Top             =   5880
      Width           =   1335
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Left            =   7560
      Top             =   5880
   End
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   6840
      Top             =   5880
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   3
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":37CE
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":B520
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":13272
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   5055
      Index           =   0
      Left            =   240
      TabIndex        =   1
      Top             =   480
      Width           =   9495
      Begin VB.CommandButton Command2 
         Caption         =   "���"
         Height          =   255
         Index           =   2
         Left            =   2760
         TabIndex        =   87
         Top             =   2160
         Width           =   855
      End
      Begin VB.Frame Frame3 
         Caption         =   "����ת�����Ĳ�������"
         Height          =   615
         Left            =   120
         TabIndex        =   18
         Top             =   2640
         Width           =   9135
         Begin VB.OptionButton Option2 
            Caption         =   "�û��Զ���(�߼�)"
            Height          =   255
            Index           =   3
            Left            =   6960
            TabIndex        =   23
            Top             =   240
            Width           =   1815
         End
         Begin VB.OptionButton Option2 
            Caption         =   "����ת��"
            Height          =   255
            Index           =   2
            Left            =   5280
            TabIndex        =   22
            Top             =   240
            Width           =   1095
         End
         Begin VB.OptionButton Option2 
            Caption         =   "ȫ��ת��(�ʻ�����)"
            Height          =   255
            Index           =   1
            Left            =   2760
            TabIndex        =   21
            Top             =   240
            Width           =   1935
         End
         Begin VB.OptionButton Option2 
            Caption         =   "ȫ��ת��(��������)"
            Height          =   255
            Index           =   0
            Left            =   240
            TabIndex        =   20
            Top             =   240
            Width           =   1935
         End
      End
      Begin VB.Frame Frame4 
         Caption         =   "�û��Զ������"
         Height          =   1455
         Left            =   120
         TabIndex        =   24
         Top             =   3480
         Width           =   9135
         Begin VB.Frame Frame5 
            BorderStyle     =   0  'None
            Caption         =   "Frame5"
            Height          =   375
            Index           =   3
            Left            =   2640
            TabIndex        =   31
            Top             =   960
            Width           =   3015
            Begin VB.OptionButton Option6 
               Caption         =   "ʹ����ͬת��"
               Height          =   180
               Index           =   1
               Left            =   0
               TabIndex        =   43
               Top             =   120
               Width           =   1455
            End
            Begin VB.OptionButton Option6 
               Caption         =   "���Բ���ͬ"
               Height          =   180
               Index           =   0
               Left            =   1680
               TabIndex        =   42
               Top             =   120
               Width           =   1335
            End
         End
         Begin VB.Frame Frame5 
            BorderStyle     =   0  'None
            Caption         =   "Frame5"
            Height          =   375
            Index           =   2
            Left            =   2640
            TabIndex        =   30
            Top             =   600
            Width           =   4455
            Begin VB.OptionButton Option5 
               Caption         =   "��,�еȺ�����"
               Height          =   255
               Index           =   2
               Left            =   2400
               TabIndex        =   40
               Top             =   120
               Width           =   1815
            End
            Begin VB.OptionButton Option5 
               Caption         =   "�򵥺��е�"
               Height          =   255
               Index           =   1
               Left            =   960
               TabIndex        =   39
               Top             =   120
               Width           =   1335
            End
            Begin VB.OptionButton Option5 
               Caption         =   "��"
               Height          =   255
               Index           =   0
               Left            =   0
               TabIndex        =   38
               Top             =   120
               Width           =   855
            End
         End
         Begin VB.Frame Frame5 
            BorderStyle     =   0  'None
            Caption         =   "Frame5"
            Height          =   375
            Index           =   1
            Left            =   7320
            TabIndex        =   29
            Top             =   240
            Width           =   1095
            Begin VB.OptionButton Option4 
               Caption         =   "��"
               Height          =   255
               Index           =   0
               Left            =   600
               TabIndex        =   36
               Top             =   120
               Width           =   495
            End
            Begin VB.OptionButton Option4 
               Caption         =   "��"
               Height          =   255
               Index           =   1
               Left            =   0
               TabIndex        =   35
               Top             =   120
               Width           =   615
            End
         End
         Begin VB.Frame Frame5 
            BorderStyle     =   0  'None
            Caption         =   "Frame5"
            Height          =   375
            Index           =   0
            Left            =   4080
            TabIndex        =   28
            Top             =   240
            Width           =   1335
            Begin VB.OptionButton Option3 
               Caption         =   "��"
               Height          =   255
               Index           =   0
               Left            =   600
               TabIndex        =   33
               Top             =   120
               Width           =   495
            End
            Begin VB.OptionButton Option3 
               Caption         =   "��"
               Height          =   255
               Index           =   1
               Left            =   0
               TabIndex        =   32
               Top             =   120
               Width           =   495
            End
         End
         Begin VB.TextBox Text3 
            Appearance      =   0  'Flat
            Height          =   270
            Left            =   1200
            TabIndex        =   26
            ToolTipText     =   "ת��Ƶ����ָ����������ж����ַ�����ת��������0.5��ʾ��50%���ַ�����ת��,1��ʾȫ������ת����"
            Top             =   360
            Width           =   495
         End
         Begin VB.Label Label5 
            Caption         =   "��ͬ�ַ��Ƿ�ʹ����ͬת����"
            Height          =   255
            Left            =   240
            TabIndex        =   41
            ToolTipText     =   "����һ�����ֿ����ж��ֱ����֣����ѡ����������������������ظ��ĺ���ʱ���Ƿ������ת������ͬ�ı����֡�"
            Top             =   1080
            Width           =   2415
         End
         Begin VB.Label Label4 
            Caption         =   "ת����Ļ�����ʶ���Ѷȣ�"
            Height          =   255
            Left            =   240
            TabIndex        =   37
            ToolTipText     =   "����ת����ı������Ƿ����ױ��ϡ�����һ�����ֿ����ж��ֱ����֣����ѡ�����ϵͳʹ���ѱ��ϻ������ױ��ϵ��ַ�����ת����"
            Top             =   720
            Width           =   2175
         End
         Begin VB.Label Label3 
            Caption         =   "�Ƿ��������ȣ�"
            Height          =   255
            Left            =   5760
            TabIndex        =   34
            ToolTipText     =   "��ת��Ƶ��С��1������£�ʹ�ó��������Ⱥ�ϵͳ������ת�����õĺ��֣�����Ƨ����ᱣ��ԭ����"
            Top             =   360
            Width           =   1455
         End
         Begin VB.Label Label2 
            Caption         =   "�Ƿ�ʹ�ôʻ����ȣ�"
            Height          =   255
            Left            =   2400
            TabIndex        =   27
            ToolTipText     =   "ʹ�ôʻ����Ⱥ�ϵͳ�ᰴ��һ�̶ֹ���ʽ����ת���ʻ�Ͷ��ʹת��������и��õķ��"
            Top             =   360
            Width           =   1695
         End
         Begin VB.Label Label1 
            Caption         =   "ת��Ƶ�ʣ�"
            Height          =   255
            Left            =   240
            TabIndex        =   25
            Top             =   360
            Width           =   975
         End
      End
      Begin VB.CheckBox Check2 
         Caption         =   "ǩ������"
         Height          =   255
         Left            =   4200
         TabIndex        =   19
         Top             =   2160
         Width           =   1095
      End
      Begin VB.CommandButton Command2 
         Caption         =   "����"
         Height          =   255
         Index           =   1
         Left            =   5880
         TabIndex        =   17
         Top             =   2160
         Width           =   975
      End
      Begin VB.CommandButton Command2 
         Caption         =   "ճ��"
         Height          =   255
         Index           =   0
         Left            =   1800
         TabIndex        =   16
         Top             =   2160
         Width           =   855
      End
      Begin VB.CheckBox Check1 
         Caption         =   "�����Զ�����"
         Height          =   180
         Index           =   1
         Left            =   7920
         TabIndex        =   15
         Top             =   2160
         Width           =   1455
      End
      Begin VB.CheckBox Check1 
         Caption         =   "����ʵʱת��"
         Height          =   180
         Index           =   0
         Left            =   120
         TabIndex        =   14
         Top             =   2160
         Width           =   1575
      End
      Begin VB.CommandButton Command1 
         Caption         =   "��ʼת��>>"
         Height          =   375
         Left            =   3960
         TabIndex        =   13
         Top             =   1560
         Width           =   1575
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         ItemData        =   "Form1.frx":1AFC4
         Left            =   3960
         List            =   "Form1.frx":1AFD4
         Style           =   2  'Dropdown List
         TabIndex        =   12
         Top             =   1080
         Width           =   1575
      End
      Begin VB.Frame Frame2 
         Caption         =   "����ģʽ"
         Height          =   855
         Left            =   3720
         TabIndex        =   9
         Top             =   120
         Width           =   2055
         Begin VB.OptionButton Option1 
            Caption         =   "���뷨���ģʽ"
            ForeColor       =   &H000000FF&
            Height          =   255
            Index           =   1
            Left            =   240
            TabIndex        =   11
            ToolTipText     =   "�ر��Ƴ������뷨���ģʽ��ѡ�д����������ʹ��ʲô���뷨�������Զ�������ת��Ϊ�����ģ�"
            Top             =   480
            Width           =   1575
         End
         Begin VB.OptionButton Option1 
            Caption         =   "ת����ģʽ"
            Height          =   255
            Index           =   0
            Left            =   240
            TabIndex        =   10
            ToolTipText     =   "���¿�ݼ�Ctrl+F12��������ת����ģʽ�����ģʽ֮���л���"
            Top             =   240
            Width           =   1215
         End
      End
      Begin VB.TextBox Text2 
         Height          =   1935
         Left            =   5880
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   8
         Top             =   120
         Width           =   3495
      End
      Begin VB.TextBox Text1 
         Height          =   1935
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   7
         Text            =   "Form1.frx":1B00A
         Top             =   120
         Width           =   3495
      End
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   5055
      Index           =   5
      Left            =   240
      TabIndex        =   6
      Top             =   480
      Width           =   9495
      Begin VB.CheckBox Check5 
         Caption         =   "ת��Ϊ������"
         Height          =   255
         Left            =   960
         TabIndex        =   128
         ToolTipText     =   "���������������ת����ת����ת���������ȡ����ת���������á�"
         Top             =   480
         Width           =   1455
      End
      Begin VB.CommandButton Command24 
         Caption         =   "�ʴ����ݿ�༭"
         Height          =   375
         Left            =   7680
         TabIndex        =   127
         Top             =   240
         Width           =   1455
      End
      Begin VB.CommandButton Command22 
         Caption         =   "��������"
         Height          =   375
         Left            =   6240
         TabIndex        =   126
         Top             =   240
         Width           =   1095
      End
      Begin VB.CheckBox Check4 
         Caption         =   "�Զ������ȼ�����QQ��Ϣ"
         Enabled         =   0   'False
         Height          =   255
         Left            =   3240
         TabIndex        =   125
         ToolTipText     =   "ѡ�д���󣬳����ÿ�������Զ������µ�QQ��Ϣ��"
         Top             =   120
         Width           =   2775
      End
      Begin VB.CheckBox Check3 
         Caption         =   "����QQ�Զ�����"
         Height          =   255
         Left            =   960
         TabIndex        =   124
         ToolTipText     =   "ѡ�д���󣬳����Զ��ظ�����QQ��Ϣ�������Ա༭�ʴ����ݿ����޸Ļظ����ݡ�"
         Top             =   120
         Width           =   1935
      End
      Begin VB.CommandButton Command11 
         Caption         =   "����"
         Default         =   -1  'True
         Height          =   375
         Left            =   8520
         TabIndex        =   74
         Top             =   4560
         Width           =   735
      End
      Begin VB.TextBox Text11 
         Appearance      =   0  'Flat
         Height          =   375
         Left            =   840
         TabIndex        =   73
         Top             =   4560
         Width           =   7455
      End
      Begin VB.TextBox Text10 
         Appearance      =   0  'Flat
         ForeColor       =   &H00004000&
         Height          =   375
         Left            =   840
         Locked          =   -1  'True
         TabIndex        =   72
         Top             =   3960
         Width           =   8415
      End
      Begin VB.TextBox Text9 
         BackColor       =   &H00E0E0E0&
         ForeColor       =   &H00FF0000&
         Height          =   2895
         Left            =   360
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   71
         Top             =   840
         Width           =   8775
      End
      Begin VB.Label Label20 
         Caption         =   " ��˵��"
         Height          =   255
         Left            =   120
         TabIndex        =   123
         Top             =   4680
         Width           =   735
      End
      Begin VB.Label Label19 
         Caption         =   "����˵��"
         Height          =   255
         Left            =   120
         TabIndex        =   122
         Top             =   4080
         Width           =   735
      End
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   5055
      Index           =   4
      Left            =   240
      TabIndex        =   5
      Top             =   480
      Width           =   9495
      Begin VB.CommandButton Command23 
         Caption         =   "����"
         Height          =   255
         Left            =   2040
         TabIndex        =   121
         Top             =   3840
         Width           =   975
      End
      Begin VB.TextBox Text18 
         Appearance      =   0  'Flat
         Height          =   270
         Left            =   6360
         TabIndex        =   119
         Text            =   "1"
         Top             =   2400
         Width           =   735
      End
      Begin VB.CommandButton Command17 
         Caption         =   "ת��"
         Height          =   255
         Index           =   4
         Left            =   5040
         TabIndex        =   117
         Top             =   2400
         Width           =   855
      End
      Begin VB.TextBox Text17 
         Height          =   270
         Left            =   5760
         Locked          =   -1  'True
         TabIndex        =   115
         Top             =   960
         Width           =   3375
      End
      Begin VB.Frame Frame13 
         Caption         =   "�½�����"
         Height          =   735
         Left            =   3840
         TabIndex        =   108
         Top             =   4200
         Width           =   5295
         Begin VB.CommandButton Command21 
            Caption         =   "�½�"
            Height          =   255
            Left            =   4320
            TabIndex        =   113
            Top             =   360
            Width           =   735
         End
         Begin VB.ComboBox Combo5 
            Height          =   300
            ItemData        =   "Form1.frx":1B022
            Left            =   2760
            List            =   "Form1.frx":1B02F
            Style           =   2  'Dropdown List
            TabIndex        =   112
            Top             =   360
            Width           =   1335
         End
         Begin VB.TextBox Text16 
            Height          =   270
            Left            =   840
            TabIndex        =   110
            Top             =   360
            Width           =   975
         End
         Begin VB.Label Label14 
            Caption         =   "�����ڣ�"
            Height          =   255
            Left            =   2040
            TabIndex        =   111
            Top             =   360
            Width           =   735
         End
         Begin VB.Label Label13 
            Caption         =   "��������"
            Height          =   255
            Left            =   120
            TabIndex        =   109
            Top             =   360
            Width           =   735
         End
      End
      Begin VB.Frame Frame12 
         Caption         =   "ɾ������"
         Height          =   735
         Left            =   240
         TabIndex        =   105
         Top             =   4200
         Width           =   3375
         Begin VB.CommandButton Command20 
            Caption         =   "ɾ��"
            Height          =   255
            Left            =   2400
            TabIndex        =   107
            Top             =   360
            Width           =   735
         End
         Begin VB.ComboBox Combo4 
            Height          =   300
            Left            =   120
            Style           =   2  'Dropdown List
            TabIndex        =   106
            Top             =   360
            Width           =   1935
         End
      End
      Begin VB.CommandButton Command19 
         Caption         =   "�������"
         Enabled         =   0   'False
         Height          =   375
         Index           =   1
         Left            =   7560
         TabIndex        =   104
         Top             =   3360
         Width           =   1575
      End
      Begin VB.CommandButton Command19 
         Caption         =   "���"
         Height          =   375
         Index           =   0
         Left            =   5040
         TabIndex        =   103
         Top             =   3360
         Width           =   1815
      End
      Begin VB.CommandButton Command18 
         Caption         =   "ɾ��"
         Height          =   375
         Index           =   2
         Left            =   8040
         TabIndex        =   102
         Top             =   2760
         Width           =   1095
      End
      Begin VB.CommandButton Command18 
         Caption         =   "����"
         Enabled         =   0   'False
         Height          =   375
         Index           =   1
         Left            =   6600
         TabIndex        =   101
         Top             =   2760
         Width           =   1095
      End
      Begin VB.CommandButton Command18 
         Caption         =   "�༭"
         Height          =   375
         Index           =   0
         Left            =   5040
         TabIndex        =   100
         Top             =   2760
         Width           =   1095
      End
      Begin VB.CommandButton Command17 
         Caption         =   "βҳ"
         Height          =   375
         Index           =   3
         Left            =   8280
         TabIndex        =   99
         Top             =   1800
         Width           =   855
      End
      Begin VB.CommandButton Command17 
         Caption         =   "��һҳ"
         Height          =   375
         Index           =   2
         Left            =   7200
         TabIndex        =   98
         Top             =   1800
         Width           =   855
      End
      Begin VB.CommandButton Command17 
         Caption         =   "��һҳ"
         Enabled         =   0   'False
         Height          =   375
         Index           =   1
         Left            =   6120
         TabIndex        =   97
         Top             =   1800
         Width           =   855
      End
      Begin VB.CommandButton Command17 
         Caption         =   "��ҳ"
         Height          =   375
         Index           =   0
         Left            =   5040
         TabIndex        =   96
         Top             =   1800
         Width           =   855
      End
      Begin VB.Frame Frame11 
         Caption         =   "ѡ�����"
         Height          =   615
         Left            =   240
         TabIndex        =   88
         Top             =   120
         Width           =   8895
         Begin VB.CommandButton Command16 
            Caption         =   "ˢ��"
            Height          =   255
            Left            =   7680
            TabIndex        =   95
            Top             =   240
            Width           =   855
         End
         Begin VB.TextBox Text15 
            Height          =   270
            Left            =   5520
            TabIndex        =   93
            ToolTipText     =   "�ڴ�����ؼ��ʣ������ˢ�¡���ť���������������˹ؼ��ʵķ�����������ͼ��"
            Top             =   240
            Width           =   1695
         End
         Begin VB.ComboBox Combo3 
            Height          =   300
            ItemData        =   "Form1.frx":1B045
            Left            =   2760
            List            =   "Form1.frx":1B047
            Style           =   2  'Dropdown List
            TabIndex        =   92
            Top             =   240
            Width           =   1575
         End
         Begin VB.OptionButton Option10 
            Caption         =   "����"
            Height          =   255
            Index           =   2
            Left            =   1680
            TabIndex        =   91
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton Option10 
            Caption         =   "����"
            Height          =   255
            Index           =   1
            Left            =   960
            TabIndex        =   90
            Top             =   240
            Width           =   735
         End
         Begin VB.OptionButton Option10 
            Caption         =   "ͼ��"
            Height          =   255
            Index           =   0
            Left            =   240
            TabIndex        =   89
            Top             =   240
            Value           =   -1  'True
            Width           =   735
         End
         Begin VB.Label Label12 
            Caption         =   "�ؼ��ʣ�"
            Height          =   255
            Left            =   4800
            TabIndex        =   94
            Top             =   240
            Width           =   735
         End
      End
      Begin VB.TextBox Text12 
         Height          =   2775
         Left            =   240
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Both
         TabIndex        =   75
         Top             =   960
         Width           =   4575
      End
      Begin VB.Label Label18 
         Caption         =   "ҳ"
         Height          =   255
         Left            =   7200
         TabIndex        =   120
         Top             =   2400
         Width           =   495
      End
      Begin VB.Label Label17 
         Caption         =   "��"
         Height          =   255
         Left            =   6120
         TabIndex        =   118
         Top             =   2400
         Width           =   255
      End
      Begin VB.Label Label16 
         Caption         =   "�ܹ�0ҳ����ǰ��0ҳ"
         Height          =   255
         Left            =   5160
         TabIndex        =   116
         Top             =   1440
         Width           =   3975
      End
      Begin VB.Label Label15 
         Caption         =   "˵����"
         Height          =   255
         Left            =   5160
         TabIndex        =   114
         Top             =   960
         Width           =   615
      End
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   5055
      Index           =   3
      Left            =   240
      TabIndex        =   4
      Top             =   480
      Width           =   9495
      Begin VB.CommandButton Command15 
         Caption         =   "����"
         Height          =   255
         Left            =   5880
         TabIndex        =   86
         Top             =   3240
         Width           =   855
      End
      Begin VB.CommandButton Command14 
         Caption         =   "���"
         Height          =   255
         Left            =   1200
         TabIndex        =   85
         Top             =   3240
         Width           =   855
      End
      Begin VB.CommandButton Command13 
         Caption         =   "ճ��"
         Height          =   255
         Left            =   120
         TabIndex        =   84
         Top             =   3240
         Width           =   855
      End
      Begin VB.Frame Frame10 
         Caption         =   "���ں���תƴ������"
         Height          =   1095
         Left            =   120
         TabIndex        =   82
         Top             =   3720
         Width           =   9255
         Begin VB.Label Label11 
            Caption         =   $"Form1.frx":1B049
            Height          =   375
            Left            =   240
            TabIndex        =   83
            Top             =   360
            Width           =   8775
         End
      End
      Begin VB.CommandButton Command12 
         Caption         =   "��ʼת��>>"
         Height          =   495
         Left            =   3960
         TabIndex        =   81
         Top             =   2160
         Width           =   1695
      End
      Begin VB.Frame Frame9 
         Caption         =   "ת��ѡ��"
         Height          =   1095
         Left            =   3840
         TabIndex        =   78
         Top             =   720
         Width           =   1935
         Begin VB.OptionButton Option9 
            Caption         =   "����ת��Ϊƴ��"
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   80
            Top             =   720
            Value           =   -1  'True
            Width           =   1575
         End
         Begin VB.OptionButton Option9 
            Caption         =   "���㺺�ֱʻ���"
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   79
            Top             =   360
            Width           =   1575
         End
      End
      Begin VB.TextBox Text14 
         Height          =   2655
         Left            =   5880
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   77
         Top             =   480
         Width           =   3495
      End
      Begin VB.TextBox Text13 
         Height          =   2655
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   76
         Text            =   "Form1.frx":1B107
         Top             =   480
         Width           =   3495
      End
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   5055
      Index           =   2
      Left            =   240
      TabIndex        =   3
      Top             =   480
      Width           =   9495
      Begin VB.CommandButton Command10 
         Caption         =   "����"
         Height          =   255
         Left            =   8400
         TabIndex        =   69
         Top             =   2640
         Width           =   975
      End
      Begin VB.CommandButton Command9 
         Caption         =   "���"
         Height          =   255
         Left            =   1320
         TabIndex        =   68
         Top             =   2640
         Width           =   975
      End
      Begin VB.CommandButton Command8 
         Caption         =   "ճ��"
         Height          =   255
         Left            =   120
         TabIndex        =   67
         Top             =   2640
         Width           =   975
      End
      Begin VB.CommandButton Command7 
         Caption         =   "��ʼת��>>"
         Height          =   495
         Left            =   4200
         TabIndex        =   66
         Top             =   1320
         Width           =   1215
      End
      Begin VB.Frame Frame8 
         Caption         =   "ǩ����������ѡ��"
         Height          =   1815
         Left            =   360
         TabIndex        =   59
         Top             =   3120
         Width           =   8895
         Begin VB.ComboBox Combo2 
            Height          =   300
            Left            =   4200
            Style           =   2  'Dropdown List
            TabIndex        =   65
            Top             =   840
            Width           =   1935
         End
         Begin VB.TextBox Text8 
            Appearance      =   0  'Flat
            Height          =   270
            Left            =   4800
            TabIndex        =   63
            Top             =   360
            Width           =   495
         End
         Begin VB.OptionButton Option8 
            Caption         =   "�̶�"
            Height          =   255
            Index           =   1
            Left            =   3240
            TabIndex        =   61
            Top             =   840
            Width           =   855
         End
         Begin VB.OptionButton Option8 
            Caption         =   "���"
            Height          =   255
            Index           =   0
            Left            =   3240
            TabIndex        =   60
            Top             =   360
            Width           =   735
         End
         Begin VB.Label Label10 
            Caption         =   "ǩ���������߿���Ϊ����ǩ�����װ���Է��ţ�ÿ��ת��������̶���"
            Height          =   255
            Left            =   1920
            TabIndex        =   70
            Top             =   1440
            Width           =   6015
         End
         Begin VB.Label Label9 
            Caption         =   "%"
            Height          =   255
            Left            =   5400
            TabIndex        =   64
            Top             =   360
            Width           =   135
         End
         Begin VB.Label Label8 
            Caption         =   "�ܶȣ�"
            Height          =   255
            Left            =   4200
            TabIndex        =   62
            Top             =   360
            Width           =   615
         End
      End
      Begin VB.TextBox Text7 
         Height          =   2175
         Left            =   5520
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   58
         Top             =   360
         Width           =   3855
      End
      Begin VB.TextBox Text6 
         Height          =   2175
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   57
         Text            =   "Form1.frx":1B120
         Top             =   360
         Width           =   3855
      End
   End
   Begin VB.Frame Frame1 
      BorderStyle     =   0  'None
      Caption         =   "Frame1"
      Height          =   5055
      Index           =   1
      Left            =   240
      TabIndex        =   2
      Top             =   480
      Width           =   9495
      Begin VB.Frame Frame7 
         Caption         =   "���ڻ����ķ���"
         Height          =   1335
         Left            =   240
         TabIndex        =   54
         Top             =   3480
         Width           =   9015
         Begin VB.Label Label7 
            Caption         =   "ע�⣺���ڻ����ĵĸ����ԣ�����һ�ʶ�������󣩣����ǲ��ܱ�֤�����׼ȷ�ԣ���˷����������ο���������ݾ����ﾳ��ϸ��ᡣ"
            Height          =   375
            Left            =   240
            TabIndex        =   56
            Top             =   840
            Width           =   8655
         End
         Begin VB.Label Label6 
            Caption         =   $"Form1.frx":1B139
            Height          =   615
            Left            =   240
            TabIndex        =   55
            Top             =   240
            Width           =   8535
         End
      End
      Begin VB.Frame Frame6 
         Caption         =   "����ѡ��"
         Height          =   615
         Left            =   240
         TabIndex        =   50
         Top             =   2640
         Width           =   9015
         Begin VB.OptionButton Option7 
            Caption         =   "���ʻ�"
            Height          =   255
            Index           =   2
            Left            =   5040
            TabIndex        =   53
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton Option7 
            Caption         =   "��������"
            Height          =   255
            Index           =   1
            Left            =   3420
            TabIndex        =   52
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton Option7 
            Caption         =   "�ʻ�����"
            Height          =   255
            Index           =   0
            Left            =   1800
            TabIndex        =   51
            Top             =   240
            Value           =   -1  'True
            Width           =   1215
         End
      End
      Begin VB.CommandButton Command6 
         Caption         =   "����"
         Height          =   255
         Left            =   8160
         TabIndex        =   49
         Top             =   2160
         Width           =   1215
      End
      Begin VB.CommandButton Command5 
         Caption         =   "���"
         Height          =   255
         Left            =   1440
         TabIndex        =   48
         Top             =   2160
         Width           =   1215
      End
      Begin VB.CommandButton Command4 
         Caption         =   "ճ��"
         Height          =   255
         Left            =   120
         TabIndex        =   47
         Top             =   2160
         Width           =   1215
      End
      Begin VB.CommandButton Command3 
         Caption         =   "��ʼ����>>"
         Height          =   495
         Left            =   4080
         TabIndex        =   46
         Top             =   960
         Width           =   1455
      End
      Begin VB.TextBox Text5 
         Height          =   1815
         Left            =   5760
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   45
         Top             =   240
         Width           =   3615
      End
      Begin VB.TextBox Text4 
         Height          =   1815
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   44
         Text            =   "Form1.frx":1B23E
         Top             =   240
         Width           =   3615
      End
   End
   Begin MSComctlLib.TabStrip TabStrip1 
      Height          =   5535
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   9735
      _ExtentX        =   17171
      _ExtentY        =   9763
      MultiRow        =   -1  'True
      HotTracking     =   -1  'True
      TabMinWidth     =   0
      ImageList       =   "ImageList1"
      _Version        =   393216
      BeginProperty Tabs {1EFB6598-857C-11D1-B16A-00C0F0283628} 
         NumTabs         =   6
         BeginProperty Tab1 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "������ת����"
            Object.ToolTipText     =   "��������ת��Ϊ���Ի����ģ�Ҳ��ʵ�ּ��嵽�����ת����"
            ImageVarType    =   2
            ImageIndex      =   1
         EndProperty
         BeginProperty Tab2 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "�����ķ�����"
            Object.ToolTipText     =   "�������Ķ��Ļ�����ת��Ϊ�������ġ�"
            ImageVarType    =   2
            ImageIndex      =   2
         EndProperty
         BeginProperty Tab3 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "ǩ����������"
            Object.ToolTipText     =   "����������������װ���Է��š�"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab4 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "ƴ����ʻ���"
            Object.ToolTipText     =   "��ѯ���ֶ�Ӧ�ĺ���ƴ�����߼������뺺�ֵıʻ���"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab5 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Ȥζ��ͼ����"
            Object.ToolTipText     =   "ʹ��Ȥζ��ͼ�����������������Ȥ��"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab6 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "�Զ����칤��"
            Object.ToolTipText     =   "��������ʱ,�˹��߿����������ܻظ�����QQ��Ϣ"
            ImageVarType    =   2
            ImageIndex      =   3
         EndProperty
      EndProperty
   End
   Begin VB.Menu gmypop0 
      Caption         =   "0"
      Visible         =   0   'False
      Begin VB.Menu gmypop1 
         Caption         =   "��ʾ������"
      End
      Begin VB.Menu gmypop2 
         Caption         =   "-"
      End
      Begin VB.Menu gmypop3 
         Caption         =   "ת����ģʽ(Ctrl+F12)"
      End
      Begin VB.Menu gmypop4 
         Caption         =   "���뷨���ģʽ"
      End
      Begin VB.Menu gmypop5 
         Caption         =   "-"
      End
      Begin VB.Menu gmypop6 
         Caption         =   "�˳�"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private myFrameNow As Integer, fun5_con As Long, fun5_id As Long
Private can_change_call As Boolean

Private Sub Check2_Click()
mysetting_change
End Sub

Private Sub Check3_Click()
If Check3.Value = 1 Then
Check4.Enabled = True
Timer1.Enabled = True
Else
Check4.Enabled = False
Timer1.Enabled = False
End If
End Sub

Private Sub Combo1_Click()
mysetting_change
End Sub

Private Sub Combo2_Click()
mysetting_change
End Sub

Private Sub Command1_Click()
If Text1.Text = "" Then Exit Sub
Text2.Text = pub_fun_jt2bt(Text1.Text)
If Check1(1).Value = 1 Then
Clipboard.Clear
Clipboard.SetText Text2.Text
End If
End Sub

Private Sub Command10_Click()
Clipboard.Clear
Clipboard.SetText Text7.Text
End Sub

Private Sub Command11_Click()
qqchat_kernel
End Sub

Private Sub Command12_Click()
Dim myinput As String, myoutput As String
myinput = Trim(Text13.Text)
If myinput = "" Then
MsgBox "������ߵķ�������Ҫת�������ݣ�"
Text13.SetFocus
Exit Sub
End If
If Option9(0).Value = True Then
   myoutput = "�����������һ����" & CStr(char_wordnum(myinput)) & "����" & vbCrLf & "���У�" & vbCrLf
   For i = 1 To Len(myinput)
       myoutput = myoutput & Mid(myinput, i, 1) & " -- " & CStr(char_wordnum(Mid(myinput, i, 1))) & "��" & vbCrLf
   Next
   Text14.Text = myoutput
Else
   Text14.Text = Trim(PinYinGetByText(Replace(myinput, " ", "")))
End If
End Sub

Private Sub Command13_Click()
Text13.Text = Clipboard.GetText
End Sub

Private Sub Command14_Click()
Text13.Text = ""
End Sub

Private Sub Command15_Click()
Clipboard.Clear
Clipboard.SetText Text14.Text
End Sub

Private Sub Command16_Click()
Command17_Click 0
End Sub

Private Sub Command17_Click(Index As Integer)
On Error Resume Next
sql = "select * from sucai where "
If Combo3.Text = "<ȫ��ͼ��>" Then sql = sql & " [mytype] in (select [mytype] from sucaitype where shuyu='ͼ��') "
If Combo3.Text = "<ȫ������>" Then sql = sql & " [mytype] in (select [mytype] from sucaitype where shuyu='����') "
If Combo3.Text = "<ȫ������>" Then sql = sql & " [mytype] in (select [mytype] from sucaitype where shuyu='����') "
If Combo3.Text <> "<ȫ��ͼ��>" And Combo3.Text <> "<ȫ������>" And Combo3.Text <> "<ȫ������>" Then sql = sql & " [mytype]='" & Combo3.Text & "'"
If Text15.Text <> "" Then
   If Option10(1).Value = True Then
      sql = sql & " and ([myname] like '%" & Text15.Text & "%' or [mytext] like '%" & Text15.Text & "%') "
   Else
      sql = sql & "and [myname] like '%" & Text15.Text & "%' "
   End If
End If
rs_pub.open sql, conn, 1, 1
Text12.Text = ""
Text17.Text = ""

If Index = 0 Then
fun5_con = 1
Command17(1).Enabled = False
If rs_pub.EOF Or fun5_con = rs_pub.RecordCount Then Command17(2).Enabled = False Else Command17(2).Enabled = True
End If

If Index = 1 Then
fun5_con = fun5_con - 1
rs_pub.Move fun5_con - 1
If rs_pub.bof Or fun5_con = 1 Then Command17(1).Enabled = False Else Command17(1).Enabled = True
If rs_pub.EOF Or fun5_con = rs_pub.RecordCount Then Command17(2).Enabled = False Else Command17(2).Enabled = True
End If

If Index = 2 Then
fun5_con = fun5_con + 1
rs_pub.Move fun5_con - 1
If rs_pub.bof Or fun5_con = 1 Then Command17(1).Enabled = False Else Command17(1).Enabled = True
If rs_pub.EOF Or fun5_con = rs_pub.RecordCount Then Command17(2).Enabled = False Else Command17(2).Enabled = True
End If

If Index = 3 Then
fun5_con = rs_pub.RecordCount
rs_pub.movelast
Command17(2).Enabled = False
If rs_pub.bof Or fun5_con = 1 Then Command17(1).Enabled = False Else Command17(1).Enabled = True
End If

If Index = 4 Then
If Not IsNumeric(Text18.Text) Then Text18.Text = "1"
fun5_con = CLng(Text18.Text)
If fun5_con > rs_pub.RecordCount Then fun5_con = rs_pub.RecordCount
If fun5_con < 1 Then fun5_con = 1
rs_pub.Move fun5_con - 1
If rs_pub.bof Or fun5_con = 1 Then Command17(1).Enabled = False Else Command17(1).Enabled = True
If rs_pub.EOF Or fun5_con = rs_pub.RecordCount Then Command17(2).Enabled = False Else Command17(2).Enabled = True
End If

Label16.Caption = "�ܹ�" & CStr(rs_pub.RecordCount) & "ҳ����ǰ��" & CStr(fun5_con) & "ҳ"
Text12.Text = rs_pub("mytext")
Text17.Text = rs_pub("myname")
fun5_id = rs_pub("id")
rs_pub.Close
End Sub

Private Sub Command18_Click(Index As Integer)
If Index = 0 Then
Command18(1).Enabled = True
Command18(0).Enabled = False
Text12.Locked = False
Text17.Locked = False
End If

If Index = 1 Then
Command18(1).Enabled = False
Command18(0).Enabled = True
Text12.Locked = True
Text17.Locked = True
sql = "select * from sucai where [id]=" & CStr(fun5_id)
rs_pub.open sql, conn, 1, 3
rs_pub("myname") = Text17.Text
rs_pub("mytext") = Text12.Text
rs_pub.Update
rs_pub.Close
End If

If Index = 2 Then
If MsgBox("���Ҫɾ������ͼ��", vbYesNo, "ȷ��ɾ��") = vbYes Then
conn.execute "delete from sucai where [id]=" & CStr(fun5_id)
Command17_Click 0
End If
End If
End Sub

Private Sub Command19_Click(Index As Integer)
On Error Resume Next
If Index = 0 Then
Text12.Locked = False
Text17.Locked = False
Text12.Text = ""
Text17.Text = ""
Command19(0).Enabled = False
Command19(1).Enabled = True
Text12.SetFocus
End If

If Index = 1 Then
If Text12.Text = "" Then MsgBox "��������ͼ�����ݣ�": Text12.SetFocus: Exit Sub
If Combo3.Text = "<ȫ��ͼ��>" Or Combo3.Text = "<ȫ������>" Or Combo3.Text = "<ȫ������>" Then MsgBox "��Ϊ����ͼѡ��һ����ϸ���࣡": Combo3.SetFocus: Exit Sub
Text12.Locked = True
Text17.Locked = True
Command19(0).Enabled = True
Command19(1).Enabled = False
sql = "select * from sucai "
rs_pub.open sql, conn, 1, 3
rs_pub.addnew
rs_pub("myname") = Text17.Text
rs_pub("mytext") = Text12.Text
rs_pub("mytype") = Combo3.Text
rs_pub.Update
rs_pub.Close
End If
End Sub

Private Sub Command2_Click(Index As Integer)
If Index = 0 Then Text1.Text = Clipboard.GetText
If Index = 1 Then
Clipboard.Clear
Clipboard.SetText Text2.Text
End If
If Index = 2 Then Text1.Text = ""
End Sub

Private Sub Command20_Click()
If MsgBox("ɾ�����ཫ��ͬʱɾ���÷����µ�������ͼ�زģ�" & Chr(13) & "Ҫ������", vbYesNo, "ȷ��ɾ��") <> vbYes Then Exit Sub
conn.execute "delete from sucaitype where mytype='" & Combo4.Text & "'"
conn.execute "delete from sucai where mytype='" & Combo4.Text & "'"
update_combo3
update_combo4
MsgBox "����ɾ���ɹ���"
End Sub

Private Sub Command21_Click()
If Text16.Text = "" Then
MsgBox "�������������ƣ�"
Text16.SetFocus
Exit Sub
End If
sql = "insert into sucaitype ([mytype],[shuyu]) values ('" & Text16.Text & "','" & Combo5.Text & "')"
Err.Clear
On Error Resume Next
conn.execute sql
update_combo3
update_combo4
If Err Then
MsgBox "��Ǹ���½�����ʧ�ܣ����ܸ÷����Ѿ����ڡ�"
Else
MsgBox "�½�����ɹ���"
End If
End Sub

Private Sub Command22_Click()
Form3.Show 1
End Sub

Private Sub Command23_Click()
Clipboard.Clear
Clipboard.SetText Text12.Text
End Sub

Private Sub Command24_Click()
Form2.Show 1
End Sub

Private Sub Command25_Click()
Form4.Show 1
End Sub

Private Sub Command26_Click()
g_myICONs.uFlags = NIF_MESSAGE Or NIF_ICON Or NIF_TIP
Shell_NotifyIcon NIM_ADD, g_myICONs
Me.Hide
End Sub

Private Sub Command3_Click()
If Text4.Text = "" Then
MsgBox "������ߵķ�������Ҫ����Ļ����ģ�"
Text4.SetFocus
Exit Sub
End If
mytype = "1"
If Option7(0).Value = True Then mytype = "1"
If Option7(1).Value = True Then mytype = "2"
If Option7(2).Value = True Then mytype = "3"
Text5.Text = bt2jt(Text4.Text, mytype)
End Sub

Private Sub Command4_Click()
Text4.Text = Clipboard.GetText
End Sub

Private Sub Command5_Click()
Text4.Text = ""
End Sub

Private Sub Command6_Click()
Clipboard.Clear
Clipboard.SetText Text5.Text
End Sub

Private Sub Command7_Click()
If Text6.Text = "" Then
MsgBox "������ߵķ�������Ҫת�������ݣ�"
Text6.SetFocus
Exit Sub
End If
Text7.Text = pub_fun_bt2bt(Text6.Text)
End Sub

Private Sub Command8_Click()
Text6.Text = Clipboard.GetText
End Sub

Private Sub Command9_Click()
Text6.Text = ""
End Sub

Private Sub Form_Load()
On Error Resume Next
If App.PrevInstance Then
   Dim tmpS As String
   tmpS = App.Title
   App.Title = ""
   AppActivate tmpS, False
   End
End If
WM_HXWDLLWX_QQBTX = RegisterWindowMessage("WM_HXWDLLWX_QQBTX")
WM_TRAY_CALLBACK = RegisterWindowMessage("WM_TRAY_CALLBACK")
RegisterHotKey Me.hwnd, 1, MOD_CONTROL, vbKeyF12
myFrameNow = 0
can_change_call = False
db = App.Path
If Right(db, 1) <> "\" Then db = db & "\"
db = """" & db & "qqbtzdat.dll" & """"
mytempStr1 = Chr(101) & Chr(107) & Chr(97) & Chr(105) & Chr(99) & Chr(104)
mytempNum = 96
mytempNum = mytempNum + 6
mytempStr1 = mytempStr1 & Chr(mytempNum)
mytempStr2 = mytempStr1 & CStr(92)
mytempNum = mytempNum - 96
mytempStr2 = """" & mytempStr2 & Chr(48) & CStr(mytempNum) & """"
connstr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & db & ";Jet OLEDB:Database Password=" & mytempStr2
Set conn = CreateObject("ADODB.Connection")
conn.open connstr
Set rs_pub = CreateObject("ADODB.recordset")
init_table
init_mycon_setting
init_myfun5
Randomize
qqchat_init
DLLstartHOOK Me.hwnd
PrevWndProc = SetWindowLong(Me.hwnd, GWL_WNDPROC, AddressOf SubWndProc)
wg_workMode_change
g_myICONs.cbSize = Len(g_myICONs)
g_myICONs.hwnd = Me.hwnd
g_myICONs.uID = 0
g_myICONs.uFlags = 0
g_myICONs.uCallbackMessage = WM_TRAY_CALLBACK
g_myICONs.hIcon = Me.Icon.Handle
g_myICONs.szTip = "������ת����" & Chr(0)
End Sub

Private Sub Form_Unload(Cancel As Integer)
save_mysetting
save_qqchat_setting
Set rs = Nothing
conn.Close
Set conn = Nothing
DLLstopHOOK
UnregisterHotKey Me.hwnd, 1
Call SetWindowLong(Me.hwnd, GWL_WNDPROC, PrevWndProc)
End Sub



Private Sub gmypop1_Click()
g_showMainForm
End Sub

Private Sub gmypop3_Click()
If Option1(0).Value <> True Then Option1(0).Value = True
End Sub

Private Sub gmypop4_Click()
If Option1(1).Value <> True Then Option1(1).Value = True
End Sub

Private Sub gmypop6_Click()
g_myICONs.uFlags = 0
Shell_NotifyIcon NIM_DELETE, g_myICONs
Unload Me
End Sub

Private Sub Option1_Click(Index As Integer)
mysetting_change
wg_workMode_change
End Sub

Private Sub Option10_Click(Index As Integer)
update_combo3
End Sub

Private Sub Option2_Click(Index As Integer)
mysetting_change
End Sub

Private Sub Option3_Click(Index As Integer)
mysetting_change
End Sub

Private Sub Option4_Click(Index As Integer)
mysetting_change
End Sub

Private Sub Option5_Click(Index As Integer)
mysetting_change
End Sub

Private Sub Option6_Click(Index As Integer)
mysetting_change
End Sub

Private Sub Option8_Click(Index As Integer)
mysetting_change
End Sub

Private Sub TabStrip1_Click()
If TabStrip1.SelectedItem.Index - 1 = myFrameNow Then Exit Sub
Frame1(myFrameNow).Visible = False
myFrameNow = TabStrip1.SelectedItem.Index - 1
Frame1(myFrameNow).Visible = True
End Sub

Private Sub init_mycon_setting()
' ��ʼ���ؼ��趨
Frame1(0).Visible = True
Frame1(1).Visible = False
Frame1(2).Visible = False
Frame1(3).Visible = False
Frame1(4).Visible = False
Frame1(5).Visible = False
' -----------------------
Combo1.ListIndex = 0
' -----------------------
sql = "select * from mychars "
Set rs = CreateObject("adodb.recordset")
rs.open sql, conn, 1, 1
Do While Not rs.EOF
Combo2.AddItem rs("myname")
rs.movenext
Loop
rs.Close
Set rs = Nothing
Combo2.ListIndex = 0
' ------------------------
sql = "select * from mysetting where [id]=1 "
Set rs = CreateObject("adodb.recordset")
rs.open sql, conn, 1, 1
myset_string = rs("myset1")
rs.Close
Set rs = Nothing
myset_array1 = Split(myset_string, ";")
set_work_mode = CInt(myset_array1(0))
set_bt2bt = CInt(myset_array1(1))
set_jt2bt_type = CInt(myset_array1(2))
set_jt2bt_t1 = CDbl(myset_array1(3))
set_jt2bt_t2 = CInt(myset_array1(4))
set_jt2bt_t3 = CInt(myset_array1(5))
set_jt2bt_t4 = CInt(myset_array1(6))
set_jt2bt_t5 = CInt(myset_array1(7))
set_bt2bt_t1 = CInt(myset_array1(8))
set_bt2bt_t2 = CInt(myset_array1(9))
set_combo1 = Combo1.Text
set_bt2bt_t3 = Combo2.Text
' --------------------------
Option1(set_work_mode).Value = True
Check2.Value = set_bt2bt
Option2(set_jt2bt_type).Value = True
Text3.Text = CStr(set_jt2bt_t1)
Option3(set_jt2bt_t2).Value = True
Option4(set_jt2bt_t3).Value = True
Option5(set_jt2bt_t4).Value = True
Option6(set_jt2bt_t5).Value = True
Option8(set_bt2bt_t1).Value = True
Text8.Text = CStr(set_bt2bt_t2)
' --------------------------
can_change_call = True
mysetting_change
End Sub

Sub save_mysetting()
sql = "select * from mysetting where [id]=1 "
Set rs = CreateObject("adodb.recordset")
rs.open sql, conn, 1, 3
rs("myset1") = CStr(set_work_mode) & ";" & CStr(set_bt2bt) & ";" & CStr(set_jt2bt_type) & ";" & CStr(set_jt2bt_t1) & ";" & CStr(set_jt2bt_t2) & ";" & CStr(set_jt2bt_t3) & ";" & CStr(set_jt2bt_t4) & ";" & CStr(set_jt2bt_t5) & ";" & CStr(set_bt2bt_t1) & ";" & CStr(set_bt2bt_t2)
rs.Update
rs.Close
Set rs = Nothing
End Sub

Sub mysetting_change()
If can_change_call = False Then Exit Sub
If Option1(0).Value = True Then
set_work_mode = 0
Command1.Enabled = True
Text1.Enabled = True
Else
set_work_mode = 1
Command1.Enabled = False
Text1.Enabled = False
End If
' -----------------------
set_combo1 = Combo1.Text
If set_combo1 <> "����ת������" Then
Frame3.Enabled = False
Else
Frame3.Enabled = True
End If
' -----------------------
set_bt2bt = Check2.Value
' -----------------------
If Option2(0).Value = True Then set_jt2bt_type = 0
If Option2(1).Value = True Then set_jt2bt_type = 1
If Option2(2).Value = True Then set_jt2bt_type = 2
If Option2(3).Value = True Then set_jt2bt_type = 3
If Option2(3).Value = True Then Frame4.Enabled = True Else Frame4.Enabled = False
If Frame3.Enabled = False Then Frame4.Enabled = False
' ------------------------
If IsNumeric(Text3.Text) Then
   tmp_var = CDbl(Text3.Text)
   If tmp_var >= 0 And tmp_var <= 1 Then
      set_jt2bt_t1 = tmp_var
   Else
      MsgBox "ת��Ƶ�ʱ�����һ��0��1֮���С����"
      Text3.Text = CStr(set_jt2bt_t1)
      Text3.SetFocus
   End If
Else
   If Text3.Text <> "" And Text3.Text <> "." Then
      MsgBox "ת��Ƶ�ʱ�����һ��0��1֮���С����"
      Text3.Text = CStr(set_jt2bt_t1)
      Text3.SetFocus
   End If
End If
' -----------------------
If Option3(0).Value = True Then set_jt2bt_t2 = 0 Else set_jt2bt_t2 = 1
If Option4(0).Value = True Then set_jt2bt_t3 = 0 Else set_jt2bt_t3 = 1
If Option5(0).Value = True Then set_jt2bt_t4 = 0
If Option5(1).Value = True Then set_jt2bt_t4 = 1
If Option5(2).Value = True Then set_jt2bt_t4 = 2
If Option6(0).Value = True Then set_jt2bt_t5 = 0 Else set_jt2bt_t5 = 1
' ------------------------
If Option8(0).Value = True Then
   set_bt2bt_t1 = 0
   Text8.Enabled = True
   Combo2.Enabled = False
Else
   set_bt2bt_t1 = 1
   Text8.Enabled = False
   Combo2.Enabled = True
End If
' -----------------------
If IsNumeric(Text8.Text) Then
   tmp_var = CInt(Text8.Text)
   If tmp_var >= 0 Then
      set_bt2bt_t2 = tmp_var
   Else
      MsgBox "�ܶȱ�����һ������0���֣�"
      Text8.Text = CStr(set_bt2bt_t2)
      Text8.SetFocus
   End If
Else
   If Text8.Text <> "" Then
      MsgBox "�ܶȱ�����һ������0���֣�"
      Text8.Text = CStr(set_bt2bt_t2)
      Text8.SetFocus
   End If
End If
' ----------------------
set_bt2bt_t3 = Combo2.Text
' ----------------------
If Frame3.Enabled = False Then
Option2(0).Enabled = False
Option2(1).Enabled = False
Option2(2).Enabled = False
Option2(3).Enabled = False
Else
Option2(0).Enabled = True
Option2(1).Enabled = True
Option2(2).Enabled = True
Option2(3).Enabled = True
End If

If Frame4.Enabled = False Then
Label1.Enabled = False
Text3.Enabled = False
Label2.Enabled = False
Option3(0).Enabled = False
Option3(1).Enabled = False
Label3.Enabled = False
Option4(0).Enabled = False
Option4(1).Enabled = False
Label4.Enabled = False
Option5(0).Enabled = False
Option5(1).Enabled = False
Option5(2).Enabled = False
Label5.Enabled = False
Option6(0).Enabled = False
Option6(1).Enabled = False
Else
Label1.Enabled = True
Text3.Enabled = True
Label2.Enabled = True
Option3(0).Enabled = True
Option3(1).Enabled = True
Label3.Enabled = True
Option4(0).Enabled = True
Option4(1).Enabled = True
Label4.Enabled = True
Option5(0).Enabled = True
Option5(1).Enabled = True
Option5(2).Enabled = True
Label5.Enabled = True
Option6(0).Enabled = True
Option6(1).Enabled = True
End If

End Sub

Private Sub Text1_Change()
If Check1(0).Value = 1 Then Command1_Click
End Sub



Private Sub Text19_Click()
ShellExecute Me.hwnd, "open", "http://www.huoxingwen.com", "", "", 5
End Sub



Private Sub Text20_Click()
ShellExecute Me.hwnd, "open", "http://www.huoxingyu.com", "", "", 5
End Sub

Private Sub Text3_Change()
mysetting_change
End Sub

Private Sub Text8_Change()
mysetting_change
End Sub
Sub init_myfun5()
update_combo3
update_combo4
Combo5.ListIndex = 0
Command17_Click 0
Text12.ForeColor = RGB(&HFF, &H0, &H99)
End Sub

Sub update_combo3()
tmp_str = ""
If Option10(0).Value = True Then tmp_str = "ͼ��"
If Option10(1).Value = True Then tmp_str = "����"
If Option10(2).Value = True Then tmp_str = "����"
Combo3.Clear
Combo3.AddItem "<ȫ��" & tmp_str & ">"
sql = "select * from sucaitype where [shuyu]='" & tmp_str & "'"
rs_pub.open sql, conn, 1, 1
Do While Not rs_pub.EOF
Combo3.AddItem rs_pub("mytype")
rs_pub.movenext
Loop
rs_pub.Close
Combo3.ListIndex = 0
End Sub

Sub update_combo4()
Combo4.Clear
sql = "select * from sucaitype "
rs_pub.open sql, conn, 1, 1
Do While Not rs_pub.EOF
Combo4.AddItem rs_pub("mytype")
rs_pub.movenext
Loop
rs_pub.Close
Combo4.ListIndex = 0
End Sub

Sub wg_workMode_change()
If Option1(1).Value = True Then
   DLLsetHOOKState True
Else
   DLLsetHOOKState False
End If
End Sub

Private Sub Timer1_Timer()
Timer1.Enabled = False
Dim i As Long, array_long As Long
If Check4.Value = 1 Then
   tmp_array = Split(qqchat_hotkey, "|")
   array_long = UBound(tmp_array)
   For i = 0 To array_long
       keybd_event CByte(tmp_array(i)), 0, 0, 0
   Next
   For i = array_long To 0 Step -1
       keybd_event CByte(tmp_array(i)), 0, KEYEVENTF_KEYUP, 0
   Next
End If

QQMsgDoLoop
Timer1.Enabled = True
End Sub
