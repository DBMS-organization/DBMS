#include "pch.h"
#include "DBLogic.h"

#include"CDBEntity.h"
#include"DBDao.h"

//接收一个数据库名并创建数据库
int CDBLogic::CreateDatabase(CString& dbName) {
	vector<CDBEntity> dblist = CDBDao::getDatabasesList(syspath);
	/*CString createtime;
	createtime = dblist->GetcreateTime();*/
	bool DBExist = false;
	for (vector<CDBEntity>::iterator ite = dblist.begin(); ite != dblist.end(); ++ite)
	{
		//AllocConsole();

		string strDBname1;
		const char* chDBname1;
		strDBname1 = CT2A(ite->GetdbName().GetString());
		chDBname1 = strDBname1.c_str();
		_cprintf("111111111 %s\n", chDBname1);


		if (ite->GetdbName() == dbName)
		{
			DBExist = true;
			_cprintf("true\n");
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

		/*char type[2];
		type[0] = '1';
		type[1] = '\0';*/
		bool type = true;
		outFile.write(chDBname, 128);
		//outFile.write(type, 2);
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
		//outFile.write(type, 2);
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




