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
		bool dbType;

		char* fileName = new char[256];
		char* createTime = new char[20];

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);

		int dbnum;	//���ݿ���
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



