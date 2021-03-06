#include "pch.h"
#include "DBLogic.h"

#include"CDBEntity.h"
#include"DBDao.h"


/***************************************************************************
* 函数名：CreateDatabase
* 参数：dbName（数据库名）
* 返回值：返回0或1（0创建数据库失败，1创建数据库成功）
* 描述：接收一个数据库名并创建数据库，并将创建成功的数据库实体信息写入db文件
****************************************************************************/
int CDBLogic::CreateDatabase(CString& dbName) {
	vector<CDBEntity> dblist = CDBDao::getDatabasesList(syspath);
	/*CString createtime;
	createtime = dblist->GetcreateTime();*/
	bool DBExist = false;
	for (vector<CDBEntity>::iterator ite = dblist.begin(); ite != dblist.end(); ++ite)
	{

		string strDBname1;
		const char* chDBname1;
		strDBname1 = CT2A(ite->GetdbName().GetString());
		chDBname1 = strDBname1.c_str();

		/*if (ite->GetdbName() == dbName)
		{
			DBExist = true;
			break;
		}*/

		if (ite->GetdbName().CompareNoCase(dbName) == 0)
		{
			DBExist = true;
			break;
		}
		
	}

	if (!DBExist) {
		ofstream outFile(syspath, ios::binary | ios::app);
		CString filepath;
		filepath = _T("DBMSROOT\\data\\") + dbName;
		CreateDirectory(filepath, NULL);//创建数据库名的数据库文件
		ofstream tbfile;
		CString tbpath = _T("DBMSROOT\\data\\") + dbName + _T("\\") + dbName + _T(".tb");
		tbfile.open(tbpath.GetString(), ios::binary | ios::app);
		tbfile.close();

		CDBEntity db(dbName, true, filepath);

		string strDBname, strfilepath, strctime;
		const char* chDBname;
		const char* fpath;
		const char* ctime;
		strDBname = CT2A(dbName.GetString());
		strfilepath = CT2A(filepath.GetString());
		strctime = CT2A(db.GetcreateTime().GetString());
		chDBname = strDBname.c_str();
		fpath = strfilepath.c_str();
		ctime = strctime.c_str();

		bool type = true;
		outFile.write(chDBname, 128);
		outFile.write((char*)(&type), sizeof(bool));

		outFile.write(fpath, 256);
		outFile.write(ctime, 20);

		outFile.close();
		return 1;
	}
	else {
		return 0;
	}
}


/*****************************************************************************
* 函数名：DeleteDatabase
* 参数：dbName（数据库名）
* 返回值：返回0或1（0删除数据库失败，1删除数据库成功）
* 描述：接收一个数据库名并删除数据库，并将删除成功之后的数据库列表重写在db文件，
		并删除相关数据库数据文件夹及其子文件
******************************************************************************/
int CDBLogic::DeleteDatabase(CString dbName)
{
	vector<CDBEntity> dblist = CDBDao::getDatabasesList(syspath);
	int count = 0;
	vector<CDBEntity> newRecord;
	for (vector<CDBEntity>::iterator dbite = dblist.begin(); dbite != dblist.end(); ++dbite) {
		if (dbite->GetdbName()==dbName)
		{
			count++;
		}
		else {			
			newRecord.push_back(*(dbite));
		}
	}
	if (count == 0)return 0;

	ofstream clearFile(syspath, ios::binary);
	clearFile.close();
	ofstream outFile(syspath, ios::binary | ios::app);

	for (vector<CDBEntity>::iterator ite = newRecord.begin(); ite != newRecord.end(); ++ite) {
		string strDBname, strfilepath, strctime;
		strDBname = CT2A(ite->GetdbName().GetString());
		strfilepath = CT2A(ite->GetDBpath().GetString());
		strctime = CT2A(ite->GetcreateTime().GetString());

		bool type = true;
		outFile.write(strDBname.c_str(), 128);
		outFile.write((char*)(&type), sizeof(bool));

		outFile.write(strfilepath.c_str(), 256);
		outFile.write(strctime.c_str(), 20);
	}
	outFile.close();
	CString filepath;
	filepath = _T("DBMSROOT\\data\\") + dbName;
	CTool::DeleteFolder(filepath);//删除文件夹及所有子文件
	return 1;
}




