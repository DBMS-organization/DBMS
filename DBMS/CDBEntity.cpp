#include "pch.h"
#include "CDBEntity.h"

CDBEntity::CDBEntity(CString dbname, bool dbtype, CString filename)
{
	AllocConsole();
	SetdbName(dbname);
	SetType(dbtype);
	SetFileName(filename);

	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	SetCreateTime(t.Format("%Y-%m-%d %H:%M:%S"));
}

CDBEntity::CDBEntity(CString dbname, bool dbtype, CString filename, CString createTime) {
	SetdbName(dbname);
	SetType(dbtype);
	SetFileName(filename);
	SetCreateTime(createTime);
}

CDBEntity::~CDBEntity(void)
{
}

