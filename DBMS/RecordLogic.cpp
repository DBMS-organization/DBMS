#include "pch.h"


CRecordLogic::CRecordLogic()
{
	
}

CRecordLogic::~CRecordLogic()
{
}

int CRecordLogic::AddRecord(CString dbname, CString tablename, CRecordEntity &record)
{
	//.tb文件记录数加1
	this->dbName = dbName;
	this->tbName = tbName;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbName + _T("\\") + tablename + _T(".trd");
	
	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	CTableEntity tb(tablename, dbname);

		//tb.SetRecordNum();

	int fieldnum = fieldlist.size();
	int recordnum;

	for (int i = 0; i < fieldnum; i++) {
		//判断所有字段中是否有主键
		if (fieldlist[i].GetPrimary()) {
			for (int j = 0; j < recordnum; j++) {
				//判断同一字段下是否存在相同的值
				if (record.GetValue(fieldlist[j].GetFieldName()) == recordlist[i].GetValue(fieldlist[j].GetFieldName())) {
					return 0;
				}
			}
			
		}
	}

	return 0;
}

int 

