#pragma once
#include <map>
#include "Global.h"
using namespace std;

class CRecordEntity
{
private:
	map<CString, CString> m_mValues;
public:
	CRecordEntity() {};
	~CRecordEntity() {};

	CString GetValue(CString fieldName) { return m_mValues[fieldName]; };
	map<CString, CString> GetValues() { return m_mValues; };
	void SetValue(CString fieldName, CString value) { m_mValues[fieldName] = value; };
};
