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

int CRecordLogic::DeleteRecord(CString dbname, CString tablename, CString fieldname, CString fieldvalue)
{
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	int count=0;
	vector<CRecordEntity> newRecord;
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		if (fieldvalue.CompareNoCase(recordite->GetValue(fieldname))!=0) {
			newRecord.push_back(*(recordite));
	        //在.tb文件中减少记录数
		}else {
			CRecordDao::MinusRecordNum(dbname, tablename);
			count++;
		}
	}
	if (count == 0)return 0;

		ofstream clrfile(trdFilePath, ios::binary);
		clrfile.close();
		//写文件
		ofstream outfile(trdFilePath, ios::binary | ios::app);
		for (vector<CRecordEntity>::iterator recordite = newRecord.begin(); recordite != newRecord.end(); ++recordite) {
			for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
				CString fieldName = ite_1->GetFieldName();
				CString recordvalue = recordite->GetValue(fieldName);
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
					int varcharSize = recordvalue.GetLength() + 1;
					string strtemp = CT2A(recordvalue.GetString());
					//char* writedvarchar = new char[varcharSize + 1];
					outfile.write((char*)(&varcharSize), sizeof(int));

					//writedvarchar
					outfile.write(strtemp.c_str(), varcharSize);
				}
			}
		}
		outfile.close();
		return 1;
	
}

/// <summary>
/// 相当于alter table tablename set modifiedField="modifiedValue" where queryFiled="fitValue";
/// </summary>
/// <param name="dbname">数据库名</param>
/// <param name="tablename">表名</param>
/// <param name="queryField">用来查询的字段名</param>
/// <param name="fitValue">用来查询的字段需要满足的值</param>
/// <param name="modifiedField">需要修改的字段名</param>
/// <param name="modifiedValue">修改后该字段的值</param>
/// <returns></returns>
int CRecordLogic::AlterRecord(CString dbname, 
	                          CString tablename, 
	                          CString queryField, 
	                          CString fitValue,
	                          CString modifiedField, 
	                          CString modifiedValue)
{
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	bool isUnique;
	for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
		if (ite_1->GetFieldName().Compare(modifiedField) == 0) {
			isUnique=ite_1->GetUnique();
		}
	}
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		
		if (fitValue.CompareNoCase(recordite->GetValue(queryField))==0) {
			if (!isUnique) {
				recordite->SetValue(modifiedField,modifiedValue);
			}
			else {
				for (vector<CRecordEntity>::iterator recordite_1 = recordlist.begin(); recordite != recordlist.end(); ++recordite_1) {
					if (recordite_1->GetValue(modifiedField).Compare(modifiedValue) == 0) {
						return 0;
					}
				}
				recordite->SetValue(modifiedField, modifiedValue);
			}
		}
	}
	ofstream clrfile(trdFilePath, ios::binary);
	clrfile.close();
	//写文件
	ofstream outfile(trdFilePath, ios::binary | ios::app);
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
			CString fieldName = ite_1->GetFieldName();
			CString recordvalue = recordite->GetValue(fieldName);
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
				int varcharSize = recordvalue.GetLength() + 1;
				string strtemp = CT2A(recordvalue.GetString());
				//char* writedvarchar = new char[varcharSize + 1];
				outfile.write((char*)(&varcharSize), sizeof(int));

				//writedvarchar
				outfile.write(strtemp.c_str(), varcharSize);
			}
		}
	}
	outfile.close();

	return 1;
}
