#pragma once
#include"CRecordEntity.h"

class CRecordDao
{
public:
	CRecordDao() {};
	~CRecordDao() {};

	static vector<CRecordEntity> getRecordList(CString dbname,CString tbname);//��ȡȫ����¼
	
};

