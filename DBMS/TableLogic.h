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

	int CreateTable(CString& tablename);//���ݱ�������һ���±�
};

