#pragma once

#include"Global.h"
#include"CDBEntity.h"

class CDBDao
{

public:
	static vector<CDBEntity> getDatabasesList(CString& sysFilepath);
};

