#include "pch.h"
#include "CDBEntity.h"

//初始化数据库实体
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
//初始化数据库实体
CDBEntity::CDBEntity(CString dbname, bool dbtype, CString DBpathIn, CString createTime) {
	SetdbName(dbname);
	SetType(dbtype);
	SetDBpath(DBpathIn);
	SetCreateTime(createTime);
}

CDBEntity::~CDBEntity(void)
{
}

