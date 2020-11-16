#include "pch.h"
#include "TableLogic.h"
#include "CTableEntity.h"
#include "TableDAO.h"

CTableLogic::CTableLogic(CString dbName)
{
	
	this->DBName = dbName;
	string strdbname;
	strdbname = CT2A(DBName.GetString());
	_cprintf("\nwwwwwwwwwwwwwwwwwwwwwwwwww strdbname.c_str()   %s\n", strdbname.c_str());
	this->tbPath = _T("DBMSROOT\\data\\") + dbName + _T("\\") + dbName + _T(".tb");
}

CTableLogic::~CTableLogic()
{
}


//根据表名创建一张新表
int CTableLogic::CreateTable(CString& tablename)
{
	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbPath);
	bool TableExist = false;

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		string strDBname1;
		const char* chDBname1;
		strDBname1 = CT2A(ite->getTableName().GetString());
		chDBname1 = strDBname1.c_str();
		//_cprintf("111111111 %s\n", chDBname1);
		string tbname1, rnum, fnum;
		const char* rnum1;
		const char* fnum1;
		rnum = std::to_string(ite->getRecord_num());
		fnum = std::to_string(ite->getField_num());
		rnum1 = rnum.c_str();
		fnum1 = fnum.c_str();

		_cprintf("5555555555555555555555%s %s %s\n", chDBname1, rnum1, fnum1);

		if (ite->getTableName() == tablename)
		{
			TableExist = true;
			//_cprintf("true\n");
			break;
		}
	}

	if (!TableExist) {
		ofstream outFile(tbPath, ios::binary | ios::app);
		ofstream createfile1, createfile2, createfile3;
		CString tdfpath, trdpath, ticpath;
		tdfpath = _T("DBMSROOT\\data\\") + DBName + _T("\\") + tablename + _T(".tdf");
		trdpath = _T("DBMSROOT\\data\\") + DBName + _T("\\") + tablename + _T(".trd");
		ticpath = _T("DBMSROOT\\data\\") + DBName + _T("\\") + tablename + _T(".tic");
		string strdbname1;
		strdbname1 = CT2A(DBName.GetString());
		_cprintf("\n111111111111111111111111strdbname.c_str()   %s\n", strdbname1.c_str());
		createfile1.open(tdfpath.GetString(), ios::binary | ios::app);
		createfile2.open(trdpath.GetString(), ios::binary | ios::app);
		createfile3.open(ticpath.GetString(), ios::binary | ios::app);
		createfile1.close();
		createfile2.close();
		createfile3.close();

		CTableEntity tb(tablename, DBName);

		string strtablename, strdbname, strtdf, strtrd, strtic, strcrtime, strmtime, strrecordnum, strfieldnum;
		const char* tbname;
		const char* dbname;
		const char* tdf;
		const char* trd;
		const char* tic;
		const char* crtime;
		const char* mtime;
		/*const char* recordnum;
		const char* fieldnum;*/
		int recordnum;
		int fieldnum;

		//string
		strtablename = CT2A(tablename.GetString());
		strdbname = CT2A(DBName.GetString());
		/*string strdbname;
		strdbname = CT2A(DBName.GetString());*/
		_cprintf("\n333333333333333333333333333333333strdbname.c_str()   %s\n", strdbname.c_str());
		strtdf = CT2A(tdfpath.GetString());
		strtrd = CT2A(trdpath.GetString());
		strtic = CT2A(ticpath.GetString());
		strcrtime = CT2A(tb.getCreateTime().GetString());
		strmtime = CT2A(tb.getModifyTime().GetString());
		/*strrecordnum = std::to_string(tb.getRecord_num());
		strfieldnum = std::to_string(tb.getField_num());*/
		recordnum = tb.getRecord_num();
		fieldnum = tb.getField_num();

		//char*
		tbname = strtablename.c_str();
		dbname = strdbname.c_str();
		tdf = strtdf.c_str();
		trd = strtrd.c_str();
		tic = strtic.c_str();
		crtime = strcrtime.c_str();
		mtime = strmtime.c_str();
		/*recordnum = strrecordnum.c_str();
		fieldnum = strfieldnum.c_str();*/


		outFile.write(dbname, 128);
		outFile.write(tbname, 128);

		/*outFile.write(recordnum, 4);
		outFile.write(fieldnum, 4);*/
		outFile.write((char*)(&recordnum), sizeof(int));
		outFile.write((char*)(&fieldnum), sizeof(int));

		outFile.write(tdf, 256);
		outFile.write(trd, 256);
		outFile.write(tic, 256);
		outFile.write(crtime, 20);
		outFile.write(mtime, 20);

		outFile.close();
		return 1;
	}
	else {
		return 0;
	}


}
