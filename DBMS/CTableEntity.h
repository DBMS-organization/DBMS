#pragma once
#include "Global.h"

class CTableEntity
{
private:
	CString TableName;//表名称
	CString DBName;//数据库名称
	int Record_num;//记录数量
	int Field_num;//字段数量
	CString tdf;//tdf文件路径
	CString tic;//tic文件路径
	CString trd;//trd文件路径
	//CString tid;
	CString CrTime;//创建时间
	CString MTime;//修改时间

public:
	CTableEntity(CString tablename, CString dbName);//初始化表实体
	CTableEntity();
	~CTableEntity();

	CString getTableName() { return TableName; };//获取表名
	CString getDBName() { return DBName; };//获取数据库名
	int getRecord_num() { return Record_num; };//获取记录数
	int getField_num() { return Field_num; };//获取字段数
	CString gettdf() { return tdf; };//获取tdf文件路径
	CString gettic() { return tic; };//获取tic文件路径
	CString gettrd() { return trd; };//获取trd文件路径
	CString gettid() { return _T("data/") + DBName + _T("/") + TableName + _T(".tid"); };
	CString getCreateTime() { return CrTime; };//获取创建时间
	CString getModifyTime() { return MTime; };//获取修改时间

	void SetTableName(CString tablename) { TableName = tablename; };//设置表名
	void SetDBName(CString dbName) { DBName = dbName; };//设数据库名
	void SetFieldNum(int field_num) { Field_num = field_num; };//设置字段数
	void SetRecordNum(int record_num) { Record_num = record_num; };//设置记录数
	void SetCreateTime(CString crtTime) { CrTime = crtTime; };//设置创建时间
	void SetModifyTime(CString modifyTime) { MTime = modifyTime; };//设置修改时间
	void Settdf(CString tdfpath) { tdf = tdfpath; };//设置tdf文件路径
	void Settrd(CString trdpath) { trd = trdpath; };//设置trd文件路径
	void Settic(CString ticpath) { tic = ticpath; };//设置tic文件路径

};

