#pragma once
#include"CRecordEntity.h"

class CRecordDao
{
public:
	CRecordDao() {};
	~CRecordDao() {};

	static vector<CRecordEntity> getRecordList(CString dbname,CString tbname);//���ļ��ж�ȡ��¼�б�
	static void AddRecordNum(CString dbname, CString tbname);//��recordnum��1��������д��.tb�ļ���
	static void MinusRecordNum(CString dbname, CString tbname);//��recordnum��1��������д��.tb�ļ���
	static void reWritetb(vector<CTableEntity> tablelist);//��д.tb�ļ�
	static void reWriteRecord(CString dbname,CString tbname,vector<CRecordEntity> recordlist);//��д��¼��
};

