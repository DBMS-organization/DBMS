#pragma once
class CRecordLogic
{
private:
	CString trdFilePath;
	CString tdfFilePath;
	CString dbName;
	CString tbName;

public:
	CRecordLogic();
	~CRecordLogic();

	int AddRecord(CString dbname, CString tablename, CRecordEntity& record);
	int 
};

