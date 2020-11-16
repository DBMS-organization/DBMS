#include "pch.h"
#include "RecordDao.h"
#include "Global.h"

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
		if (ite->getTableName() == tbname) {
			recordNum = ite->getRecord_num();
			break;
		}
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

		for (int i = 0; i < recordNum;i++) {
			//读取每一个字段信息
			for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1)
			{
				if (ite_1->GetFieldType() == TYPE_BOOL) {

				}
				else if (ite_1->GetFieldType() == TYPE_DATETIME) {

				}
				else if (ite_1->GetFieldType() == TYPE_DOUBLE) {

				}
				else if (ite_1->GetFieldType() == TYPE_INTEGER) {

				}
				else if (ite_1->GetFieldType() == TYPE_VARCHAR) {

				}
			}
		}

	}

	infile.close();

	return recordList;
}
