// test2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include<string>
using namespace std;
void print()
{
	system( "CLS" );
	system( "title C++ �ػ�����" );//����cmd���ڱ���
	system( "mode con cols=52 lines=17" );//���ڿ�ȸ߶�
	system( "color 8f" );
	cout<< " �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[\n"\
		<< " �U			C++ �ػ�����\n"\
		<< " �U		   	�����룺\n"\
		<< " �U1.ʵ�ֶ�ʱ�رռ����\n"\
		<< " �U2.�����رռ����\n"\
		<< " �U3.ע�������\n"\
		<< " �U4.�������������\n"\
		<< " �U5.ȡ���ػ���ע������(�������½���ʾ)\n"\
		<< " �U0.�˳�ϵͳ(��Ȼֱ�ӹر���һ����)\n"\
		<< " �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a\n";
}
int main()
{
	string c;
	char cmds[ 25 ] = "shutdown -s -t 000000000";
	print();
	cin >> c;
	while( c != "0" )
	{ 
		while( c == "1" )
		{
			int num;
			cout << "Please enter the time: " << endl;cin >> num;
			for( int i = 0; i < 9; i++ )
			{
				cmds[ 23 - i ] = static_cast< char >( num % 10 + 48 );
				num = num / 10;
			}
			system( cmds );
			break;
		}
		while( c == "2" )
		{
			system( "shutdown -p" );
			break;
		}
		while( c == "3" )
		{
			system( "shutdown -l" );
			break;
		}
		while( c == "4" )
		{
			system( "shutdown -r -t 0" );
			break;
		}
		while( c == "5" )
		{
			system( "shutdown -a" );
			break;
		}
		print();
		cin >> c;
	}
	return 0;
}
