#pragma once
#define TYPE_BOOL      1
#define TYPE_DATETIME  2
#define TYPE_DOUBLE    3
#define TYPE_INTEGER   4
#define TYPE_VARCHAR   5

class CTool
{
public:
	static const char* CStringToChar(CString str);
	static CString BoolToCString(bool bl);
	static const char* IntToChar(int i);
	static int dataType(CString tpname);
	static CString IntTodataType(int num);

	static CString IntToCString(int num);

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

};

