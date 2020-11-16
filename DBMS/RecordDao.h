#pragma once
#include"CRecordEntity.h"

class CRecordDao
{
public:
	CRecordDao() {};
	~CRecordDao() {};

	static vector<CRecordEntity> getRecordList(CString dbname,CString tbname);//获取全部记录
	static void AddRecordNum(CString dbname, CString tbname);//每次改变recordnum，将其重写到.tb文件中
	static void MinusRecordNum(CString dbname, CString tbname);
	static void reWritetb(vector<CTableEntity> tablelist);
};

