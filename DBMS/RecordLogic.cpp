#include "pch.h"


CRecordLogic::CRecordLogic()
{
	
}

CRecordLogic::~CRecordLogic()
{
}

/*************************************************
* 函数名：AddRecord
* 参数：dbname（数据库名）
		tablename（表名）
		record（记录实体）
* 返回值：返回0或1（0添加记录失败，1添加记录成功）
* 描述：添加记录，并判断该记录的类型写入trd文件
**************************************************/
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
					AfxMessageBox(_T("字段")+fieldite->GetFieldName()+_T("的值触犯唯一性约束！"));
					return 0;
				}
			}
		}
		if (fieldite->GetNotNull()) {
			if (record.GetValue(fieldite->GetFieldName()) == _T("")) {
				AfxMessageBox(_T("字段") + fieldite->GetFieldName() + _T("的值触犯非空约束！"));
				return 0;
			}
		}
	}
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
			outfile.write((char*)(&varcharSize), sizeof(int));
			outfile.write(strtemp.c_str(), varcharSize);
		}
	}
	outfile.close();

	//在.tb文件中增加记录数
	CRecordDao::AddRecordNum(dbname, tablename);

	return 1;
}

/******************************************************
* 函数名：DeleteRecord
* 参数：dbname（数据库名）
		tablename（表名）
		fieldname（字段名）
		fieldvalue（字段值）
* 返回值：返回0或1（0删除记录失败，1删除记录成功）
* 描述：删除记录，并将删除记录后的纪录列表重写入trd文件
*******************************************************/
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
					outfile.write((char*)(&varcharSize), sizeof(int));
					outfile.write(strtemp.c_str(), varcharSize);
				}
			}
		}
		outfile.close();
		return 1;
	
}


/************************************************************************************************
* 函数名：AlterRecord
* 参数：dbname（数据库名）
		tablename（表名）
		queryField（用来查询的字段名）
		fitValue（用来查询的字段需要满足的值）
		modifiedField（需要修改的字段名）
		modifiedValue（修改后该字段的值）
* 返回值：返回0或1（0修改记录失败，1修改记录成功）
* 描述：修改记录，并重写trd文件
		相当于alter table tablename set modifiedField="modifiedValue" where queryFiled="fitValue";
**************************************************************************************************/
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

	bool isUnique = false;
	bool isNotNull = false;
	bool isexistmodifiedField = false;
	for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
		if (ite_1->GetFieldName().CompareNoCase(modifiedField) == 0) {
			isUnique=ite_1->GetUnique();
			isNotNull = ite_1->GetNotNull();
			isexistmodifiedField = true;
		}
	}
	if (!isexistmodifiedField) {
		AfxMessageBox(_T("不存在要修改的字段 ")+modifiedField + _T(" ！"));
		return 0;
	}
	if (isNotNull) {
		if (modifiedValue == _T("")) {
			AfxMessageBox(modifiedField + _T("的值") + modifiedValue + _T("触犯非空约束！"));
			return 0;
		}
	}
	bool isexistqueryField = false;

	for (vector<CFieldEntity>::iterator ite_2 = fieldlist.begin(); ite_2 != fieldlist.end(); ++ite_2) {
		if (ite_2->GetFieldName().CompareNoCase(queryField) == 0) {
			isexistqueryField = true;
		}
	}
	if (!isexistqueryField) {
		AfxMessageBox(_T("不存在要查询的字段 ") + queryField + _T(" ！"));
		return 0;
	}

	for (vector<CFieldEntity>::iterator ite_3 = fieldlist.begin(); ite_3 != fieldlist.end(); ++ite_3) {
		if (ite_3->GetFieldName().CompareNoCase(modifiedField) == 0) {
			CString type = CTool::IntTodataType(ite_3->GetFieldType());
			if (!CTool::judgeType(type, modifiedValue, ite_3->GetFieldParam())) {
				//valid = false;
				//AfxMessageBox(_T("输入值不符合约束！"));
				if (type == _T("BOOL")) {
					AfxMessageBox(_T("修改后的值与该字段数据类型不符,\n请输入true或false！"));
				}
				else if (type == _T("DATETIME")) {
					AfxMessageBox(_T("修改后的值与该字段数据类型不符,请按格式yyyy-mm-dd输入！"));
				}
				else if (type == _T("VARCHAR")) {
					AfxMessageBox(_T("修改后的值超过该字段限制的最大长度！"));
				}
				else if (type == _T("INTEGER")) {
					AfxMessageBox(_T("修改后的值与该字段数据类型不符，不是整型数！"));
				}
				else if (type == _T("DOUBLE")) {
					AfxMessageBox(_T("修改后的值与该字段数据类型不符，不是双精度数！"));
				}
				return 0;
			}
		}
	}

	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {	
		if (fitValue.CompareNoCase(recordite->GetValue(queryField))==0) {
			if (!isUnique) {
				recordite->SetValue(modifiedField,modifiedValue);
			}
			else {
				for (vector<CRecordEntity>::iterator recordite_1 = recordlist.begin(); recordite_1 != recordlist.end(); ++recordite_1) {
					if (recordite_1->GetValue(modifiedField).Compare(modifiedValue) == 0) {
						AfxMessageBox(modifiedField+_T("的值")+ modifiedValue+ _T("触犯唯一性约束！"));
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
				outfile.write((char*)(&varcharSize), sizeof(int));

				//writedvarchar
				outfile.write(strtemp.c_str(), varcharSize);
			}
		}
	}
	outfile.close();

	return 1;
}
