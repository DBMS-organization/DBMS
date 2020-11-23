#pragma once
#include"CRecordEntity.h"

class CRecordDao
{
public:
	CRecordDao() {};
	~CRecordDao() {};

	static vector<CRecordEntity> getRecordList(CString dbname,CString tbname);//从文件中读取记录列表
	static void AddRecordNum(CString dbname, CString tbname);//将recordnum加1，将其重写到.tb文件中
	static void MinusRecordNum(CString dbname, CString tbname);//将recordnum减1，将其重写到.tb文件中
	static void reWritetb(vector<CTableEntity> tablelist);//重写.tb文件
	static void reWriteRecord(CString dbname,CString tbname,vector<CRecordEntity> recordlist);//重写记录表
};

