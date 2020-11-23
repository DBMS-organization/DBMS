#include "pch.h"


CRecordLogic::CRecordLogic()
{
	
}

CRecordLogic::~CRecordLogic()
{
}

/*************************************************
* ��������AddRecord
* ������dbname�����ݿ�����
		tablename��������
		record����¼ʵ�壩
* ����ֵ������0��1��0��Ӽ�¼ʧ�ܣ�1��Ӽ�¼�ɹ���
* ��������Ӽ�¼�����жϸü�¼������д��trd�ļ�
**************************************************/
int CRecordLogic::AddRecord(CString dbname, CString tablename, CRecordEntity &record)
{
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath= _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");
	
	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	for (vector<CFieldEntity>::iterator fieldite = fieldlist.begin(); fieldite != fieldlist.end(); ++fieldite) {
		//�ж������ֶ����Ƿ�Ψһ
		if (fieldite->GetUnique()) {
			for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
				//�ж�ͬһ�ֶ����Ƿ������ͬ��ֵ
				if (record.GetValue(fieldite->GetFieldName()) == recordite->GetValue(fieldite->GetFieldName())) {
					AfxMessageBox(_T("�ֶ�")+fieldite->GetFieldName()+_T("��ֵ����Ψһ��Լ����"));
					return 0;
				}
			}
		}
		if (fieldite->GetNotNull()) {
			if (record.GetValue(fieldite->GetFieldName()) == _T("")) {
				AfxMessageBox(_T("�ֶ�") + fieldite->GetFieldName() + _T("��ֵ�����ǿ�Լ����"));
				return 0;
			}
		}
	}
	ofstream outfile(trdFilePath, ios::binary | ios::app);
	
	for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
		CString fieldName = ite_1->GetFieldName();
		CString recordvalue = record.GetValue(fieldName);
		if (ite_1->GetFieldType() == TYPE_BOOL) {
			bool tempbool;
			tempbool = CTool::CStringToBool(recordvalue);
			outfile.write((char*)(&tempbool), sizeof(bool));
		}
		else if (ite_1->GetFieldType() == TYPE_DATETIME) {
			string tempTime;
			tempTime = CT2A(recordvalue.GetString());
			outfile.write(tempTime.c_str(), CTool::getTypeStoreLength(_T("DATETIME")));
		}
		else if (ite_1->GetFieldType() == TYPE_DOUBLE) {
			double tempDouble;
			tempDouble = CTool::CStringToDouble(recordvalue);
			outfile.write((char*)(&tempDouble), sizeof(double));
		}
		else if (ite_1->GetFieldType() == TYPE_INTEGER) {
			int tempInt = CTool::CStringToInt(recordvalue);
			outfile.write((char*)(&tempInt), sizeof(int));
		}
		else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
			int varcharSize = recordvalue.GetLength()+1;
			string strtemp = CT2A(recordvalue.GetString());
			outfile.write((char*)(&varcharSize), sizeof(int));
			outfile.write(strtemp.c_str(), varcharSize);
		}
	}
	outfile.close();

	//��.tb�ļ������Ӽ�¼��
	CRecordDao::AddRecordNum(dbname, tablename);

	return 1;
}

