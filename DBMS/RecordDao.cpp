#include "pch.h"
#include "RecordDao.h"

vector<CRecordEntity> CRecordDao::getRecordList(CString dbname, CString tbname)
{
	CString tdfFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname+_T(".tdf");
	CString trdFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".trd");
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);
	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	int recordNum;

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite)
	{
		
	}

	vector<CRecordEntity> recordList;
	ifstream infile(tdfFilePath, ios::binary);				//打开表描述文件，获取字段信息

	if (!infile) {					//文件打开异常
		AfxMessageBox(_T("表描述文件打开异常，请重试！"));
	}
	else {							//文件正常打开
		infile.seekg(0, ios::cur);	//文件指针指向开头

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);

		int dbnum;
		dbnum = ceil((double)fileSize / (128 + 2 + 256 + 20));

	}

	infile.close();

	return recordList;
}
