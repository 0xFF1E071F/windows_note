#include <Windows.h>
#include <string>
#include <iostream>
/*#using namespace std;*/
int SubPoint(_comm_SubPaPoint &req) ;

int SubPoint(_comm_SubPaPoint &req) 
{ 
	int err; 
	AnsiString strxml,strtem; 
	char buff[4096]={0}; 

	//��һ�Σ� ����ͨѶͨ����������֤����� 
	char path[128]={0}; 
	GetCurrentDirectory(127,path); 

	// �ڶ��Σ�����ͨѶ��/*����xml begin*/ 
	CoInitialize(NULL); 
	{ 
		_di_IXMLDocument XMLDoc; 
		try 
		{ 
			XMLDoc = LoadXMLDocument((String)path+"\\ssl\\EKA1.xml"); 
		} 
		catch(Exception &exception) 
		{ 
			TraceLog(LogLevel_4,999,"xml�ļ�����ʧ��!",path); 
			req.iRet = -1; 
			return -1; 
		} 
		if(req.iRet == 0) 
		{ 
			XMLDoc->Active=true; 
			_di_IXMLNode root = XMLDoc->DocumentElement; /*XML���ڵ�*/ 
			if(root->HasChildNodes) /*�Ƿ����ӽڵ�*/ 
			{ 
				_di_IXMLNode item; 
				_di_IXMLNodeList list = root->ChildNodes; 
				item = list->FindNode(AnsiString("Request")); 
				list = item->ChildNodes; 
				item = list->FindNode(AnsiString("TerminalID")); 
				item->Text=req.cpTerminalTel; 
				item = list->FindNode(AnsiString("FirmID")); 
				item->Text=req.cpCompanyID; 
				item = list->FindNode(AnsiString("StoreID")); 
				item->Text=req.cpSID; 
				item = list->FindNode(AnsiString("Operation")); 
				item->Text="Redemption"; 
				item = list->FindNode(AnsiString("TTNumber")); 
				item->Text=req.cpOrderID; 
				char datestr[30]={0}; 
				{ 
					long i; 
					time(&i); 
					struct tm * tim=localtime(&i); 
					sprintf(datestr,"%02d/%02d/%04d %02d:%02d:%2d",tim->tm_mon+1,tim->tm_mday, 
						tim->tm_year+1900,tim->tm_hour,tim->tm_min,tim->tm_sec); 
				} 
				item = list->FindNode(AnsiString("TTTime")); 
				item->Text=datestr; 
				item = list->FindNode(AnsiString("CardNumber")); 
				item->Text=req.cpCardNo; 
				item = list->FindNode(AnsiString("Amount")); 
				item->NodeValue=req.dwPointA/100; 
			} 
			XMLDoc->SaveToXML(strxml); 
			strcpy(buff,strxml.c_str()); 
		} 

		sprintf(m_buff,"POST /invoke/wm.tn/receive? HTTP/1.0\r\n" 
			"Content-Length: %d\r\n" 
			"Content-Type: text/xml\r\n\r\n",strlen(strxml.c_str())); 
		strcat(m_buff,buff); 

		//pos���������ڴ˵���httpsͨ�� 
		printf("%s",m_buff); 
		DoTask(); 
		printf("%s",Utf8ToAnsi(m_buff)); 

		//�ҵ�xml�� 
		char *p=strstr(m_buff," <?xml version="); 
		//��utf8����ת��Ϊansistring�ַ��� 
		strxml = Utf8ToAnsi(p); 

		/*����XML begin*/ 
		_di_IXMLDocument xml = NewXMLDocument(); 
		xml->LoadFromXML(strxml); /*��һ��xml�ִ�����,Ҳ����LoadFromFile���ļ�����*/ 
		_di_IXMLNode root = xml->DocumentElement; /*XML���ڵ�*/ 
		if(root->HasChildNodes) /*�Ƿ����ӽڵ�*/ 
		{ 
			_di_IXMLNode item; 
			_di_IXMLNodeList list = root->ChildNodes; 
			item = list->FindNode(AnsiString("Message")); /*������ΪUserName���ӽڵ�*/ 
			list = item->ChildNodes; 
			item = list->FindNode(AnsiString("ErrorCode")); 
			//if(item &&!item->Text.IsEmpty()) 
			{ 
				req.iRet = item->NodeValue; 
			} 
			item = list->FindNode(AnsiString("ErrorMessage")); 
			//if(item &&!item->Text.IsEmpty()) 
			{ 
				strtem = item->NodeValue; 
				strcpy(req.cpErro,strtem.c_str()); 
			} 
			if(req.iRet == 0) 
			{ 
				//����ҵ������ 
				list = root->ChildNodes; 
				item = list->FindNode(AnsiString("Response")); /*������ΪUserName���ӽڵ�*/ 
				list = item->ChildNodes; 
				item = list->FindNode(AnsiString("PointsLeft")); 
				//if(item &&!item->Text.IsEmpty()) 
				{ 
					req.dwPointA = item->NodeValue; 
				} 
				item = list->FindNode(AnsiString("STNumber")); 
				//if(item &&!item->Text.IsEmpty()) 
				{ 
					strtem = item->NodeValue; 
					strcpy(req.cpOrderID,strtem.c_str()); 
				} 
				item = list->FindNode(AnsiString("PointsRedemed")); 
				//if(item &&!item->Text.IsEmpty()) 
				{ 
					req.dwPointB = item->NodeValue; 
				} 
			} 
		} 
	} 
	CoUninitialize(); 
	return 0; 
}
void main()
{
	
}