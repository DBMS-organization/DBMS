#include "pch.h"


CRecordLogic::CRecordLogic()
{
	
}

CRecordLogic::~CRecordLogic()
{
}

int CRecordLogic::AddRecord(CString dbname, CString tablename, CRecordEntity &record)
{
	//.tb�ļ���¼����1
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
		//�ж������ֶ����Ƿ�Ψһ
		if (fieldite->GetUnique()) {
			for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
				//�ж�ͬһ�ֶ����Ƿ������ͬ��ֵ
				if (record.GetValue(fieldite->GetFieldName()) == recordite->GetValue(fieldite->GetFieldName())) {
					return 0;
				}
			}
		}
	}
	//��.tb�ļ������Ӽ�¼��
	CRecordDao::AddRecordNum(dbname, tablename);



	return 1;
}

