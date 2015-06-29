/*
 *	���ߣ�adly
 */
#include <stdio.h>
#include <windows.h>

#pragma pack(1)						// �ṹ����뷽ʽ,��һ�ֽڶ���

#define  INFILENAME  "inBMP.bmp"	// ����λͼ�ļ���
#define  OUTFILENAME  "outBMP.txt"	// ���λͼ�ļ���

// BMP�ļ�ͷ�ṹ��
typedef struct _BMPHead
{
	short Head_Flag;	//1   //0x00-0x01 bmp�ļ���־���̶�Ϊ'BM'
	int File_Size;		//2   //0x02-0x05 �ļ��ܳ��ȣ������ļ�ͷ
	int Reserved;		//3   //0x06-0x09 ����δ��
	int Data_Offset;	//4   //0x0A-0x0D �������Ŀ�ʼλ�ã����ļ�ͷ����
	int Head_Info_Size;//5   //0x0E-0x11 λͼ��Ϣͷ�Ĵ�С,Windows��Ϊ0x28
	int Pic_Width;		//6   //0x12-0x15 ͼ�εĿ�ȣ���λ������
	int Pic_High;		//7   //0x16-0x19 ͼ�εĸ߶ȣ���λ����
	short Pic_Plans;	//8   //0x1A-0x1B ͼ�ε�ҳ��
	short Color_Bit;	//9   //0x1C-0x1D ��ɫ��λ������ȣ�
	int Compress;		//10  //0x1E-0x21 ѹ����ʽ��0��ʾδѹ��
	int Data_Size;		//11  //0x22-0x25 �������ĳ���
	int Pic_Xppm;		//12  //0x26-0x29 ˮƽ�ֱ���
	int Pic_Yppm;		//13  //0x2A-0x2D ��ֱ�ֱ���
	int Color_Used;	//14  //0x2E-0x31 λͼʹ�õ���ɫ��
	int Color_Imp;		//15  //0x32-0x35 ��Ҫ��ɫ��Ŀ
}BMPHead, *pBMPHead;

// ��ȡBMP�ļ�ͷ����
bool ReadBMPHeadFromFile( FILE *in, BMPHead & bmpHead )
{
	if ( fgets( (char*)&bmpHead, sizeof( bmpHead ), in ) == NULL )
	{
		printf( "��ȡ�ļ�ͷʱ���ִ���!\n" );
		return FALSE;
	}
	return TRUE;
}
// ��ȡBMP���ݵ��ļ�
bool ReadBMPDataFromFile( FILE *in, FILE *out )
{
	int ch;
	ch = fgetc( in );
	while( ! feof( in ) )
	{
		ch = fgetc( in );
		if ( ch == 0xFFFFFFFF ) break;
		fprintf( out, "%02X ", ch );
	}
	return TRUE;
}
// ��ʾBMP�ļ�ͷ����
bool DisplayBMPHead( BMPHead & bmpHead )
{
	char *bHeadInfo = (char*) malloc( 500 + sizeof( BMPHead ) );
	if ( bHeadInfo == NULL )
	{
		printf( "�����ڴ�ʧ��!\n" );
		return FALSE;
	}
	
	sprintf( bHeadInfo, "�ļ���־:\t%c%c\n", *((char*)&bmpHead), *((char*)&bmpHead + 1) );
	sprintf( bHeadInfo, "%s�ļ�����:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.File_Size, bmpHead.File_Size );
	sprintf( bHeadInfo, "%s����:\t\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Reserved, bmpHead.Reserved );
	sprintf( bHeadInfo, "%s������ʼλ��:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Data_Offset, bmpHead.Data_Offset );
	sprintf( bHeadInfo, "%sλͼ��Ϣͷ��С:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Head_Info_Size, bmpHead.Head_Info_Size );
	sprintf( bHeadInfo, "%sλͼ���:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Pic_Width, bmpHead.Pic_Width );
	sprintf( bHeadInfo, "%sλͼ�߶�:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Pic_High, bmpHead.Pic_High );
	sprintf( bHeadInfo, "%sλͼҳ��:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Pic_Plans, bmpHead.Pic_Plans );
	sprintf( bHeadInfo, "%s��ɫ���:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Color_Bit, bmpHead.Color_Bit );
	sprintf( bHeadInfo, "%sѹ����ʽ:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Compress, bmpHead.Compress );
	sprintf( bHeadInfo, "%s���ݴ�С:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Data_Size, bmpHead.Data_Size );
	sprintf( bHeadInfo, "%sˮƽ�ֱ���:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Pic_Xppm, bmpHead.Pic_Xppm );
	sprintf( bHeadInfo, "%s��ֱ�ֱ���:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Pic_Yppm, bmpHead.Pic_Yppm );
	sprintf( bHeadInfo, "%sλͼ��ɫ��:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Color_Used, bmpHead.Color_Used );
	sprintf( bHeadInfo, "%s��Ҫ��ɫ��:\t%d\t(0x%X)\n", bHeadInfo, bmpHead.Color_Imp, bmpHead.Color_Imp );
	printf( "%s", bHeadInfo );
	
	free( bHeadInfo );
	return TRUE;
}
// ������
int main()
{
	FILE *in, *out;
	BMPHead bHead;
	if (( in = fopen( INFILENAME, "rb" )) == NULL )
	{
		printf( "���ļ� %s ʱ����!\n", INFILENAME );
		system( "pause>nul" );
		return -1;
	}
	if ( ! ReadBMPHeadFromFile( in, bHead ) )	// ���ļ���ȡBMP�ļ�ͷ
	{
		printf( "���ļ���ȡBMP�ļ�ͷ���ṹ��ʧ��!\n" );
		system( "pause>nul" );
		return -1;
	}
	if ( ! DisplayBMPHead( bHead ) )			// ��ʾBMP�ļ�ͷ
	{
		printf( "��ʾBMP�ļ�ͷʧ��!\n" );
		system( "pause>nul" );
		return -1;
	}
	// ������ļ�,��ȡBMP���ݵ����ļ�
	if (( out = fopen( OUTFILENAME, "wb" )) == NULL )
	{
		printf( "���ļ� %s ʱ����!\n", OUTFILENAME );
		system( "pause>nul" );
		return -1;
	}
	if ( ! ReadBMPDataFromFile( in, out ) )		// ��ȡBMP���ݵ��ļ�
	{
		printf( "��ȡBMP���ݵ��ļ�ʧ��!\n" );
		system( "pause>nul" );
		return -1;
	}
	else
	{
		printf( "\n\nBMP �����ļ��Ѷ�ȡ���ļ� %s ��!!!\n", OUTFILENAME );
	}
	fclose( out );
	fclose( in );
	system( "pause>nul" );
	return 0;
}