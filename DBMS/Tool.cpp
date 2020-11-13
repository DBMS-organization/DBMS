#include "pch.h"
#include "Tool.h"
#include "Global.h"

const char* CTool::CStringToChar(CString str)
{
	string s;
	const char* ch;
	s = CT2A(str.GetString());
	return s.c_str();
}

char* CTool::boolToChar(bool bl)
{
	char ch[2];
	ch[1] = '\0';
	if (bl) ch[0] = '1';
	else ch[0] = '0';
	return ch;
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

int CTool::dataType(CString tpname) {
	if (tpname == _T("BOOL")) {
		return 1;
	}
	else if (tpname == _T("DATETIME")) {
		return 2;
	}
	else if (tpname == _T("DOUBLE")) {
		return 3;
	}
	else if (tpname == _T("INTEGER")) {
		return 4;
	}
	else if (tpname == _T("VARCHAR")) {
		return 5;
	}
	return 0;
}