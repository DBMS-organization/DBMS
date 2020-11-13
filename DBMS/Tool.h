#pragma once
class CTool
{
public:
	static const char* CStringToChar(CString str);
	static char* boolToChar(bool bl);
	static const char* IntToChar(int i);
	static int dataType(CString tpname);
};

