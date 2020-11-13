#pragma once
#include "Global.h"

class CTableEntity
{
private:
	CString TableName;//±í¸ñÃû³Æ
	CString DBName;
	int Record_num;
	int Field_num;
	CString tdf;
	CString tic;
	CString trd;
	//CString tid;
	CString CrTime;
	CString MTime;

public:
	CTableEntity(CString tablename, CString dbName);
	CTableEntity();
	~CTableEntity();

	CString getTableName() { return TableName; };
	CString getDBName() { return DBName; };
	int getRecord_num() { return Record_num; };
	int getField_num() { return Field_num; };
	CString gettdf() { return tdf; };
	CString gettic() { return tic; };
	CString gettrd() { return trd; };
	CString gettid() { return _T("data/") + DBName + _T("/") + TableName + _T(".tid"); };
	CString getCreateTime() { return CrTime; };
	CString getModifyTime() { return MTime; };

	void SetTableName(CString tablename) { TableName = tablename; };
	void SetDBName(CString dbName) { DBName = dbName; };
	void SetFieldNum(int field_num) { Field_num = field_num; };
	void SetRecordNum(int record_num) { Record_num = record_num; };
	void SetCreateTime(CString crtTime) { CrTime = crtTime; };
	void SetModifyTime(CString modifyTime) { MTime = modifyTime; };
	void Settdf(CString tdfpath) { tdf = tdfpath; };
	void Settrd(CString trdpath) { trd = trdpath; };
	void Settic(CString ticpath) { tic = ticpath; };

};

