#pragma once
#include"Global.h"

class CDBEntity
{

private:
	CString dbName;
	bool dbType;
	CString fileName;
	CString createTime;

public:
	CDBEntity(CString dbname, bool dbtype, CString filename);
	CDBEntity(CString dbname, bool dbtype, CString filename, CString createTime);
	~CDBEntity(void);

	bool GetdbType() { return dbType; };
	CString GetdbName() { return dbName; };
	CString GetfileName() { return fileName; };
	CString GetcreateTime() { return createTime; };

	void SetdbName(CString dbname) { dbName = dbname; };
	void SetType(bool dbtype) { dbType = dbtype; };
	void SetFileName(CString filename) { fileName = filename; };
	void SetCreateTime(CString createtime) { createTime = createtime; };
};



