#include "pch.h"
#include "DBDao.h"


//从文件读取数据库列表
vector<CDBEntity> CDBDao::getDatabasesList(CString& sysFilePath) {
	AllocConsole();

	vector<CDBEntity> databasesList;
	ifstream infile(sysFilePath, ios::binary);
	if (!infile) {					//文件打开异常
		AfxMessageBox(_T("数据库打开异常，请重试！"));
	}
	else {							//文件正常打开
		//infile.seekg(0, ios::cur);	//文件指针指向开头
		char* dbName = new char[128];
		bool dbType;

		char* fileName = new char[256];
		char* createTime = new char[20];

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);

		int dbnum;	//数据库数
		dbnum = ceil((double)fileSize / (128 + sizeof(bool) + 256 + 20));


		for (int i = 0; i < dbnum; i++) {
			infile.read(dbName, 128);
			infile.read((char*)&dbType, sizeof(bool));

			infile.read(fileName, 256);
			infile.read(createTime, 20);

			CString dbn(dbName);
			CString fn(fileName);
			CString ct(createTime);

			CDBEntity dbE = CDBEntity(dbn, true, fn, ct);

			databasesList.push_back(dbE);
		}
	}

	infile.close();


	return databasesList;
}



