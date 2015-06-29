#ifndef _TC_MD5_TOOL_H_
#define _TC_MD5_TOOL_H_

#include <string>

#define R_memset(x, y, z) memset(x, y, z)
#define R_memcpy(x, y, z) memcpy(x, y, z)
#define R_memcmp(x, y, z) memcmp(x, y, z) 

/* Constants for MD5Transform routine. */
/*md5ת���õ��ĳ������㷨����涨��*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21 

typedef unsigned long UINT4;
typedef unsigned char *POINTER; 

/* MD5 context. */ 
typedef struct {
	/* state (ABCD) */   
	/*�ĸ�32bits�������ڴ�����ռ���õ�����ϢժҪ������Ϣ���ȡ�512bitsʱ��Ҳ���ڴ��ÿ��512bits���м���*/ 
	UINT4 state[4];   

	/* number of bits, modulo 2^64 (lsb first) */    
	/*�洢ԭʼ��Ϣ��bits������,����������bits���Ϊ 2^64 bits����Ϊ2^64��һ��64λ�������ֵ*/
	UINT4 count[2];

	/* input buffer */ 
	/*����������Ϣ�Ļ�������512bits*/
	unsigned char buffer[64];  
} MD5_CTX; 

class TCMd5Tool
{
public:
	TCMd5Tool(void);
	virtual ~TCMd5Tool(void);

public:
	int algorithm(const char* p_pszData, const unsigned int p_nDataLen, std::string& p_strMd5);

private:
	void MD5Init(MD5_CTX *);
	void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
	void MD5Final(unsigned char [16], MD5_CTX *); 

private:
	void MD5Transform(UINT4 [4], unsigned char [64]);
	void Encode(unsigned char *, UINT4 *, unsigned int);
	void Decode(UINT4 *, unsigned char *, unsigned int); 
};


#endif //_TC_MD5_TOOL_H_
