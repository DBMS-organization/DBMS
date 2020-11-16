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
	ifstream infile(tdfFilePath, ios::binary);				//�򿪱������ļ�����ȡ�ֶ���Ϣ

	if (!infile) {					//�ļ����쳣
		AfxMessageBox(_T("�������ļ����쳣�������ԣ�"));
	}
	else {							//�ļ�������
		infile.seekg(0, ios::cur);	//�ļ�ָ��ָ��ͷ

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);

		for (int i = 0; i < recordNum;i++) {
			//��ȡÿһ���ֶ���Ϣ
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

void CRecordDao::AddRecordNum(CString dbname, CString tbname)
{
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tbname) {
			int temp = ite->getRecord_num() + 1;
			ite->SetRecordNum(temp);
		}
	}

}

void CRecordDao::MinusRecordNum(CString dbname, CString tbname)
{
	CString tbFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {
		if (ite->getTableName() == tbname) {
			int temp = ite->getRecord_num() - 1;
			ite->SetRecordNum(temp);
		}
	}
}

void CRecordDao::WriteRecordNum(vector<CTableEntity> tablelist)
{
	for (vector<CTableEntity>::iterator ite = tablelist.begin(); ite != tablelist.end(); ++ite) {

	}

}
