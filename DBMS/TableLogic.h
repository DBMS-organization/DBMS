#pragma once
#include "Global.h"


class CTableLogic
{
private:
	CString tbPath;//tb文件路径
	CString DBName;//数据库名

public:
	CTableLogic(CString dbName);//初始化表逻辑层
	~CTableLogic();

	int CreateTable(CString& tablename);//根据表名创建一张新表
	int DeleteTable(CString tablename);//删除表
};