/******************************************************
* ��������DeleteRecord
* ������dbname�����ݿ�����
		tablename��������
		fieldname���ֶ�����
		fieldvalue���ֶ�ֵ��
* ����ֵ������0��1��0ɾ����¼ʧ�ܣ�1ɾ����¼�ɹ���
* ������ɾ����¼������ɾ����¼��ļ�¼�б���д��trd�ļ�
*******************************************************/
int CRecordLogic::DeleteRecord(CString dbname, CString tablename, CString fieldname, CString fieldvalue)
{
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	int count=0;
	vector<CRecordEntity> newRecord;
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		if (fieldvalue.CompareNoCase(recordite->GetValue(fieldname))!=0) {
			newRecord.push_back(*(recordite));
	        //��.tb�ļ��м��ټ�¼��
		}else {
			CRecordDao::MinusRecordNum(dbname, tablename);
			count++;
		}
	}
	if (count == 0)return 0;

		ofstream clrfile(trdFilePath, ios::binary);
		clrfile.close();
		//д�ļ�
		ofstream outfile(trdFilePath, ios::binary | ios::app);
		for (vector<CRecordEntity>::iterator recordite = newRecord.begin(); recordite != newRecord.end(); ++recordite) {
			for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
				CString fieldName = ite_1->GetFieldName();
				CString recordvalue = recordite->GetValue(fieldName);
				if (ite_1->GetFieldType() == TYPE_BOOL) {
					bool tempbool;
					tempbool = CTool::CStringToBool(recordvalue);
					outfile.write((char*)(&tempbool), sizeof(bool));
				}
				else if (ite_1->GetFieldType() == TYPE_DATETIME) {
					string tempTime;
					tempTime = CT2A(recordvalue.GetString());
					outfile.write(tempTime.c_str(), CTool::getTypeStoreLength(_T("DATETIME")));
				}
				else if (ite_1->GetFieldType() == TYPE_DOUBLE) {
					double tempDouble;
					tempDouble = CTool::CStringToDouble(recordvalue);
					outfile.write((char*)(&tempDouble), sizeof(double));
				}
				else if (ite_1->GetFieldType() == TYPE_INTEGER) {
					int tempInt = CTool::CStringToInt(recordvalue);
					outfile.write((char*)(&tempInt), sizeof(int));
				}
				else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
					int varcharSize = recordvalue.GetLength() + 1;
					string strtemp = CT2A(recordvalue.GetString());
					outfile.write((char*)(&varcharSize), sizeof(int));
					outfile.write(strtemp.c_str(), varcharSize);
				}
			}
		}
		outfile.close();
		return 1;
	
}


