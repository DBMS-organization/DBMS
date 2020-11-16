﻿#include "pch.h"
#include "Tool.h"
#include "Global.h"
#include "regex"


const char* CTool::CStringToChar(CString str)
{
	string s;
	const char* ch;
	s = CT2A(str.GetString());
	return s.c_str();
}

CString CTool::BoolToCString(bool bl)
{
	CString s;
	if (bl) s = "ture";
	else s = "false";
	return s;
}

//intתchar*
const char* CTool::IntToChar(int i)
{
	const char* c;
	string s;
	s = std::to_string(i);
	c = s.c_str();
	return c;
}

CString CTool::IntToCStrign(int)
{
	return CString();
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
	regex r("\d\d\d\d-\d\d-\d\d\s\d\d:\d\d:\d\d");
	string s = CT2A(time.GetString());
	return regex_match(s, r);
	return true;
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
		regex r("\d+\.\d+");
	    string s = CT2A(d.GetString());
     	return regex_match(s,r);
	}
}

//判断是否为bool类型
bool CTool::isBool(CString b)
{
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
			if (length != -1&& length < content.GetLength())
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
		return 20;
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
