#pragma once
class CRecordLogic
{
private:
	CString trdFilePath;//trd�ļ�·��
	CString tdfFilePath;//tdf�ļ�·��
	CString tbFilePath;//tb�ļ�·��
	CString dbName;//���ݿ���
	CString tbName;//����

public:
	CRecordLogic();
	~CRecordLogic();

	int AddRecord(CString dbname, CString tablename, CRecordEntity& record);//��Ӽ�¼
	int DeleteRecord(CString dbname, CString tablename, CString fieldname, CString fieldvalue);//ɾ����¼
	int AlterRecord(CString dbname, CString tablename, CString primarykey, CString primarykeyValue, CString fieldname, CString fieldvalue);//�޸ļ�¼
};

