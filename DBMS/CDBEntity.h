#pragma once
#include"Global.h"

class CDBEntity
{

private:
	CString dbName;//数据库名
	bool dbType;//数据库类型
	CString DBpath;//数据库数据文件夹路径
	CString createTime;//创建时间

public:
	CDBEntity(CString dbname, bool dbtype, CString DBpathIn);//初始化数据库实体
	CDBEntity(CString dbname, bool dbtype, CString DBpathIn, CString createTime);
	~CDBEntity(void);

	bool GetdbType() { return dbType; };//获取数据库类型
	CString GetdbName() { return dbName; };//获取数据库名
	CString GetDBpath() { return DBpath; };//获取数据库数据文件路径
	CString GetcreateTime() { return createTime; };//获取创建时间

	void SetdbName(CString dbname) { dbName = dbname; };//设置数据库名
	void SetType(bool dbtype) { dbType = dbtype; };//设置数据库类型
	void SetDBpath(CString DBpathIn) { DBpath = DBpathIn; };//设置数据库数据文件路径
	void SetCreateTime(CString createtime) { createTime = createtime; };//设置创建时间
};



