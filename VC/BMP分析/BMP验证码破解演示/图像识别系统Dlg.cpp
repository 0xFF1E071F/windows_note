// ͼ��ʶ��ϵͳDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ͼ��ʶ��ϵͳ.h"
#include "ͼ��ʶ��ϵͳDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define			BMP_CODE_NAME		"Code.bmp"			//��֤�뱣��Ϊ���ļ���


#pragma pack(1)		//�ṹ����뷽ʽ,��һ�ֽڶ���

//BMP�ļ�ͷ�ṹ��
typedef struct _BMPHead
{
	short Head_Flag;	//1		//0x00-0x01 bmp�ļ���־���̶�Ϊ'BM'
	int File_Size;		//2		//0x02-0x05 �ļ��ܳ��ȣ������ļ�ͷ
	int Reserved;		//3		//0x06-0x09 ����δ��
	int Data_Offset;	//4		//0x0A-0x0D �������Ŀ�ʼλ�ã����ļ�ͷ����
	int Head_Info_Size;	//5		//0x0E-0x11 λͼ��Ϣͷ�Ĵ�С,Windows��Ϊ0x28
	int Pic_Width;		//6		//0x12-0x15 ͼ�εĿ�ȣ���λ������
	int Pic_High;		//7		//0x16-0x19 ͼ�εĸ߶ȣ���λ����
	short Pic_Plans;	//8		//0x1A-0x1B ͼ�ε�ҳ��
	short Color_Bit;	//9		//0x1C-0x1D ��ɫ��λ������ȣ�
	int Compress;		//10		//0x1E-0x21 ѹ����ʽ��0��ʾδѹ��
	int Data_Size;		//11		//0x22-0x25 �������ĳ���
	int Pic_Xppm;		//12		//0x26-0x29 ˮƽ�ֱ���
	int Pic_Yppm;		//13		//0x2A-0x2D ��ֱ�ֱ���
	int Color_Used;		//14		//0x2E-0x31 λͼʹ�õ���ɫ��
	int Color_Imp;		//15		//0x32-0x35 ��Ҫ��ɫ��Ŀ
} BMPHead, *pBMPHead;


typedef struct _RESULT_NUM
{
	int LastResult[5];		//���������֤����
	int Correct[5];			//�����֤������ƥ����
} RESULT_NUM, *pRESULT_NUM;

//�����ַ�������Ϣ
char Num[10][56] = \
{
	/*0*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xCF\xCC\xFF\xFF\xCC\xCF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFC\xCC\xFF\xFF\xCC\xFC\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*1*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xCC\xCC\xCF\xFF\xFF\xFF\xCC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*2*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xCC\xCC\xCC\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*3*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xCC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*4*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xCC\xCC\xCC\xCF\xFF\xCC\xFF\xCC\xFF\xFF\xFC\xCF\xCC\xFF\xFF\xFC\xCF\xCC\xFF\xFF\xFC\xCF\xCC\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*5*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xCC\xCC\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xCC\xCC\xCF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xCC\xCC\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*6*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xCC\xCF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*7*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xCC\xCC\xCC\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*8*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFC\xCC\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xCC\xCF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"},
	/*9*/{"\xFF\xFF\xFF\xFF\xFF\xFF\xFC\xCC\xFF\xFF\xFF\xFF\xCC\xFF\xFF\xFF\xFF\xFC\xCF\xFF\xFF\xFC\xCC\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xCC\xFF\xCC\xFF\xFF\xFC\xCC\xCF\xFF\xFF\xFF\xFF\xFF\xFF"}
};

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	m_Url = _T("http://www.huihu32.cn/function/c_validcode.asp?name=commentvalid");
	m_Num = _T("");
	m_Correct = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	DDX_Text(pDX, IDC_URL, m_Url);
	DDX_Text(pDX, IDC_NUM, m_Num);
	DDX_Text(pDX, IDC_CORRECT, m_Correct);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetProcessInfo( "��ӭʹ�� Adly ��������֤��ʶ�𹤾�!" );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
	//���¼�����֤��ͼ��
	HBITMAP Bitmap, OldBitmap;
	Bitmap = (HBITMAP)LoadImage( AfxGetInstanceHandle(), BMP_CODE_NAME, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	CDC MemDC;
	CDC *pDC;
	pDC = GetDC();
	MemDC.CreateCompatibleDC( pDC );
	OldBitmap = (HBITMAP)MemDC.SelectObject( Bitmap );
	pDC->BitBlt( 270, 175, 50, 11, &MemDC, 0, 0, SRCCOPY );
	MemDC.SelectObject( OldBitmap );
}

HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::SetProcessInfo( char* Str )	//���ù�����Ϣ
{
	GetDlgItem( IDC_Result )->SetWindowText( Str );
}

BOOL CMyDlg::DownLoadBmp()
{
	UpdateData( TRUE );	//�õ����º����֤����ַ

	LONG ret = ::URLDownloadToFile( 0, m_Url, BMP_CODE_NAME, 0, 0 );	//������֤�룬����Ϊ Code.bmp

	if( !ret )		//�ɹ�
	{
		return TRUE;
	}

	return FALSE;
}

