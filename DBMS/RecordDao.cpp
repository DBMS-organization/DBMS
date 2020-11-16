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
	ifstream infile(tdfFilePath, ios::binary);				//打开表描述文件，获取字段信息

	if (!infile) {					//文件打开异常
		AfxMessageBox(_T("表描述文件打开异常，请重试！"));
	}
	else {							//文件正常打开
		infile.seekg(0, ios::cur);	//文件指针指向开头

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);

		for (int i = 0; i < recordNum;i++) {
			//读取每一个字段信息
			for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1)
			{
				if (ite_1->GetFieldType() == TYPE_BOOL) {

				}
				else if (ite_1->GetFieldType() == TYPE_DATETIME) {

				}
				else if (ite_1->GetFieldType() == TYPE_DOUBLE) {

				}
				else if (ite_1->GetFieldType() == TYPE_INTEGER) {

				}
				else if (ite_1->GetFieldType() == TYPE_VARCHAR) {

				}
			}
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
	CRecordDao::WriteRecordNum(tablelist);
}

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
	CRecordDao::WriteRecordNum(tablelist);
}

void CRecordDao::WriteRecordNum(vector<CTableEntity> tablelist)
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

		SYSTEMTIME time;
		::GetLocalTime(&time);
		CTime t(time);
		CString ctime = t.Format("%Y-%m-%d %H:%M:%S");
		


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
