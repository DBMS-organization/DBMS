#include "pch.h"
#include "Tool.h"
#include "Global.h"
#include "regex"


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

void CTool::CStringToChar(CString str,char*c)
{
	string s;
	const char* ch=new char[sizeof(s)];
	s = CT2A(str.GetString());
	ch = s.c_str();
	c = const_cast<char*>(ch);
}

CString CTool::BoolToCString(bool bl)
{
	CString s;
	if (bl) s = "ture";
	else s = "false";
	return s;
}

//int to char*
const char* CTool::IntToChar(int i)
{
	const char* c;
	string s;
	s = std::to_string(i);
	c = s.c_str();
	return c;
}

int CTool::CStringToInt(CString str)
{
	return atoi(CT2A(str.GetBuffer()));
}

bool CTool::CStringToBool(CString str)
{
	if (strcmp(CT2A(str.GetBuffer()), "false")==0)
		return false;
	else
		return true;
}

double CTool::CStringToDouble(CString str)
{
	return atof(CT2A(str.GetBuffer()));
}

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

CString CTool::IntToCString(int num)
{
	CString s;
	s.Format(_T("%d"), num);
	return s;
}

//判断是否为time数据类型
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



	/*regex r1("^(((1[6-9]|[2-9]\\d)\\d{2})-(0?[13578]|1[02])-(0?[1-9]|[12]\\d|3[01]))|(((1[6-9]|[2-9]\\d)\\d{2})-(0?[13456789]|1[012])-(0?[1-9]|[12]\\d|30))|(((1[6-9]|[2-9]\\d)\\d{2})-0?2-(0?[1-9]|1\\d|2[0-8]))|((((1[6-9]|[2-9]\\d)(0[48]|[2468][048]|[13579][26]))|((16|[2468][048]|[3579][26])00))-0?2-29)");
	regex r2(" (20|21|22|23|[0-1]?\\d):[0-5]?\\d:[0-5]?\\d$");
	string s = CT2A(time.GetString());
	string s1 = s.substr(0,10);
	string s2 = s.substr(10,9);
	if (regex_match(s1, r1) && regex_match(s2, r2))
		return true;
	else return false;*/




	/*
	regex r("\d\d\d\d-\d\d-\d\d\s\d\d:\d\d:\d\d");
	string s = CT2A(time.GetString());
	if(!regex_match(s, r)) return false;
	else {
		string s1 = s.substr(0,10);
		regex r1("((([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})(((0[13578]|1[02])(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)(0[1-9]|[12][0-9]|30))|(02(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|((0[48]|[2468][048]|[3579][26])00))0229))|((([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})-(((0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)-(0[1-9]|[12][0-9]|30))|(02-(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|((0[48]|[2468][048]|[3579][26])00))-02-29))$");
		if (!regex_match(s1, r1))return false;
		else {
			string s2 = s.substr(12,8);
			regex r2("");
			return regex_match(s2,r2);
		}
	}*/
}

//判断是否为int类型
bool CTool::isInt(CString i)
{
	if (i != i.SpanIncluding(L"1234567890"))
		return false;
	else
		return true;
}

//判断是否为double类型
bool CTool::isDouble(CString d)
{
	if (isInt(d)) {
		return true;
	}
	else{
		//regex r("\d+\.\d+");
		regex r("-[0-9]+(.[0-9]+)?|[0-9]+(.[0-9]+)?");
		
	    string s = CT2A(d.GetString());
     	return regex_match(s,r);
	}
}

//判断是否为bool类型
bool CTool::isBool(CString b)
{
	if (b == _T("")) return true;

	if (strcmp(CStringToChar(b), "true") == 0 ||
		strcmp(CStringToChar(b), "false") == 0)
		return true;
	else
		return false;
}

/*判断content的数据类型是否为type
* 若type为varchar则会判断varchar的长度是否能够存储所给的content
*/
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

//判断文件名是否合法
bool CTool::isValidFileName(CString filename) 
{
	if (filename!= filename.SpanExcluding(L"|\\/:*?<>|\""))
		return false;
	else
		return true;
}

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

CString CTool::GetCurrTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	return t.Format("%Y-%m-%d %H:%M:%S");
}

//删除文件夹及所有子文件
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

