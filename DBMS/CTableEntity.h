#pragma once
#include "Global.h"

class CTableEntity
{
private:
	CString TableName;//������
	CString DBName;//���ݿ�����
	int Record_num;//��¼����
	int Field_num;//�ֶ�����
	CString tdf;//tdf�ļ�·��
	CString tic;//tic�ļ�·��
	CString trd;//trd�ļ�·��
	//CString tid;
	CString CrTime;//����ʱ��
	CString MTime;//�޸�ʱ��

public:
	CTableEntity(CString tablename, CString dbName);//��ʼ����ʵ��
	CTableEntity();
	~CTableEntity();

	CString getTableName() { return TableName; };//��ȡ����
	CString getDBName() { return DBName; };//��ȡ���ݿ���
	int getRecord_num() { return Record_num; };//��ȡ��¼��
	int getField_num() { return Field_num; };//��ȡ�ֶ���
	CString gettdf() { return tdf; };//��ȡtdf�ļ�·��
	CString gettic() { return tic; };//��ȡtic�ļ�·��
	CString gettrd() { return trd; };//��ȡtrd�ļ�·��
	CString gettid() { return _T("data/") + DBName + _T("/") + TableName + _T(".tid"); };
	CString getCreateTime() { return CrTime; };//��ȡ����ʱ��
	CString getModifyTime() { return MTime; };//��ȡ�޸�ʱ��

	void SetTableName(CString tablename) { TableName = tablename; };//���ñ���
	void SetDBName(CString dbName) { DBName = dbName; };//�����ݿ���
	void SetFieldNum(int field_num) { Field_num = field_num; };//�����ֶ���
	void SetRecordNum(int record_num) { Record_num = record_num; };//���ü�¼��
	void SetCreateTime(CString crtTime) { CrTime = crtTime; };//���ô���ʱ��
	void SetModifyTime(CString modifyTime) { MTime = modifyTime; };//�����޸�ʱ��
	void Settdf(CString tdfpath) { tdf = tdfpath; };//����tdf�ļ�·��
	void Settrd(CString trdpath) { trd = trdpath; };//����trd�ļ�·��
	void Settic(CString ticpath) { tic = ticpath; };//����tic�ļ�·��

};

