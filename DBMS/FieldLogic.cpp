#include "pch.h"
#include "FieldLogic.h"
#include "FieldDAO.h"
#include <amp.h>

using namespace std;
/*****************************************************************************
* ��������CFieldLogic
* ������dbName�����ݿ�����
		tableName��������
* ����ֵ����
* �������������ݿ����ͱ������г�ʼ��
******************************************************************************/
CFieldLogic::CFieldLogic(CString dbName, CString tableName)
{
	this->DBName = dbName;
	this->TableName = tableName;
	this->tdfPath = _T("DBMSROOT\\data\\") + dbName + _T("\\") + tableName + _T(".tdf");
}

CFieldLogic::~CFieldLogic()
{
}

/********************************************************
* ��������CreateField
* ������fieldname���ֶ�����
		type���ֶ����ͣ�
		param���ֶ����Ͳ�����
		cdefault���ֶ�Ĭ��ֵ��
		Primary������true��������false��
		NotNull���ǿ�true�������false��
		Unique��Ψһtrue����Ψһfalse��
* ����ֵ������0��1��0�����ֶ�ʧ�ܣ�1�����ֶγɹ���
* �����������ֶ�ʵ����Ϣ�����ֶΣ������ֶ���Ϣд��tdf�ļ�
*********************************************************/
int CFieldLogic::CreateField(CString& fieldname, int type, int param, CString cdefault, bool primary, bool unique, bool notnull)
{
	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfPath);
	order = fieldlist.size() + 1;
	/*CString createtime;
	createtime = dblist->GetcreateTime();*/
	bool FieldExist = false;
	for (vector<CFieldEntity>::iterator ite = fieldlist.begin(); ite != fieldlist.end(); ++ite)
	{

		string strfieldname1;
		const char* chfieldname1;
		strfieldname1 = CT2A(ite->GetFieldName().GetString());
		chfieldname1 = strfieldname1.c_str();

		if (ite->GetFieldName() == fieldname)
		{
			FieldExist = true;
			break;
		}
	}

	if (!FieldExist) {
		vector<CRecordEntity> recordlist = CRecordDao::getRecordList(DBName, TableName);
		for (vector<CRecordEntity>::iterator ite = recordlist.begin(); ite != recordlist.end(); ++ite) {
			ite->SetValue(fieldname, cdefault);
		}

		ofstream outfile(tdfPath, ios::binary | ios::app);

		CFieldEntity field(fieldname);
		field.SetDefaultValue(cdefault);
		field.SetFieldName(fieldname);
		field.SetFieldOrder(order);
		field.SetFieldParam(param);
		field.SetFieldType(type);
		field.SetIntegrities(primary, unique, notnull);

		string strfieldname, strorder, strtype, strparam, strtime, strdefault, strprimary, strunique, strnotnull;

		//string
		strfieldname = CT2A(fieldname.GetString());
		strtime = CT2A(field.GetModifyTime().GetString());
		strdefault = CT2A(cdefault.GetString());


		outfile.write((char*)(&order), sizeof(int));
		outfile.write(strfieldname.c_str(), 128);
		outfile.write((char*)(&type), sizeof(int));
		outfile.write((char*)(&param), sizeof(int));
		outfile.write(strtime.c_str(), 20);
		outfile.write(strdefault.c_str(), 128);
		outfile.write((char*)(&primary), sizeof(bool));
		outfile.write((char*)(&unique), sizeof(bool));
		outfile.write((char*)(&notnull), sizeof(bool));

		outfile.close();

		CRecordDao::reWriteRecord(DBName, TableName, recordlist);

		return 1;
	}
	else {
		return 0;
	}
}
