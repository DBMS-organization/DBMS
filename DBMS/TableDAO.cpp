#include "pch.h"
#include "TableDAO.h"

CTableDAO::CTableDAO()
{
}

CTableDAO::~CTableDAO()
{
}


//从文件读取表数据
vector<CTableEntity> CTableDAO::getTableList(CString& tbFilePath)
{

	vector<CTableEntity> tableList;
	ifstream infile(tbFilePath, ios::binary);
	if (!infile) {					//文件打开异常

	}
	else {							//文件正常打开
		//infile.seekg(0, ios::cur);	//文件指针指向开头
		char* tbName = new char[128];
		char* dbName = new char[128];

		/*char* Record_num = new char[4];
		char* Field_num = new char[4];*/
		int Record_num;
		int Field_num;

		char* tdf = new char[256];//表格定义文件路径
		char* tic = new char[256];//表格完整性文件路径
		char* trd = new char[256];//表格记录文件路径
		//char* tid = new char[256];//表格索引文件路径
		char* CrTime = new char[20];
		char* MTime = new char[20];

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);
		//_cprintf("fileSize-----%d\n", fileSize);

		int tablenum;
		tablenum = ceil((double)fileSize / (128 + 4 * 2 + 256 * 3 + 20 * 2));


		for (int i = 0; i < tablenum; i++) {
			infile.read(tbName, 128);

			/*infile.read(Record_num, 4);
			infile.read(Field_num, 4);*/
			infile.read((char*)&Record_num, sizeof(int));
			infile.read((char*)&Field_num, sizeof(int));

			infile.read(tdf, 256);
			infile.read(tic, 256);
			infile.read(trd, 256);
			infile.read(CrTime, 20);
			infile.read(MTime, 20);



			CString tbn(tbName);
			CString dbn(dbName);
			/*int recordnum = atoi(Record_num);
			int fieldnum = atoi(Field_num);*/
			CString tdfpath(tdf);
			CString ticpath(tic);
			CString trdpath(trd);
			CString createtime(CrTime);
			CString modifytime(MTime);
			//_cprintf("33333333 %s\n", dbName);

			CTableEntity tbE;
			tbE.SetTableName(tbn);
			tbE.SetCreateTime(createtime);

			//tbE.SetRecordNum(recordnum);
			tbE.SetRecordNum(Record_num);

			tbE.SetModifyTime(modifytime);

			//tbE.SetFieldNum(fieldnum);
			tbE.SetFieldNum(Field_num);

			_cprintf("recordnum, fieldnum: %d %d\n", Record_num, Field_num);
			
			tbE.SetDBName(dbn);
			tbE.Settdf(tdfpath);
			tbE.Settrd(trdpath);
			tbE.Settic(ticpath);

			tableList.push_back(tbE);
			//_cprintf("222222222 %s\n", tableList.at(0).getTableName().GetString());
		}
	}

	infile.close();


	return tableList;
}


