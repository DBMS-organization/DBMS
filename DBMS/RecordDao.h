#pragma once
#include"CRecordEntity.h"

class CRecordDao
{
public:
	CRecordDao() {};
	~CRecordDao() {};

	static vector<CRecordEntity> getRecordList(CString dbname,CString tbname);//��ȡȫ����¼
	static void AddRecordNum(CString dbname, CString tbname);//ÿ�θı�recordnum��������д��.tb�ļ���
	static void MinusRecordNum(CString dbname, CString tbname);
	static void reWritetb(vector<CTableEntity> tablelist);
};

