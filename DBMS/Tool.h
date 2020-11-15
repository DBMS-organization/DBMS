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

	//�ж��Ƿ�Ϊtime��������
	static bool isTime(CString time);
	//�ж��Ƿ�Ϊint����
	static bool isInt(CString i);
	//�ж��Ƿ�Ϊdouble����
	static bool isDouble(CString d);
	//�ж��Ƿ�Ϊbool����
	static bool isBool(CString b);
	//�ж�content�����������Ƿ�Ϊtype
	static bool judgeType(CString type, CString content, int length = -1);

	//�ж��ļ����Ƿ�Ϸ�
	static bool isValidFileName(CString filename);

};

