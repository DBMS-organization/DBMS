#pragma once
class CRecordLogic
{
private:
	CString trdFilePath;
	CString tdfFilePath;
	CString tbFilePath;
	CString dbName;
	CString tbName;

public:
	CRecordLogic();
	~CRecordLogic();

	int AddRecord(CString dbname, CString tablename, CRecordEntity& record);
	int DeleteRecord(CString dbname, CString tablename, CString fieldname, CString fieldvalue);
	int CRecordLogic::AlterRecord(CString dbname, CString tablename, CString primarykey, CString primarykeyValue, CString fieldname, CString fieldvalue);
};

