#pragma once
#include"Global.h"

class CDBEntity
{

private:
	CString dbName;//���ݿ���
	bool dbType;//���ݿ�����
	CString DBpath;//���ݿ������ļ���·��
	CString createTime;//����ʱ��

public:
	CDBEntity(CString dbname, bool dbtype, CString DBpathIn);//��ʼ�����ݿ�ʵ��
	CDBEntity(CString dbname, bool dbtype, CString DBpathIn, CString createTime);
	~CDBEntity(void);

	bool GetdbType() { return dbType; };//��ȡ���ݿ�����
	CString GetdbName() { return dbName; };//��ȡ���ݿ���
	CString GetDBpath() { return DBpath; };//��ȡ���ݿ������ļ�·��
	CString GetcreateTime() { return createTime; };//��ȡ����ʱ��

	void SetdbName(CString dbname) { dbName = dbname; };//�������ݿ���
	void SetType(bool dbtype) { dbType = dbtype; };//�������ݿ�����
	void SetDBpath(CString DBpathIn) { DBpath = DBpathIn; };//�������ݿ������ļ�·��
	void SetCreateTime(CString createtime) { createTime = createtime; };//���ô���ʱ��
};



