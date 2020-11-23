#include "pch.h"
#include "CTableEntity.h"

//初始化表实体
CTableEntity::CTableEntity(CString tablename, CString dbName)
{
	this->TableName = tablename;
	this->Field_num = 0;
	this->DBName = dbName;
	this->Record_num = 0;
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	this->CrTime = t.Format("%Y-%m-%d %H:%M:%S");
	this->MTime = this->CrTime;
}

CTableEntity::CTableEntity()
{
}

CTableEntity::~CTableEntity()
{
}
