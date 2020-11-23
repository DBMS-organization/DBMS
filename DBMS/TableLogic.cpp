#include "pch.h"
#include "TableLogic.h"
#include "CTableEntity.h"
#include "TableDAO.h"

/**************************************
* 函数名：CTableLogic
* 参数：dbname（数据库名）
* 返回值：无
* 描述：初始化表逻辑层
***************************************/
CTableLogic::CTableLogic(CString dbName)
{
	
	this->DBName = dbName;
	string strdbname;
	strdbname = CT2A(DBName.GetString());
	this->tbPath = _T("DBMSROOT\\data\\") + dbName + _T("\\") + dbName + _T(".tb");
}

CTableLogic::~CTableLogic()
{
}


/*****************************************************
* 函数名：CreateTable
* 参数：tablename（表名）
* 返回值：返回0或1（0创建表失败，1创建表成功）
* 描述：根据表名创建一张新表，并将表实体信息写入tb文件
******************************************************/
int CTableLogic::CreateTable(CString& tablename)
{
	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbPath);
	bool TableExist = false;

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		string strDBname1;
		const char* chDBname1;
		strDBname1 = CT2A(ite->getTableName().GetString());
		chDBname1 = strDBname1.c_str();
		string tbname1, rnum, fnum;
		const char* rnum1;
		const char* fnum1;
		rnum = std::to_string(ite->getRecord_num());
		fnum = std::to_string(ite->getField_num());
		rnum1 = rnum.c_str();
		fnum1 = fnum.c_str();

		/*if (ite->getTableName() == tablename)
		{
			TableExist = true;
			break;
		}*/

		if (ite->getTableName().CompareNoCase(tablename) == 0)
		{
			TableExist = true;
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
		int recordnum;
		int fieldnum;

		//string
		strtablename = CT2A(tablename.GetString());
		strdbname = CT2A(DBName.GetString());
		/*string strdbname;
		strdbname = CT2A(DBName.GetString());*/
		strtdf = CT2A(tdfpath.GetString());
		strtrd = CT2A(trdpath.GetString());
		strtic = CT2A(ticpath.GetString());
		strcrtime = CT2A(tb.getCreateTime().GetString());
		strmtime = CT2A(tb.getModifyTime().GetString());
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

		outFile.write(dbname, 128);
		outFile.write(tbname, 128);

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

/********************************************************
* 函数名：DeleteTable
* 参数：tablename（表名）
* 返回值：返回0或1（0删除表失败，1删除表成功）
* 描述：根据表名删除表，并将删除后的表信息列表重写入tb文件
**********************************************************/
int CTableLogic::DeleteTable(CString tablename)
{
	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbPath);
	int count = 0;
	vector<CTableEntity> newRecord;
	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tablename)
		{
			DeleteFile(ite->gettdf());
			DeleteFile(ite->gettrd());
			DeleteFile(ite->gettic());
			count++;
		}
		else {
			newRecord.push_back(*(ite));
		}
	}
	if (count == 0) {
		return 0;
	}
	ofstream clearFile(tbPath, ios::binary);
	clearFile.close();
	ofstream outFile(tbPath, ios::binary | ios::app);

	for (vector<CTableEntity>::iterator ite_1 = newRecord.begin(); ite_1 != newRecord.end(); ++ite_1) {
		string strtablename, strdbname, strtdf, strtrd, strtic, 
			strcrtime, strmtime, strrecordnum, strfieldnum;
		int recordnum;
		int fieldnum;

		//string
		strdbname = CT2A(ite_1->getDBName().GetString());
		strtablename = CT2A(ite_1->getTableName().GetString());
		strtdf = CT2A(ite_1->gettdf().GetString());
		strtrd = CT2A(ite_1->gettrd().GetString());
		strtic = CT2A(ite_1->gettic().GetString());
		strcrtime = CT2A(ite_1->getCreateTime().GetString());
		strmtime = CT2A(ite_1->getModifyTime().GetString());
		recordnum = ite_1->getRecord_num();
		fieldnum = ite_1->getField_num();

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
	return 1;
}
