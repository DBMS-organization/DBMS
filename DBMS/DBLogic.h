#pragma once

#include "Global.h"
class CDBLogic
{
private:
	CString syspath = SYSPATH;//db文件路径
public:
	int CreateDatabase(CString& dbName);		//创建数据库
	int DeleteDatabase(CString dbName);		//删除数据库
};

