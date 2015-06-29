// syn_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>

#include <winsock2.h>
#pragma comment(lib,"WS2_32")

#include <WS2TCPIP.h>
#include <time.h>

// New WSAIoctl Options
#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST      _WSAIOW(IOC_VENDOR,2)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)
#define SIO_ABSORB_RTRALERT   _WSAIOW(IOC_VENDOR,5)
#define SIO_UCAST_IF          _WSAIOW(IOC_VENDOR,6)
#define SIO_LIMIT_BROADCASTS  _WSAIOW(IOC_VENDOR,7)
#define SIO_INDEX_BIND        _WSAIOW(IOC_VENDOR,8)
#define SIO_INDEX_MCASTIF     _WSAIOW(IOC_VENDOR,9)
#define SIO_INDEX_ADD_MCAST   _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST   _WSAIOW(IOC_VENDOR,11) 


typedef struct _iphdr 
{
	unsigned char h_lenver; //4λ�ײ�����+4λIP�汾��
	unsigned char tos; //8λ��������TOS
	unsigned short total_len; //16λ�ܳ��ȣ��ֽڣ�
	unsigned short ident; //16λ��ʶ
	unsigned short frag_and_flags; //3λ��־λ
	unsigned char ttl; //8λ����ʱ�� TTL
	unsigned char proto; //8λЭ�� (TCP, UDP ������)
	unsigned short checksum; //16λIP�ײ�У���
	unsigned int sourceIP; //32λԴIP��ַ
	unsigned int destIP; //32λĿ��IP��ַ
}IP_HEADER;

typedef struct _tcphdr //����TCP�ײ�
{
	USHORT th_sport; //16λԴ�˿�
	USHORT th_dport; //16λĿ�Ķ˿�
	unsigned int th_seq; //32λ���к�
	unsigned int th_ack; //32λȷ�Ϻ�
	unsigned char th_lenres; //4λ�ײ�����/6λ������
	unsigned char th_flag; //6λ��־λ
	USHORT th_win; //16λ���ڴ�С
	USHORT th_sum; //16λУ���
	USHORT th_urp; //16λ��������ƫ����
}TCP_HEADER; 

struct //����TCPα�ײ�
{
	unsigned long saddr; //Դ��ַ
	unsigned long daddr; //Ŀ�ĵ�ַ
	char mbz;
	char ptcl; //Э������
	unsigned short tcpl; //TCP����
}psd_header;

SOCKET sockRaw = INVALID_SOCKET,sockListen = INVALID_SOCKET;
int BeginPort,EndPort;
char *HOST;
int iErrorCode;
struct sockaddr_in dest;
BOOL StopScan = FALSE;

#define SEQ 0x28376839

void CheckSockError(int ierror,char *pErrorMsg)
{
	if (ierror == SOCKET_ERROR)
	{
		printf("%s ErrorCode:%d\n",pErrorMsg,ierror);
		closesocket(sockRaw);
		ExitProcess(-1);
	}
}


void meesage()
{
	printf("\t-------syn by panda--------------------------\n");
	printf("\t-------syn_test [ip] [port-port]-------------\n");
	printf("\t-------example: syn_test 127.0.0.1 1-1000----\n");
}
BOOL DecodeIPHeader(char *RecvBuf)
{
	IP_HEADER *iphdr = (IP_HEADER*)RecvBuf;
	unsigned short iphdrlen = sizeof(unsigned long) * (iphdr->h_lenver&0xf);
	TCP_HEADER* tcphdr = (TCP_HEADER*)(RecvBuf + iphdrlen);
	
	if (iphdr->sourceIP != dest.sin_addr.s_addr)
	{
		return FALSE;
	}
	if (ntohl(tcphdr->th_ack) != (SEQ+1) && ntohl(tcphdr->th_ack) != SEQ)
	{
		return FALSE;
	}
	if (tcphdr->th_flag == 18)
	{
		printf("\t%d open \n",ntohs(tcphdr->th_sport));
		return true;
	}
	return FALSE;
}


