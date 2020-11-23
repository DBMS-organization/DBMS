#include "pch.h"
#include "RecordDao.h"
#include "Global.h"

/***************************************
* 函数名：getRecordList
* 参数：数据库名、表名
* 返回值：返回一个vector类型的记录列表
* 描述：从文件读取记录数据，存入vector中
****************************************/
vector<CRecordEntity> CRecordDao::getRecordList(CString dbname, CString tbname)
{
	CString tdfFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname+_T(".tdf");
	CString trdFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".trd");
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);
	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	int recordNum;

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite)
	{
		if (ite->getTableName() == tbname) {
			recordNum = ite->getRecord_num();
			break;
		}
	}

	vector<CRecordEntity> recordList;
	ifstream infile(trdFilePath, ios::binary);				//打开记录文件段信息

	if (!infile) {					//文件打开异常
		AfxMessageBox(_T("记录文件打开异常，请重试！"));
	}
	else {							//文件正常打开
		infile.seekg(0, ios::cur);	//文件指针指向开头

		CString fieldName = _T("");
		CString fieldValue = _T("");
		for (int i = 0; i < recordNum;i++) {
			CRecordEntity recordEntity;
			//读取每一个字段信息
			for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1)
			{
				fieldName = ite_1->GetFieldName();
				if (ite_1->GetFieldType() == TYPE_BOOL) {
					bool tempbool;
					infile.read((char*)&tempbool, sizeof(bool));
					fieldValue=CTool::BoolToCString(tempbool);
				}
				else if (ite_1->GetFieldType() == TYPE_DATETIME) {
					char* tempTime = new char[11];
					infile.read(tempTime, 11);
					fieldValue = CString(tempTime);
				}
				else if (ite_1->GetFieldType() == TYPE_DOUBLE) {			
					double tempDouble;
					infile.read((char*)&tempDouble, sizeof(double));
					fieldValue = CTool::DoubleToCString(tempDouble);
				}
				else if (ite_1->GetFieldType() == TYPE_INTEGER) {
					int tempInt;
					infile.read((char*)&tempInt, sizeof(int));
					fieldValue = CTool::IntToCString(tempInt);
				}
				else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
					int varcharSize = 0;
					infile.read((char*)&varcharSize, sizeof(int));
					char* tempVarchar = new char[varcharSize];
					infile.read(tempVarchar, varcharSize);
					fieldValue = CString(tempVarchar);
				}
				recordEntity.SetValue(fieldName, fieldValue);
			}
			recordList.push_back(recordEntity);
		}

	}

	infile.close();
	
	return recordList;
}

/*******************************************************************
* 函数名：AddRecordNum
* 参数：数据库名、表名
* 返回值：无
* 描述：将recordnum加1，并将其重写到.tb文件中，从而解决添加记录的bug
********************************************************************/
void CRecordDao::AddRecordNum(CString dbname, CString tbname)
{
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tbname) {
			int recordnum;
			recordnum = ite->getRecord_num();
			int temp = ite->getRecord_num() + 1;
			ite->SetRecordNum(temp);
			recordnum = ite->getRecord_num();
			break;
		}
	}
	CRecordDao::reWritetb(tablelist);
}

/*******************************************************************
* 函数名：MinusRecordNum
* 参数：数据库名、表名
* 返回值：无
* 描述：将recordnum减1，并将其重写到.tb文件中，从而解决删除记录的bug
********************************************************************/
void CRecordDao::MinusRecordNum(CString dbname, CString tbname)
{
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tbname) {
			int temp = ite->getRecord_num() - 1;
			ite->SetRecordNum(temp);
		}
	}
	CRecordDao::reWritetb(tablelist);
}


/*******************************************
* 函数名：reWritetb
* 参数：vector<CTableEntity>类型的表数据列表
* 返回值：无
* 描述：重写到.tb文件
*********************************************/
void CRecordDao::reWritetb(vector<CTableEntity> tablelist)
{
	CString tbfilepath = DATAFILEPATH + _T("\\") + tablelist[0].getDBName() + _T("\\") + tablelist[0].getDBName() + _T(".tb");
	ofstream clearFile(tbfilepath, ios::binary);
	clearFile.close();

	ofstream outFile(tbfilepath, ios::binary | ios::app);
	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		string strtablename, strdbname, strtdf, strtrd, 
			strtic, strcrtime, strmtime, strrecordnum, strfieldnum;
		
		int recordnum;
		int fieldnum;

		CString ctime = CTool::GetCurrTime();

		strtablename = CT2A(ite->getTableName().GetString());
		strdbname = CT2A(ite->getDBName().GetString());
		strtdf = CT2A(ite->gettdf().GetString());
		strtrd = CT2A(ite->gettrd().GetString());
		strtic = CT2A(ite->gettic().GetString());
		strcrtime = CT2A(ite->getCreateTime().GetString());
		strmtime = CT2A(ctime.GetString());

		recordnum = ite->getRecord_num();
		fieldnum = ite->getField_num();

		outFile.write(strdbname.c_str(), 128);
		outFile.write(strtablename.c_str(), 128);
		outFile.write((char*)(&recordnum), sizeof(int));
		outFile.write((char*)(&fieldnum), sizeof(int));
		outFile.write(strtdf.c_str(), 256);
		outFile.write(strtrd.c_str(), 256);
		outFile.write(strtic.c_str(), 256);
		outFile.write(strcrtime.c_str(), 20);
		outFile.write(strmtime.c_str(), 20);


	}
	outFile.close();
}


/**************************************************************
* 函数名：reWriteRecord
* 参数：数据库名、表名、vector<CRecordEntity>类型的记录数据列表
* 返回值：无
* 描述：重写trd文件，用于解决添加记录之后不能添加字段的bug
***************************************************************/
void CRecordDao::reWriteRecord(CString dbname, CString tbname, vector<CRecordEntity> recordlist)
{
	CString trdfilepath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".trd");

	CString tdfFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".tdf");

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	//清空trd文件
	ofstream clearFile(trdfilepath, ios::binary);
	clearFile.close();

	ofstream outfile(trdfilepath, ios::binary | ios::app);

	for (vector<CRecordEntity>::iterator ite = recordlist.begin(); ite != recordlist.end(); ++ite) {
		for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
			CString fieldName = ite_1->GetFieldName();
			CString recordvalue = ite->GetValue(fieldName);
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
}
