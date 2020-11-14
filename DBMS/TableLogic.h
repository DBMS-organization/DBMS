#pragma once
#include "Global.h"


class CTableLogic
{
private:
	CString tbPath;
	CString DBName;

public:
	CTableLogic(CString dbName);
	~CTableLogic();

	int CreateTable(CString& tablename);//根据表名创建一张新表
};