DWORD WINAPI RecvThread(LPVOID para)//���������߳�
{
	//���������׽���
	sockListen = socket(AF_INET,SOCK_RAW,IPPROTO_IP);
	CheckSockError(sockListen,"RecvThread` socket");
	
	BOOL bOpt =true;
	iErrorCode = setsockopt(sockRaw,IPPROTO_IP,IP_HDRINCL,(char*)&bOpt,sizeof(bOpt));
	CheckSockError(iErrorCode,"RecvThread setsockopt");
	
	//��ñ���IP
	char LocalName[256];
	gethostname(LocalName,sizeof(LocalName));
	struct hostent * my_hostent = gethostbyname(LocalName);
	
	SOCKADDR_IN sa;
	memcpy(&sa.sin_addr.S_un.S_addr,my_hostent->h_addr_list[0],my_hostent->h_length);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(8000);
	
	iErrorCode = bind(sockListen,(sockaddr*)&sa,sizeof(sa));
	CheckSockError(iErrorCode,"bind");

	//����SOCK_RAWΪSIO_RCVALL���Ա�������е�IP��
	DWORD dwBufferInLen = 1;
	DWORD dwBufferLen[10];
	DWORD dwBytesReturned = 0;
	iErrorCode = WSAIoctl(sockListen,SIO_RCVALL,\
		&dwBufferInLen,sizeof(dwBufferInLen),\
		&dwBufferLen,sizeof(dwBufferLen),&dwBytesReturned,NULL,NULL);
	CheckSockError(iErrorCode,"RecvThread WSAIoctl");
	
	char RecvBuf[65535]={0};
	memset(RecvBuf,0,sizeof(RecvBuf));
	
	while (1)//ѭ������  ���� ���ݰ�
	{
		iErrorCode = recv(sockListen,RecvBuf,sizeof(RecvBuf),0);
		//CheckSockError(iErrorCode,"RecvThread  recv");
		DecodeIPHeader(RecvBuf);
		if (StopScan == TRUE)
		{
			closesocket(sockListen);
			return 0;
		}
	}
	return 0;
}



