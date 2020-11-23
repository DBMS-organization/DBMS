#pragma once

#include"Global.h"
#include"CDBEntity.h"

class CDBDao
{

public:
	static vector<CDBEntity> getDatabasesList(CString& sysFilepath);//从.db文件读取数据库列表
};

