 // ownshell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib,"Ws2_32")

#define OWN_KEY		346				//�Լ�����Կ
#define MAXDATASIZE 1024				//ÿ�ο��Խ��յ�����ֽ� 
#define OWN_SIGN	"scuclark\n"

//�Զ�������Э��ṹ��
typedef struct conninfo 
{
	int go;
	int come;
	char a;
} * Pconninfo;

BOOL connestab(SOCKET server);
int AverageRandom(int min,int max);

int main(int argc, char *argv[]) 
{ 
	SOCKET sockfd;
	char * recvbufp, * sendbufp;
	int numbytes, state,readn=0,sendn=0,recvn=0; 
	int istty;
	fd_set FdRead, ding;
	char buf[OWNDATASIZE],bufin[OWNDATASIZE]; 
	struct sockaddr_in their_addr;	/* �Է��ĵ�ַ�˿���Ϣ	 */ 
	if (argc != 3) 
	{ 
		//��Ҫ�з����ip����
		fprintf(stderr,"�÷�: ownshell.exe ip port\n���������Լ���ľ��\nPower by ѩɽ"); 
		exit(1); 
	} 
	
	printf("���ӵ� %s �˿�: %s .....\n",argv[1],argv[2]);

	WSADATA ws;
	WSAStartup(MAKEWORD(2,2),&ws);	//��ʼ��Windows Socket Dll
	
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//���ӶԷ�
	their_addr.sin_family = AF_INET;											/* Э��������INET	*/ 
	their_addr.sin_port = htons((unsigned short)atol(argv[2]));					/* ���ӶԷ�830�˿�	*/ 
	their_addr.sin_addr.s_addr = inet_addr(argv[1]);							/* ���ӶԷ���IP	*/ 
	state=connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr));
	
	if(state!=0)
	{
		printf("���� %s:%sʧ��!!\n",argv[1],argv[2]);
		WSACleanup();
		exit(1);
	}else
		system("cls");

	printf("������ %s:%s ��������......\n",argv[1],argv[2]);
	if(connestab(sockfd)==FALSE){
		printf("�� %s:%s ����ʧ��!!\n",argv[1],argv[2]);
		closesocket(sockfd);
		WSACleanup();
		exit(1);
	}
	printf("��ϲ,�� %s:%s ���ֳɹ�!!\n",argv[1],argv[2]);
	
	struct timeval TimeOut; 
	TimeOut.tv_sec=0; 
	TimeOut.tv_usec=1000; 
	FD_ZERO(&ding); 
	FD_ZERO(&FdRead);
	FD_SET (sockfd, &ding);

	while (FD_ISSET(sockfd,&ding))
	{
		//����ʼֵ
		FdRead=ding;

		//��ʱ�����
		int Er=select(sockfd+1, &FdRead, 0, 0, &TimeOut);
		if( (Er==SOCKET_ERROR))
		{
			printf("select socket err!\n",Er);
			FD_CLR(sockfd,&FdRead);
			FD_CLR (sockfd, &ding);
		}
		if( FD_ISSET(sockfd,&FdRead))
		{
				numbytes=recv(sockfd, buf, sizeof(buf), 0);
				if(numbytes == SOCKET_ERROR || numbytes==0)
				{
					FD_CLR (sockfd, &ding);
				}		//�ɶ����Ѿ��ر�����
				else
				{
					recvn=numbytes;
					buf[numbytes] = '\0';
					recvbufp= buf;
				}
		}
		
		if(sendn)goto print;	//����Ƿ������ݻ�δ����
		
		if (kbhit()) //����ն����Ƿ�������,���������
		{
			gets(bufin);
			strcat(bufin, "\n");
			readn = strlen(bufin);
			if(readn>0)
			{
				sendn=readn;
				bufin[readn]='\0';
				sendbufp=bufin;
			}
		}	

print:
		
		if(recvn)
		{	
			int rr= write (1, recvbufp, recvn);
			recvbufp+=rr;
			recvn-=rr;
		}

		if(sendn)
		{
			readn=send(sockfd,sendbufp,sendn,0);
			sendbufp+=readn;
			sendn-=readn;
		}
		if(recvn || sendn )goto print;
		Sleep(10);
		
	}
	printf("�� %s �Ͽ�����!!\n",argv[1]);
	closesocket(sockfd); 
	WSACleanup();
	return 0; 
} 

BOOL connestab(SOCKET server)
{
	int count;
	conninfo info;
	Pconninfo infop=&info;
	char buf[256],cmd[5];
	srand( (unsigned)time( NULL ) );
	info.a='\n';
	send(server,OWN_SIGN,strlen(OWN_SIGN),NULL);
	count=0;
	while(count<256)
		{ 
			if(recv(server,cmd,1,0)==SOCKET_ERROR)
			{
				closesocket(server); 
				return FALSE;
			}
			buf[count]=cmd[0]; 
			if(cmd[0]==0xa || cmd[0]==0xd) 
			{ 
				buf[count]=0; 
				break; 
			} 
			count++; 
		} 
	if( ((Pconninfo)buf) ->come!=68)return FALSE;
	info.come=((Pconninfo)buf)->go%OWN_KEY;
	info.go=AverageRandom(100000,999999);
	send(server,(char *)infop,sizeof(info)-3,NULL);
	count=0;
	while(count<256)
		{ 
			if(recv(server,cmd,1,0)==SOCKET_ERROR)
			{
				closesocket(server); 
				return FALSE;
			}
			buf[count]=cmd[0]; 
			if(cmd[0]==0xa || cmd[0]==0xd) 
			{ 
				buf[count]=0; 
				break; 
			} 
			count++; 
		} 
	if( ((Pconninfo)buf)->come!=info.go%OWN_KEY && ((Pconninfo)buf)->go!='o'+'k')
		return FALSE;
	return TRUE;
}

int AverageRandom(int min,int max)
        {
        int minInteger = (int)(min*10000);
        int maxInteger = (int)(max*10000);
        int randInteger = rand()*rand();
        int diffInteger = maxInteger - minInteger;
        int resultInteger = randInteger % diffInteger + minInteger;
        return resultInteger/10000;
        }														//���������
