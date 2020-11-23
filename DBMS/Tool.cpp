#include "pch.h"
#include "Tool.h"
#include "Global.h"
#include "regex"

/***************************************************************************************
* 函数名    ：CStringToChar
* 参数str   : 一个CString类型的参数
* 返回值    ：返回一个char数组
* 描述      ：将CString转为char*
****************************************************************************************/
const char* CTool::CStringToChar(CString str)
{
	/*
	string s;
	const char* ch;
	s = CT2A(str.GetString());
	ch = s.c_str();
	return ch;
	*/
	//针对Unicode编码的转化方案
	USES_CONVERSION;
	return T2A(str);
}

/***************************************************************************************
* 函数名     ：CStringToChar（两个参数的转化方法）
* 参数str    :一个CString类型的参数
*     c      :一个char*类型的参数
* 返回值     ：void
* 描述       ：将CString转为char*存储到c中
****************************************************************************************/
void CTool::CStringToChar(CString str,char*c)
{
	string s;
	const char* ch=new char[sizeof(s)];
	s = CT2A(str.GetString());
	ch = s.c_str();
	c = const_cast<char*>(ch);
}

/***************************************************************************************
* 函数名    ：BoolToCString
* 参数bl    ：需要转化为CString类型的bool值
* 返回值    ：bool值转化成的CString
* 描述      ：将bool类型的bl转为CString
****************************************************************************************/
CString CTool::BoolToCString(bool bl)
{
	CString s;
	if (bl) s = "ture";
	else s = "false";
	return s;
}

/***************************************************************************************
* 函数名    ：IntToChar
* 参数i     ：需要转化为char*类型的int值
* 返回值    ：int值转化成的char*
* 描述      ：将int类型的i转为char*
****************************************************************************************/
const char* CTool::IntToChar(int i)
{
	const char* c;
	string s;
	s = std::to_string(i);
	c = s.c_str();
	return c;
}

/***************************************************************************************
* 函数名    ：CStringToInt
* 参数str   ：需要转化为int类型的CString
* 返回值    ：CString值转化成的int
* 描述      ：将CString类型的str转为int
****************************************************************************************/
int CTool::CStringToInt(CString str)
{
	return atoi(CT2A(str.GetBuffer()));
}

/***************************************************************************************
* 函数名    ：CStringToBool
* 参数str   ：需要转化为bool类型的CString
* 返回值    ：CString值转化成的bool值
* 描述      ：将CString类型的str转为bool
****************************************************************************************/
bool CTool::CStringToBool(CString str)
{
	if (strcmp(CT2A(str.GetBuffer()), "false")==0)
		return false;
	else
		return true;
}

/***************************************************************************************
* 函数名    ：CStringToDouble
* 参数str   ：需要转化为double类型的CString
* 返回值    ：CString值转化成的double
* 描述      ：将CString类型的str转为double
****************************************************************************************/
double CTool::CStringToDouble(CString str)
{
	return atof(CT2A(str.GetBuffer()));
}

/***************************************************************************************
* 函数名    ：DoubleToCString
* 参数d     ：需要转化为CString类型的double值
* 返回值    ：double值转化成的CString
* 描述      ：将double类型的d转为CString
****************************************************************************************/
CString CTool::DoubleToCString(double d)
{
	CString s;
	s.Format(_T("%f"), d);
	return s;
	/*
	char buff[128];
	sprintf_s(buff, "%lf", d);
	return CString(buff);
	*/
}

/***************************************************************************************
* 函数名    ：dataType
* 参数tpname：用于描述变量类型名的CString
* 返回值    ：int
* 描述      ：判断CString中的字符串表示的是哪一类型的参数并返回int值
*             TYPE_BOOL,TYPE_DATATIME,TYPE_DOUBLE,TYPE_INTEGER,TYPE_VARCHAR
*             分别代表五种数据类型
****************************************************************************************/
int CTool::dataType(CString tpname) {
	if (tpname == _T("BOOL")) {
		return TYPE_BOOL;
	}
	else if (tpname == _T("DATETIME")) {
		return TYPE_DATETIME;
	}
	else if (tpname == _T("DOUBLE")) {
		return TYPE_DOUBLE;
	}
	else if (tpname == _T("INTEGER")) {
		return TYPE_INTEGER;
	}
	else if (tpname == _T("VARCHAR")) {
		return TYPE_VARCHAR;
	}
	return 0;
}

/***************************************************************************************
* 函数名    ：IntTodataType
* 参数num   ：int值 TYPE_BOOL,TYPE_DATATIME,TYPE_DOUBLE,TYPE_INTEGER,TYPE_VARCHAR中的一个
* 返回值    ：用来表示数据类型的CString
* 描述      ：将TYPE_BOOL,TYPE_DATATIME,TYPE_DOUBLE,TYPE_INTEGER,TYPE_VARCHAR
              （int值）转为其数据类型所对应的CString
****************************************************************************************/
CString CTool::IntTodataType(int num)
{
	if (num == TYPE_BOOL) {
		return _T("BOOL");
	}
	else if (num == TYPE_DATETIME) {
		return _T("DATETIME");
	}
	else if (num == TYPE_DOUBLE) {
		return _T("DOUBLE");
	}
	else if (num == TYPE_INTEGER) {
		return _T("INTEGER");
	}
	else if (num == TYPE_VARCHAR) {
		return _T("VARCHAR");
	}

	return _T("");
}

/***************************************************************************************
* 函数名    ：IntToCString
* 参数num   ：需要转化为CString类型的int值
* 返回值    ：int值转化成的CString
* 描述      ：将int类型的num转为CString
****************************************************************************************/
CString CTool::IntToCString(int num)
{
	CString s;
	s.Format(_T("%d"), num);
	return s;
}

/***************************************************************************************
* 函数名    ：isTime
* 参数time  ：需要判断是否符合时间类型格式（yyyy-mm-dd）
              以及时间合法性(如："2021-02-29"非法)的CString
* 返回值    ：判断结果bool值
* 描述      ：判断时间是否符合格式及是否合法，若符合规则返回true
****************************************************************************************/
bool CTool::isTime(CString time)
{
	if (time == _T("")) return true;

	regex r1("^(((1[6-9]|[2-9]\\d)\\d{2})-(0?[13578]|1[02])-(0?[1-9]|[12]\\d|3[01]))|(((1[6-9]|[2-9]\\d)\\d{2})-(0?[13456789]|1[012])-(0?[1-9]|[12]\\d|30))|(((1[6-9]|[2-9]\\d)\\d{2})-0?2-(0?[1-9]|1\\d|2[0-8]))|((((1[6-9]|[2-9]\\d)(0[48]|[2468][048]|[13579][26]))|((16|[2468][048]|[3579][26])00))-0?2-29)");
	//regex r2(" (20|21|22|23|[0-1]?\\d):[0-5]?\\d:[0-5]?\\d$");
	string s = CT2A(time.GetString());
	string s1 = s.substr(0, 10);
	//string s2 = s.substr(10, 9);
	if (regex_match(s1, r1))
		return true;
	else return false;	
}

/***************************************************************************************
* 函数名    ：isInt
* 参数i     ：CString类型的参数
* 返回值    ：bool值
* 描述      ：判断CString类型的值i是否是整数，若是则返回true
****************************************************************************************/
bool CTool::isInt(CString i)
{
	if(i == _T("")) return true;
	/*if (i != i.SpanIncluding(L"1234567890"))
		return false;
	else
		return true;*/
	regex r("^\\+?\\d+$");
	regex r1("^-[1-9]\\d*|0$");
	string s = CT2A(i.GetString());
	if (regex_match(s, r)|| regex_match(s, r1)) {
		return true;
	}
	else {
		return false;
	}
}

/***************************************************************************************
* 函数名    ：isDouble
* 参数d     ：CString类型的参数
* 返回值    ：bool值
* 描述      ：判断CString类型的值d是否是double类型的数，若是则返回true
****************************************************************************************/
bool CTool::isDouble(CString d)
{
	if (d == _T("")) return true;

	if (isInt(d)) {
		return true;
	}
	else{
		regex r("^\\+?\\d+(\\.\\d+)?$");
		regex r1("^((-\\d+(\\.\\d+)?)|(0+(\\.0+)?))$");
	    string s = CT2A(d.GetString());

		if (regex_match(s, r) || regex_match(s, r1)) {
			return true;
		}
		else {
			return false;
		}
	}
}

/***************************************************************************************
* 函数名    ：isBool
* 参数b     ：CString类型的参数
* 返回值    ：bool值
* 描述      ：判断CString类型的值b是否是bool类型，若是则返回true
****************************************************************************************/
bool CTool::isBool(CString b)
{
	if (b == _T("")) return true;

	if (strcmp(CStringToChar(b), "true") == 0 ||
		strcmp(CStringToChar(b), "false") == 0)
		return true;
	else
		return false;
}

/***************************************************************************************
* 函数名     ：judgeType
* 参数type   ：CString类型的值
*     content: CString类型的值
*     length : int类型的值
* 返回值     ：bool类型的值
* 描述       ：判断content中的数据的数据类型是否为字符串type所表示的数据类型
*              若是则返回true
*              若type为varchar则会判断varchar的长度是否能够存储所给的content
* 如         : judgeType(_T("bool"),_T("false"),0)  "false"是"bool"类型，故返回true
****************************************************************************************/
bool CTool::judgeType(CString type, CString content, int length)
{
	bool judge = true;
	switch (dataType(type)) {
		case TYPE_BOOL:
			judge = isBool(content);;
			break;
		case TYPE_DATETIME:
			judge = isTime(content);;
			break;
		case TYPE_INTEGER:
			judge = isInt(content);;
			break;
		case TYPE_DOUBLE:
			judge = isDouble(content);;
			break;
		case TYPE_VARCHAR:
			if (length < content.GetLength())
				judge=false;
			break;
	}
	return judge;
}

/***************************************************************************************
* 函数名      ：isValidFileName
* 参数filename：需要转化为CString类型的int值
* 返回值      ：bool类型的值
* 描述        ：判断文件名是否合法，若文件名中含有|\/:*?<>|" 中的任意一个
*               则文件名非法，返回false，否则返回true
****************************************************************************************/
bool CTool::isValidFileName(CString filename) 
{
	if (filename!= filename.SpanExcluding(L"|\\/:*?<>|\""))
		return false;
	else
		return true;
}

/***************************************************************************************
* 函数名    ：getTypeStoreLength
* 参数tpname：CString类型的参数
* 返回值    ：int值
* 描述      ：返回tpname所表示的数据类型在文件中的存储长度
****************************************************************************************/
int CTool::getTypeStoreLength(CString tpname)
{
	if (tpname == _T("BOOL")) {
		return sizeof(bool);
	}
	else if (tpname == _T("DATETIME")) {
		return 11;
	}
	else if (tpname == _T("DOUBLE")) {
		return sizeof(double);
	}
	else if (tpname == _T("INTEGER")) {
		return sizeof(int);
	}
	else if (tpname == _T("VARCHAR")) {
		return TYPE_VARCHAR;
	}
	return 0;
}

/***************************************************************************************
* 函数名    ：GetCurrTime
* 参数      ：无
* 返回值    ：表示当时（系统）时间的CString
* 描述      ：获取当时的系统时间，以"%Y-%m-%d %H:%M:%S"格式的CString返回
****************************************************************************************/
CString CTool::GetCurrTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	return t.Format("%Y-%m-%d %H:%M:%S");
}

/***************************************************************************************
* 函数名        ：DeleteFolder
* 参数folderName：CString类型的参数
* 返回值        ：bool值
* 描述          ：删除文件夹及所有子文件，成功则返回true
****************************************************************************************/
bool CTool::DeleteFolder(CString& folderName)
{

	CFileFind finder;
	CString path;
	path.Format(CString("%s/*.*"), folderName);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())
		{//处理文件夹
			CTool::DeleteFolder(finder.GetFilePath()); //递归删除文件夹
			RemoveDirectory(finder.GetFilePath());
		}
		else
		{//处理文件
			DeleteFile(finder.GetFilePath());
		}
	}
	if (!RemoveDirectory(folderName))
		return false;

	return true;
}

