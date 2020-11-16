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
	ifstream infile(tbFilePath, ios::binary);				//打开记录文件段信息

	if (!infile) {					//文件打开异常
		AfxMessageBox(_T("记录文件打开异常，请重试！"));
	}
	else {							//文件正常打开
		infile.seekg(0, ios::cur);	//文件指针指向开头

		//int fileSize;
		//infile.seekg(0, ios::end);
		//fileSize = infile.tellg();
		//infile.seekg(0, ios::beg);

		CString fieldName = _T("");
		CString fieldValue = _T("");

		for (int i = 0; i < recordNum;i++) {
			CRecordEntity recordEntity;
			//读取每一个字段信息
			for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1)
			{
				fieldName = ite_1->GetFieldName();
				if (ite_1->GetFieldType() == TYPE_BOOL) {
					bool tempbool;
					infile.read((char*)&tempbool, sizeof(bool));
					fieldValue=CTool::BoolToCString(tempbool);
				}
				else if (ite_1->GetFieldType() == TYPE_DATETIME) {
					char* tempTime = new char[20];
					infile.read(tempTime, 20);
					fieldValue = CString(tempTime);
				}
				else if (ite_1->GetFieldType() == TYPE_DOUBLE) {			
					double tempDouble;
					infile.read((char*)&tempDouble, sizeof(double));
					fieldValue = CTool::DoubleToCString(tempDouble);
				}
				else if (ite_1->GetFieldType() == TYPE_INTEGER) {
					int tempInt;
					infile.read((char*)&tempInt, sizeof(int));
					fieldValue = CTool::IntToCString(tempInt);
				}
				else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
					int varcharSize = 0;
					infile.read((char*)&varcharSize, sizeof(int));
					char* tempVarchar = new char[varcharSize];
					infile.read(tempVarchar, varcharSize);
					fieldValue = CString(tempVarchar);
				}

				recordEntity.SetValue(fieldName, fieldValue);
				recordList.push_back(recordEntity);
			}
		}

	}

	infile.close();

	return recordList;
}
