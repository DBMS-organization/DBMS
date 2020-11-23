#include "pch.h"
#include "TableDAO.h"

CTableDAO::CTableDAO()
{
}

CTableDAO::~CTableDAO()
{
}


/*********************************************
* ��������getTableList
* ������tb�ļ�·��
* ����ֵ��vector<CTableEntity>���͵ı������б�
* ���������ļ���ȡ�����ݣ�����vector��
**********************************************/
vector<CTableEntity> CTableDAO::getTableList(CString& tbFilePath)
{

	vector<CTableEntity> tableList;
	ifstream infile(tbFilePath, ios::binary);
	if (!infile) {					//�ļ����쳣

	}
	else {							//�ļ�������
		//infile.seekg(0, ios::cur);	//�ļ�ָ��ָ��ͷ
		char* tbName = new char[128];
		char* dbName = new char[128];

		int Record_num;
		int Field_num;

		char* tdf = new char[256];//������ļ�·��
		char* tic = new char[256];//����������ļ�·��
		char* trd = new char[256];//����¼�ļ�·��
		char* CrTime = new char[20];
		char* MTime = new char[20];

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);

		int tablenum;
		tablenum = ceil((double)fileSize / (128*2 + sizeof(int) * 2 + 256 * 3 + 20 * 2));

		for (int i = 0; i < tablenum; i++) {
			infile.read(dbName, 128);
			infile.read(tbName, 128);

			infile.read((char*)&Record_num, sizeof(int));
			infile.read((char*)&Field_num, sizeof(int));

			infile.read(tdf, 256);
			infile.read(tic, 256);
			infile.read(trd, 256);
			infile.read(CrTime, 20);
			infile.read(MTime, 20);

			CString tbn(tbName);
			CString dbn(dbName);
			CString tdfpath(tdf);
			CString ticpath(tic);
			CString trdpath(trd);
			CString createtime(CrTime);
			CString modifytime(MTime);

			CTableEntity tbE;
			tbE.SetDBName(dbn);
			tbE.SetTableName(tbn);
			tbE.SetCreateTime(createtime);
			tbE.SetRecordNum(Record_num);
			tbE.SetModifyTime(modifytime);
			tbE.SetFieldNum(Field_num);
			tbE.Settdf(tdfpath);
			tbE.Settrd(trdpath);
			tbE.Settic(ticpath);

			tableList.push_back(tbE);
		}
	}

	infile.close();


	return tableList;
}


