#include "pch.h"
#include "CDBEntity.h"

//��ʼ�����ݿ�ʵ��
CDBEntity::CDBEntity(CString dbname, bool dbtype, CString DBpathIn)
{
	SetdbName(dbname);
	SetType(dbtype);
	SetDBpath(DBpathIn);

	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	SetCreateTime(t.Format("%Y-%m-%d %H:%M:%S"));
}
//��ʼ�����ݿ�ʵ��
CDBEntity::CDBEntity(CString dbname, bool dbtype, CString DBpathIn, CString createTime) {
	SetdbName(dbname);
	SetType(dbtype);
	SetDBpath(DBpathIn);
	SetCreateTime(createTime);
}

CDBEntity::~CDBEntity(void)
{
}

