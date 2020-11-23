#pragma once
class CRecordLogic
{
private:
	CString trdFilePath;//trd文件路径
	CString tdfFilePath;//tdf文件路径
	CString tbFilePath;//tb文件路径
	CString dbName;//数据库名
	CString tbName;//表名

public:
	CRecordLogic();
	~CRecordLogic();

	int AddRecord(CString dbname, CString tablename, CRecordEntity& record);//添加记录
	int DeleteRecord(CString dbname, CString tablename, CString fieldname, CString fieldvalue);//删除记录
	int AlterRecord(CString dbname, CString tablename, CString primarykey, CString primarykeyValue, CString fieldname, CString fieldvalue);//修改记录
};

