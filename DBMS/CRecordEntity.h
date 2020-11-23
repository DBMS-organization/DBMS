#pragma once
#include <map>
#include "Global.h"
using namespace std;

class CRecordEntity
{
private:
	map<CString, CString> m_mValues;//�����ֶζ�Ӧ�ļ�¼ֵ
public:
	CRecordEntity() {};
	~CRecordEntity() {};

	CString GetValue(CString fieldName) { return m_mValues[fieldName]; };//��ȡ�ֶζ�Ӧ�ļ�¼ֵ
	map<CString, CString> GetValues() { return m_mValues; };//�������е��ֶζ�Ӧֵ
	void SetValue(CString fieldName, CString value) { m_mValues[fieldName] = value; };//�����ֶζ�Ӧ�ļ�¼ֵ
};
