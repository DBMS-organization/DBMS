#include "pch.h"
#include "RecordDao.h"

vector<CRecordEntity> CRecordDao::getRecordList(CString dbname, CString tbname)
{
	CString tdfFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname+_T(".tdf");
	CString trdFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".trd");

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

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

		int dbnum;
		dbnum = ceil((double)fileSize / (128 + 2 + 256 + 20));

	}

	infile.close();

	return recordList;
}
