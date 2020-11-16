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
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath= _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");
	
	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	//CTableEntity tb(tablename, dbname);

		//tb.SetRecordNum();

	int fieldnum = fieldlist.size();
	int recordnum = recordlist.size();

	for (vector<CFieldEntity>::iterator fieldite = fieldlist.begin(); fieldite != fieldlist.end(); ++fieldite) {
		//判断所有字段中是否唯一
		if (fieldite->GetUnique()) {
			for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
				//判断同一字段下是否存在相同的值
				if (record.GetValue(fieldite->GetFieldName()) == recordite->GetValue(fieldite->GetFieldName())) {
					return 0;
				}
			}
		}
	}
	//在.tb文件中增加记录数
	CRecordDao::AddRecordNum(dbname, tablename);
	ofstream outfile(trdFilePath, ios::binary, ios::app);

	for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
		/*fieldName = ite_1->GetFieldName();*/
		if (ite_1->GetFieldType() == TYPE_BOOL) {
			bool tempbool;
			outfile.write()
		}
		else if (ite_1->GetFieldType() == TYPE_DATETIME) {
			char* tempTime = new char[20];
			
		}
		else if (ite_1->GetFieldType() == TYPE_DOUBLE) {			///未实现
			double tempDouble;
			
		}
		else if (ite_1->GetFieldType() == TYPE_INTEGER) {
			int tempInt;
			
		}
		else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
			int varcharSize = 0;
			
		}

		recordEntity.SetValue(fieldName, fieldValue);
		recordList.push_back(recordEntity);
	}

	return 1;
}

