#pragma once
#include<windows.h>

struct DATAENTRY {
	char name[20];
	char surname[20];
	char city[20];
	int user_id;
	int department_id;
	int sex;
	int age;
	int date;
	int month;
	int year;
	
};

struct ATTRIBUTES
{
	char name[20];
	int user_id;
	int department_id;
};



#define ID_ETNAME       101
#define ID_ETADDRESS    102
#define ID_ETAGE        103
#define ID_ETSALRS      104
#define ID_ETSALPS      105
#define ID_MALE         106
#define ID_FEMALE       107
#define ID_PBCONTINUE   108
#define ID_LB           109
#define ID_TEXT         110
#define ID_DATEOFBIRTH  111
#define ID_DATEOFJOIN   112
#define ID_USERID       113
#define ID_DEPARTMENT   114
#define ID_ETSURNAME    115
#define ID_OK           116
#define ID_SPOTMESSAGE  117
#define ID_DD           118
#define ID_MM           119
#define ID_YYYY         120
struct RESULTSET
{
	DATAENTRY ResultSet[20];
	int dataEntry_counter;
};
class IReadRecordFile : public IUnknown
{
public:
	virtual HRESULT __stdcall ReadRecord(void *, void *) = 0;
	virtual HRESULT __stdcall SpotRead(void*, void*) = 0;
};

class IWriteRecordFIle : public IUnknown
{
public:
	virtual HRESULT __stdcall WriteRecord(void*, BOOL *) = 0;
};



//{F1CF53D8-CD6D-4F59-BF6F-BC91B41359CA}
const CLSID CLSID_ReadWrite = { 0xf1cf53d8, 0xcd6d, 0x4f59, 0xbf, 0x6f, 0xbc, 0x91, 0xb4, 0x13, 0x59, 0xca };

//{1281C8FD-2387-49DF-B873-D309BD3C2D55}
const IID IID_IRead = { 0x1281c8fd, 0x2387, 0x49df, 0xb8, 0x73, 0xd3, 0x9, 0xbd, 0x3c, 0x2d, 0x55 };

//{E977105C-4106-447D-9938-918C2F77A896}
const IID IID_IWrite = { 0xe977105c, 0x4106, 0x447d, 0x99, 0x38, 0x91, 0x8c, 0x2f, 0x77, 0xa8, 0x96 };



