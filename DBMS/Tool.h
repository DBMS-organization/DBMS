#pragma once
#define TYPE_BOOL      1
#define TYPE_DATETIME  2
#define TYPE_DOUBLE    3
#define TYPE_INTEGER   4
#define TYPE_VARCHAR   5

#include "FieldEntity.h"
using namespace std;

class CTool
{
public:
	//XXXToChar()
	static const char* CStringToChar(CString str); //空指针
	static void CStringToChar(CString str,char* c);//乱码
	static const char* IntToChar(int i);//空指针

	//XXXToCString()
	static CString BoolToCString(bool bl);
	static CString DoubleToCString(double d);
	static CString IntToCString(int num);

    //dataTypeToInt&intToDataType
	static int dataType(CString tpname);
	static CString IntTodataType(int num);
		
	//判断是否为time数据类型
	static bool isTime(CString time);
	//判断是否为int类型
	static bool isInt(CString i);
	//判断是否为double类型
	static bool isDouble(CString d);
	//判断是否为bool类型
	static bool isBool(CString b);
	//判断content的数据类型是否为type
	static bool judgeType(CString type, CString content, int length = -1);
	//判断文件名是否合法
	static bool isValidFileName(CString filename);
	//获取指定数据类型的存储长度
	static int getTypeStoreLength(CString tpname);
	//获取当前时间
	static CString GetCurrTime();

};

