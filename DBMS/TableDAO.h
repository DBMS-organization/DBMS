#pragma once
#include "Global.h"
#include "CTableEntity.h"


class CTableDAO
{
public:
	CTableDAO();
	~CTableDAO();

	static vector<CTableEntity> getTableList(CString& tbFilePath);//���ļ���ȡ����б�

};

