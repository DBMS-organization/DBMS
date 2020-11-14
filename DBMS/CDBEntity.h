#pragma once
#include"Global.h"

class CDBEntity
{

private:
	CString dbName;
	bool dbType;
	CString DBpath;//数据库文件夹路径
	CString createTime;

public:
	CDBEntity(CString dbname, bool dbtype, CString DBpathIn);
	CDBEntity(CString dbname, bool dbtype, CString DBpathIn, CString createTime);
	~CDBEntity(void);

	bool GetdbType() { return dbType; };
	CString GetdbName() { return dbName; };
	CString GetDBpath() { return DBpath; };
	CString GetcreateTime() { return createTime; };

	void SetdbName(CString dbname) { dbName = dbname; };
	void SetType(bool dbtype) { dbType = dbtype; };
	void SetDBpath(CString DBpathIn) { DBpath = DBpathIn; };
	void SetCreateTime(CString createtime) { createTime = createtime; };
};



