#pragma once
#include <map>
#include "Global.h"
using namespace std;

class CRecordEntity
{
private:
	map<CString, CString> m_mValues;//储存字段对应的记录值
public:
	CRecordEntity() {};
	~CRecordEntity() {};

	CString GetValue(CString fieldName) { return m_mValues[fieldName]; };//获取字段对应的记录值
	map<CString, CString> GetValues() { return m_mValues; };//返回所有的字段对应值
	void SetValue(CString fieldName, CString value) { m_mValues[fieldName] = value; };//设置字段对应的记录值
};