/************************************************************************************************
* ��������AlterRecord
* ������dbname�����ݿ�����
		tablename��������
		queryField��������ѯ���ֶ�����
		fitValue��������ѯ���ֶ���Ҫ�����ֵ��
		modifiedField����Ҫ�޸ĵ��ֶ�����
		modifiedValue���޸ĺ���ֶε�ֵ��
* ����ֵ������0��1��0�޸ļ�¼ʧ�ܣ�1�޸ļ�¼�ɹ���
* �������޸ļ�¼������дtrd�ļ�
		�൱��alter table tablename set modifiedField="modifiedValue" where queryFiled="fitValue";
**************************************************************************************************/
int CRecordLogic::AlterRecord(CString dbname, 
	                          CString tablename, 
	                          CString queryField, 
	                          CString fitValue,
	                          CString modifiedField, 
	                          CString modifiedValue)
{
	this->dbName = dbname;
	this->tbName = tablename;
	this->tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".tdf");
	this->trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tablename + _T(".trd");
	this->tbFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + dbname + _T(".tb");

	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbName, tbName);

	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	vector<CTableEntity> tablelist = CTableDAO::getTableList(tbFilePath);

	bool isUnique = false;
	bool isNotNull = false;
	bool isexistmodifiedField = false;
	for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
		if (ite_1->GetFieldName().CompareNoCase(modifiedField) == 0) {
			isUnique=ite_1->GetUnique();
			isNotNull = ite_1->GetNotNull();
			isexistmodifiedField = true;
		}
	}
	if (!isexistmodifiedField) {
		AfxMessageBox(_T("������Ҫ�޸ĵ��ֶ� ")+modifiedField + _T(" ��"));
		return 0;
	}
	if (isNotNull) {
		if (modifiedValue == _T("")) {
			AfxMessageBox(modifiedField + _T("��ֵ") + modifiedValue + _T("�����ǿ�Լ����"));
			return 0;
		}
	}
	bool isexistqueryField = false;

	for (vector<CFieldEntity>::iterator ite_2 = fieldlist.begin(); ite_2 != fieldlist.end(); ++ite_2) {
		if (ite_2->GetFieldName().CompareNoCase(queryField) == 0) {
			isexistqueryField = true;
		}
	}
	if (!isexistqueryField) {
		AfxMessageBox(_T("������Ҫ��ѯ���ֶ� ") + queryField + _T(" ��"));
		return 0;
	}

	for (vector<CFieldEntity>::iterator ite_3 = fieldlist.begin(); ite_3 != fieldlist.end(); ++ite_3) {
		if (ite_3->GetFieldName().CompareNoCase(modifiedField) == 0) {
			CString type = CTool::IntTodataType(ite_3->GetFieldType());
			if (!CTool::judgeType(type, modifiedValue, ite_3->GetFieldParam())) {
				//valid = false;
				//AfxMessageBox(_T("����ֵ������Լ����"));
				if (type == _T("BOOL")) {
					AfxMessageBox(_T("�޸ĺ��ֵ����ֶ��������Ͳ���,\n������true��false��"));
				}
				else if (type == _T("DATETIME")) {
					AfxMessageBox(_T("�޸ĺ��ֵ����ֶ��������Ͳ���,�밴��ʽyyyy-mm-dd���룡"));
				}
				else if (type == _T("VARCHAR")) {
					AfxMessageBox(_T("�޸ĺ��ֵ�������ֶ����Ƶ���󳤶ȣ�"));
				}
				else if (type == _T("INTEGER")) {
					AfxMessageBox(_T("�޸ĺ��ֵ����ֶ��������Ͳ�����������������"));
				}
				else if (type == _T("DOUBLE")) {
					AfxMessageBox(_T("�޸ĺ��ֵ����ֶ��������Ͳ���������˫��������"));
				}
				return 0;
			}
		}
	}

	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {	
		if (fitValue.CompareNoCase(recordite->GetValue(queryField))==0) {
			if (!isUnique) {
				recordite->SetValue(modifiedField,modifiedValue);
			}
			else {
				for (vector<CRecordEntity>::iterator recordite_1 = recordlist.begin(); recordite_1 != recordlist.end(); ++recordite_1) {
					if (recordite_1->GetValue(modifiedField).Compare(modifiedValue) == 0) {
						AfxMessageBox(modifiedField+_T("��ֵ")+ modifiedValue+ _T("����Ψһ��Լ����"));
						return 0;
					}
				}
				recordite->SetValue(modifiedField, modifiedValue);
			}
		}
	}
	ofstream clrfile(trdFilePath, ios::binary);
	clrfile.close();
	//д�ļ�
	ofstream outfile(trdFilePath, ios::binary | ios::app);
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1) {
			CString fieldName = ite_1->GetFieldName();
			CString recordvalue = recordite->GetValue(fieldName);
			if (ite_1->GetFieldType() == TYPE_BOOL) {
				bool tempbool;
				tempbool = CTool::CStringToBool(recordvalue);
				outfile.write((char*)(&tempbool), sizeof(bool));
			}
			else if (ite_1->GetFieldType() == TYPE_DATETIME) {
				string tempTime;
				tempTime = CT2A(recordvalue.GetString());
				outfile.write(tempTime.c_str(), CTool::getTypeStoreLength(_T("DATETIME")));
			}
			else if (ite_1->GetFieldType() == TYPE_DOUBLE) {
				double tempDouble;
				tempDouble = CTool::CStringToDouble(recordvalue);
				outfile.write((char*)(&tempDouble), sizeof(double));
			}
			else if (ite_1->GetFieldType() == TYPE_INTEGER) {
				int tempInt = CTool::CStringToInt(recordvalue);
				outfile.write((char*)(&tempInt), sizeof(int));
			}
			else if (ite_1->GetFieldType() == TYPE_VARCHAR) {
				int varcharSize = recordvalue.GetLength() + 1;
				string strtemp = CT2A(recordvalue.GetString());
				outfile.write((char*)(&varcharSize), sizeof(int));

				//writedvarchar
				outfile.write(strtemp.c_str(), varcharSize);
			}
		}
	}
	outfile.close();

	return 1;
}
