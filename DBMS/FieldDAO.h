#pragma once
#include "Global.h"
#include "FieldEntity.h"


class CFieldDAO
{
public:
	CFieldDAO();
	~CFieldDAO();

	static vector<CFieldEntity> getFieldList(CString& tdfFilePath);//���ļ���ȡ�ֶε��б�
};


