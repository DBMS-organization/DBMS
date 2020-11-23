#pragma once
#include "Global.h"
#include "FieldEntity.h"


class CFieldDAO
{
public:
	CFieldDAO();
	~CFieldDAO();

	static vector<CFieldEntity> getFieldList(CString& tdfFilePath);//从文件读取字段的列表
};