void CMyDlg::OnStart()	//��ʼʶ����֤��
{
	/////////////////////////////
	//������֤��
	SetProcessInfo( "����������֤�� ..." );
	if( !DownLoadBmp() )
	{
		SetProcessInfo( "������֤��ʧ�� ..." );
		return ;
	}
	/////////////////////////
	//��ʾ��֤��
	SetProcessInfo( "���ڼ�����֤�� ..." );
	HBITMAP Bitmap, OldBitmap;
	Bitmap = (HBITMAP)LoadImage( AfxGetInstanceHandle(), BMP_CODE_NAME, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	CDC MemDC;
	CDC *pDC;
	pDC = GetDC();
	MemDC.CreateCompatibleDC( pDC );
	OldBitmap = (HBITMAP)MemDC.SelectObject( Bitmap );
	pDC->BitBlt( 270, 175, 50, 11, &MemDC, 0, 0, SRCCOPY );
	MemDC.SelectObject( OldBitmap );
	/////////////////////////
	//������֤��
	SetProcessInfo( "���ڷ�����֤������ ..." );
	//����֤���ļ�
	HANDLE hBmpCode = ::CreateFile( BMP_CODE_NAME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hBmpCode == INVALID_HANDLE_VALUE )		//��ʧ��
	{
		SetProcessInfo( "��Bmp�ļ�ʧ�� ..." );
		return ;
	}
	/////////////////////////////////////
	//���ｫbmp����д�뵽 hBmpData ����

	//��ȡBMP��Ϣͷ
	DWORD dwRead;
	BMPHead BmpHead;
	BOOL Success = ::ReadFile( hBmpCode, &BmpHead, sizeof( BmpHead ), &dwRead, NULL );
	if( !Success || ( dwRead != sizeof( BMPHead ) ) )		//��ȡ�ļ�ͷʧ��
	{
		CloseHandle( hBmpCode );
		SetProcessInfo( "��ȡBmp��Ϣͷʧ�� ..." );
		return ;
	}

	char BmpDataBuffer[5][55];

	//�����ļ�ָ��
	::SetFilePointer( hBmpCode, BmpHead.Data_Offset, NULL, FILE_BEGIN );

	int i, j;
	for( i = 0; i < 11; i ++ )		//BMPͼƬ���ظ�
	{
		for( j = 0; j < 5; j ++ )		//BMPͼƬ�����������
		{
			//��ȡ���ݵ����������
			Success = ::ReadFile( hBmpCode, (void*)&BmpDataBuffer[j][i*5], 5, &dwRead, NULL );	//ÿ�ζ�ȡ 5 byte
			if( !Success || ( dwRead != 5 ) )		//��ȡʧ��
			{
				CloseHandle( hBmpCode );
				SetProcessInfo( "��ȡBmp����ʧ�� ..." );
				return ;
			}
		}
		::SetFilePointer( hBmpCode, 3, NULL, FILE_CURRENT );		//�ļ�ָ�������3�ֽ�
	}

	//ȥ��ͼƬ�е�����
	for( i = 0; i < 5; i ++ )		//5������
	{
		for( j = 0; j < 55; j ++ )		//ÿ������ 55 byte��ʾ
		{
			if( ( BmpDataBuffer[i][j] & 0xF0 ) == 0x80 )
			{
				BmpDataBuffer[i][j] |= 0xF0 ;
			}
			if( ( BmpDataBuffer[i][j] & 0x0F ) == 0x08 )
			{
				BmpDataBuffer[i][j] |= 0x0F ;
			}
		}
	}

	//��������ƥ��
	RESULT_NUM Result;		//�������Ľ��

	int k;
	for( k = 0; k < 5; k ++ )			//5������
	{
		int CorrectCount[10] = {0,0,0,0,0,0,0,0,0,0};
		int Total = 0;
		for( i = 0; i < 55; i ++ )		//�Ƚ� 55 ���ֽ�
		{
			for( j = 0; j < 10; j ++ )		//�Ƚ� 10 ����ģ����ƥ��
			{
				Total ++;
				Total ++;
				if( ( BmpDataBuffer[k][i] & 0xF0 ) == ( Num[j][i] & 0xF0 ) )	//��4λƥ��ɹ�
				{
					CorrectCount[j] ++;
				}
				if( ( BmpDataBuffer[k][i] & 0x0F ) == ( Num[j][i] & 0x0F ) )	//��4λƥ��ɹ�
				{
					CorrectCount[j] ++;
				}
			}
		}
		Total /= 10;
		//����ͳ�ƱȽϳ����ƥ��
		Result.LastResult[k] = 0;
		int MaxCorrect = 0;
		for( i = 0; i < 10; i ++ )		//��10��ͳ����ѡ�����ֵ
		{
			if( MaxCorrect < CorrectCount[i] )
			{
				MaxCorrect = CorrectCount[i];
				Result.LastResult[k] = i;
			}
		}
		Result.Correct[k] = (int)( (double)MaxCorrect/(double)Total * 100.0 );
	}

	//��������õ���ʾ����
	char ResultNumString[6];
	char ResultCorrectString[20];
	sprintf( ResultNumString, "%d%d%d%d%d", Result.LastResult[0], Result.LastResult[1], \
		Result.LastResult[2], Result.LastResult[3], Result.LastResult[4] );
	sprintf( ResultCorrectString, "%d|%d|%d|%d|%d", Result.Correct[0], Result.Correct[1], \
		Result.Correct[2], Result.Correct[3], Result.Correct[4] );

	m_Num.Format( "%s", ResultNumString );
	m_Correct.Format( "%s", ResultCorrectString );

	UpdateData( FALSE );

	CloseHandle( hBmpCode );
	SetProcessInfo( "ͼ��ʶ�����!!!" );
}

void CMyDlg::OnClose() 
{
	//ɾ�� BMP_CODE_NAME �ļ�
	::DeleteFile( BMP_CODE_NAME );
	CDialog::OnClose();
}
