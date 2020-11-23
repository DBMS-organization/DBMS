#include "pch.h"
#include "FieldLogic.h"
#include "FieldDAO.h"
#include <amp.h>

using namespace std;
/*****************************************************************************
* 函数名：CFieldLogic
* 参数：dbName（数据库名）
		tableName（表名）
* 返回值：无
* 描述：接收数据库名和表名进行初始化
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
* 函数名：CreateField
* 参数：fieldname（字段名）
		type（字段类型）
		param（字段类型参数）
		cdefault（字段默认值）
		Primary（主键true，非主键false）
		NotNull（非空true，允许空false）
		Unique（唯一true，非唯一false）
* 返回值：返回0或1（0创建字段失败，1创建字段成功）
* 描述：接收字段实体信息创建字段，并将字段信息写入tdf文件
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
