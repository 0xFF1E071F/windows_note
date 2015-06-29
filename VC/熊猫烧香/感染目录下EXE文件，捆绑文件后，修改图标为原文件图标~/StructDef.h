#pragma once

typedef struct ICONRESDIR { 
	BYTE Width; 
	BYTE Height; 
	BYTE ColorCount; 
	BYTE reserved; 
} ICONRESDIR;  //ͼ��ṹ

typedef struct tagRESDIR { 
	ICONRESDIR   Icon; 
	WORD    Planes; 
	WORD    BitCount; 
	DWORD   BytesInRes; 
	WORD  IconCursorId; 
} RESDIR; 

typedef struct NEWHEADER { 
	WORD Reserved; 
	WORD ResType; 
	WORD ResCount; 
} NEWHEADER, *PNEWHEADER; 

typedef struct {
	const RESDIR* pcResDir;
	BYTE* pMatchIcon;
} my_enum_res_callback_data;

