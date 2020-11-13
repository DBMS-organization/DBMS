#pragma once
#include "Global.h"
#include "CTableEntity.h"


class CTableDAO
{
public:
	CTableDAO();
	~CTableDAO();

	static vector<CTableEntity> getTableList(CString& tbFilePath);//从文件读取表的列表

};