USHORT CalcCheckSum(USHORT *buffer,int size)
{
	unsigned long cksum = 0;
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size)
	{
		cksum += *(USHORT*)buffer;
	}
	cksum = (cksum >> 16) + (cksum &0xffff);
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}
int play=0;
void progressbar(void)
{
	// ������ 
	char *plays[12]= 
	{ 
		" | ", 
			" / ", 
			" - ", 
			" \\ ", 
			" | ", 
			" / ", 
			" - ", 
			" \\ ", 
			" | ", 
			" / ", 
			" - ", 
			" \\ ", 
	}; 
	printf(" =%s=\r", plays[play]);
	play = (play==11)?0:play+1;
	Sleep(2); 
}
int main(int argc, char* argv[])
{
	char *p;
	if (argc != 3)
	{
		meesage();
		return 0;
	}
	p = argv[2];
	if (strstr(argv[2],"-"))
	{
		BeginPort = atoi(argv[2]);
		while (*p)
		{
			if (*(p++) == '-')
			{
				break;
			}
		}
		EndPort = atoi(p);
		if (BeginPort <1 || BeginPort>65535 ||EndPort<1|| EndPort >65535|| EndPort<EndPort)
		{
			meesage();
			return 0;
		}
	}
	HOST = argv[1];
	meesage();

	WSADATA wsadata;
	iErrorCode = WSAStartup(MAKEWORD(2,2),&wsadata);
	CheckSockError(iErrorCode, "WsaStartup()");

	//////////////////////////////////////////////////////////////////////////////
	sockRaw = socket(AF_INET,SOCK_RAW,IPPROTO_IP);
	CheckSockError(sockRaw, "socket()");
	//����IPͷ����ѡ�� �Ƿ���TCP���ĵ��׽���
	BOOL bOpt = true;
	setsockopt(sockRaw,IPPROTO_IP,IP_HDRINCL,(char*)&bOpt,sizeof(bOpt));
	CheckSockError(sockRaw,"setsockopt()");

	//���Ŀ������IP ,ͨ����������  ��һ�����ְ�
	memset(&dest,0,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = ntohs(BeginPort);

	struct hostent *my_hostent;
	if ((dest.sin_addr.s_addr = inet_addr(HOST)) == INADDR_NONE)
	{
		if ((my_hostent = gethostbyname(HOST)) == NULL)
		{
			memcpy(&(dest.sin_addr),my_hostent->h_addr_list[0],my_hostent->h_length);
			dest.sin_family = my_hostent->h_addrtype;
			printf("dest.sin_addr = %s",inet_ntoa(dest.sin_addr));
		}
		else
		{
			CheckSockError(SOCKET_ERROR,"gethostbyname");
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	sockListen = socket(AF_INET , SOCK_RAW , IPPROTO_IP);
	CheckSockError(sockListen, "socket");
	
	//��ñ���IP
	SOCKADDR_IN sa;
	unsigned char LocalName[256];
	struct hostent *hp;
	
	iErrorCode = gethostname((char*)LocalName, sizeof(LocalName)-1);
	CheckSockError(iErrorCode, "gethostname()");
	if((hp = gethostbyname((char*)LocalName)) == NULL)
	{
		CheckSockError(SOCKET_ERROR, "gethostbyname()");
	}
	memcpy(&sa.sin_addr.S_un.S_addr, hp->h_addr_list[0],hp->h_length);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(8000);
	iErrorCode = bind(sockListen, (PSOCKADDR)&sa, sizeof(sa));
	CheckSockError(iErrorCode, "bind");

	//�������ؼ��� �ڶ������ְ� �߳�
	HANDLE Thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RecvThread,0,0,0);
	Sleep(1000);
    //////////////////////////////////////////////////////////////////////////////
	//���͵�һ�����ְ�
	
	IP_HEADER ip_header;
	TCP_HEADER tcp_header;
	//���IP�ײ� һ��IP��ͷ�ĳ����Ϊ��1111������15*4��60���ֽڡ�IP��ͷ��С����Ϊ20�ֽڡ����ڱ�׼ipv4��ͷ������ֶε�ֵ�϶���20/4=5��10���ƣ�=0101��2���ƣ���
	ip_header.h_lenver = (4<<4 | sizeof(ip_header)/sizeof(unsigned long));
	//����λIP�汾�ţ�����λ�ײ�����
	ip_header.total_len=htons(sizeof(IP_HEADER)+sizeof(TCP_HEADER)); //16λ�ܳ��ȣ��ֽڣ�
	ip_header.ident=1; //16λ��ʶ
	ip_header.frag_and_flags=0; //3λ��־λ
	ip_header.ttl=128; //8λ����ʱ��TTL
	ip_header.proto=IPPROTO_TCP; //8λЭ��(TCP,UDP��)
	ip_header.checksum=0; //16λIP�ײ�У���
	ip_header.sourceIP=sa.sin_addr.s_addr;  //32λԴIP��ַ
	ip_header.destIP=dest.sin_addr.s_addr;  //32λĿ��IP��ַ
	
	//���TCP�ײ�
	tcp_header.th_sport = htons(8000);//Դ�˿ں�
	tcp_header.th_lenres = (sizeof(TCP_HEADER)/4<<4 | 0);//TCP���Ⱥͱ���λ
	tcp_header.th_win=htons(16384); 

	//���TCPα�ײ������ڼ���У��ͣ������������ͣ�
	psd_header.saddr=ip_header.sourceIP;
	psd_header.daddr=ip_header.destIP;
	psd_header.mbz=0;
	psd_header.ptcl=IPPROTO_TCP;
	psd_header.tcpl=htons(sizeof(tcp_header));

	
	printf("\n");
	printf("Scaning %s port : %d-%d\n",HOST,BeginPort,EndPort);
	clock_t start,end;//�������е���ʼ�ͽ���ʱ��

	  start=clock();//��ʼ��ʱ
	//��ʼ����~~~~
	  char SendBuf[128] = {0};
	for (;BeginPort < EndPort;BeginPort++)
	{
		// ������ 
		progressbar();
		
		tcp_header.th_dport = htons(BeginPort); //Ŀ�Ķ˿ں�
		tcp_header.th_ack=0;                  //ACK���к���Ϊ0
		tcp_header.th_flag=2;                 //SYN ��־
		tcp_header.th_seq=htonl(SEQ);         //SYN���к�
		tcp_header.th_urp=0;                  //ƫ��
		tcp_header.th_sum=0;                  //У���
		
		
		
		//����TCPУ��� ��TCPͷ����TCP���ݽ���У��ͼ��㣬����Ŀ��˽�����֤��
		memcpy(SendBuf,&psd_header,sizeof(psd_header));
		memcpy(SendBuf+sizeof(psd_header), &tcp_header,sizeof(tcp_header));
		tcp_header.th_sum = CalcCheckSum((USHORT *)SendBuf,sizeof(psd_header)+sizeof(tcp_header));
		
		//����IPУ��� IP��ͷ�Ǳ䳤�ģ������ṩһ��ͷ��У������֤IP��ͷ����Ϣ����ȷ�ԡ�
		memcpy(SendBuf,&ip_header,sizeof(ip_header));
		memcpy(SendBuf+sizeof(ip_header),&tcp_header,sizeof(tcp_header));
		memset(SendBuf+sizeof(ip_header)+sizeof(tcp_header),0,4);
		ip_header.checksum = CalcCheckSum((USHORT *)SendBuf,sizeof(ip_header)+sizeof(tcp_header));

		//��䷢�ͻ�����
		memcpy(SendBuf,&ip_header,sizeof(ip_header));

		//����TCP����
		iErrorCode=sendto(sockRaw,SendBuf,sizeof(ip_header)+sizeof(tcp_header),0,\
			(struct sockaddr*) &dest,
			sizeof(dest));
		CheckSockError(iErrorCode, "sendto()");
	}
	//��������~~~~
	end=clock();//��ʱ����
	StopScan = TRUE;
	printf("Closeing Scan.....\n");
	WaitForSingleObject(Thread,5000);
	CloseHandle(Thread);

	printf("Cost time: %f Sec",(float)(end-start) / CLOCKS_PER_SEC/*1000*/);
	
	if (sockRaw != INVALID_SOCKET)
	{
		closesocket(sockRaw);
	}
	if (sockListen!= INVALID_SOCKET)
	{
		closesocket(sockListen);
	}
	WSACleanup();
	return 0;
}

