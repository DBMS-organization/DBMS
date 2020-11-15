#include "pch.h"
#include "DBDao.h"


//���ļ���ȡ���ݿ��б�
vector<CDBEntity> CDBDao::getDatabasesList(CString& sysFilePath) {
	AllocConsole();

	vector<CDBEntity> databasesList;
	ifstream infile(sysFilePath, ios::binary);
	if (!infile) {					//�ļ����쳣
		AfxMessageBox(_T("���ݿ���쳣�������ԣ�"));
	}
	else {							//�ļ�������
		//infile.seekg(0, ios::cur);	//�ļ�ָ��ָ��ͷ
		char* dbName = new char[128];
		//char* dbType = new char[2];
		bool dbType;

		char* fileName = new char[256];
		char* createTime = new char[20];

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);
		//_cprintf("fileSize-----%d\n", fileSize);

		int dbnum;
		//int pageNum; //ҳ��
		dbnum = ceil((double)fileSize / (128 + sizeof(bool) + 256 + 20));


		for (int i = 0; i < dbnum; i++) {
			infile.read(dbName, 128);
			//infile.read(dbType, 2);
			infile.read((char*)&dbType, sizeof(bool));

			infile.read(fileName, 256);
			infile.read(createTime, 20);


			/*if (dbType[0] == '1') {
				_cprintf("aaaaaaaaaaa%s %s %s %s\n", dbName, dbType, fileName, createTime);
			}*/

			if (dbType) {
				_cprintf("getDatabasesList() dbName, dbType, fileName, createTime:  %s %d %s %s\n", dbName, dbType, fileName, createTime);
			}


			CString dbn(dbName);
			CString fn(fileName);
			CString ct(createTime);
			_cprintf("33333333 %s\n", dbn);

			CDBEntity dbE = CDBEntity(dbn, true, fn, ct);

			databasesList.push_back(dbE);
			_cprintf("222222222 %s\n", databasesList.at(0).GetdbName().GetString());
		}
	}

	infile.close();


	return databasesList;
}



