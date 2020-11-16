#include "pch.h"
#include "RecordDao.h"
#include "Global.h"

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

		//int fileSize;
		//infile.seekg(0, ios::end);
		//fileSize = infile.tellg();
		//infile.seekg(0, ios::beg);

		CString fieldName = _T("");
		CString fieldValue = _T("");
		_cprintf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  %d\n", recordNum);
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
					char* tempTime = new char[20];
					infile.read(tempTime, 20);
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
					//_cprintf("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ %s\n", tempVarchar);
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

void CRecordDao::AddRecordNum(CString dbname, CString tbname)
{
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);
	
	/*string strdbname, strtbname;
	strdbname = CT2A(tablelist.at(0).getDBName().GetString());
	strtbname = CT2A(tablelist.at(0).getTableName().GetString());
	_cprintf("\nstrdbname.c_str(), strtbname.c_str()    %s %s\n", strdbname.c_str(), strtbname.c_str());*/
	//_cprintf("\ndddddddddddddddddddddddddddddddddddddd tablelist size %d\n ",tablelist.size());

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tbname) {
			int recordnum;
			recordnum = ite->getRecord_num();
			_cprintf("\ntestrecordnum1    %d\n", recordnum);
			int temp = ite->getRecord_num() + 1;
			ite->SetRecordNum(temp);
			recordnum = ite->getRecord_num();
			_cprintf("\ntestrecordnum2    %d\n", recordnum);
			break;
		}
	}
	CRecordDao::reWritetb(tablelist);
}

void CRecordDao::MinusRecordNum(CString dbname, CString tbname)
{
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tbname) {
			int temp = ite->getRecord_num() - 1;
			ite->SetRecordNum(temp);
			_cprintf("111111111111111111111111111111111111111111111111111recordnum %d\n", temp);
		}
	}
	CRecordDao::reWritetb(tablelist);
}

void CRecordDao::reWritetb(vector<CTableEntity> tablelist)
{
	//string strdbname, strtbname;
	//strdbname = CT2A(tablelist.at(0).getDBName().GetString());
	//strtbname = CT2A(tablelist.at(0).getTableName().GetString());
	//_cprintf("\n888888888888888strdbname.c_str(), strtbname.c_str()    %s %s\n", strdbname.c_str(), strtbname.c_str());
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
		_cprintf("\ntestrecordnum3    %d %s\n", recordnum, strtablename.c_str());

		
		
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
