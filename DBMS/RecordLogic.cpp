#include "pch.h"


CRecordLogic::CRecordLogic()
{
	
}

CRecordLogic::~CRecordLogic()
{
}

int CRecordLogic::AddRecord(CString dbname, CString tablename, CRecordEntity &record)
{
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath= _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");
	
	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

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
		if (fieldite->GetNotNull()) {
			if (record.GetValue(fieldite->GetFieldName()) == _T("")) {
				return 0;
			}
		}
	}
	_cprintf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
	ofstream outfile(trdFilePath, ios::binary | ios::app);
	
	for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
		CString fieldName = ite_1->GetFieldName();
		CString recordvalue = record.GetValue(fieldName);
		if (ite_1->GetFieldType() == TYPE_BOOL) {
			bool tempbool;
			tempbool = CTool::CStringToBool(recordvalue);
			outfile.write((char*)(&tempbool), sizeof(bool));
		}
		else if (ite_1->GetFieldType() == TYPE_DATETIME) {
			string tempTime;
			tempTime = CT2A(recordvalue.GetString());
			outfile.write(tempTime.c_str(), CTool::getTypeStoreLength(_T("DATETIME")));
		}
		else if (ite_1->GetFieldType() == TYPE_DOUBLE) {
			double tempDouble;
			tempDouble = CTool::CStringToDouble(recordvalue);
			outfile.write((char*)(&tempDouble), sizeof(double));
		}
		else if (ite_1->GetFieldType() == TYPE_INTEGER) {
			int tempInt = CTool::CStringToInt(recordvalue);
			outfile.write((char*)(&tempInt), sizeof(int));
		}
		else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
			int varcharSize = recordvalue.GetLength()+1;
			string strtemp = CT2A(recordvalue.GetString());
			//char* writedvarchar = new char[varcharSize + 1];
			outfile.write((char*)(&varcharSize), sizeof(int));

			//writedvarchar
			outfile.write(strtemp.c_str(), varcharSize);
		}
	}
	outfile.close();

	//在.tb文件中增加记录数
	CRecordDao::AddRecordNum(dbname, tablename);

	return 1;
}

