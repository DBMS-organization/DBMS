#include "pch.h"
#include "DBDao.h"



/***************************************************************************************
* 函数名：getDatabasesList
* 参数：sysFilePath（db文件的地址）
* 返回值：返回一个vector类型的数据库列表
* 描述：从.db文件读取数据库列表，
		.db文件储存各数据库的数据库名、数据库类型、数据库数据文件夹路径、创建时间，
		通过读取文件得到数据库列表，每个vector储存一条数据库的信息，供其他函数调用
****************************************************************************************/
vector<CDBEntity> CDBDao::getDatabasesList(CString& sysFilePath) {

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



