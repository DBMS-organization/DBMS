#include "pch.h"
#include "FieldLogic.h"
#include "FieldDAO.h"
#include <amp.h>

using namespace std;

CFieldLogic::CFieldLogic(CString dbName, CString tableName)
{
	this->DBName = dbName;
	this->TableName = tableName;
	this->tdfPath = _T("DBMSROOT\\data\\") + dbName + _T("\\") + tableName + _T(".tdf");
}

CFieldLogic::~CFieldLogic()
{
}

int CFieldLogic::CreateField(CString& fieldname, int type, int param, CString cdefault, bool primary, bool unique, bool notnull)
{
	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfPath);
	order = fieldlist.size() + 1;
	/*CString createtime;
	createtime = dblist->GetcreateTime();*/
	bool FieldExist = false;
	for (vector<CFieldEntity>::iterator ite = fieldlist.begin(); ite != fieldlist.end(); ++ite)
	{
		//AllocConsole();

		string strfieldname1;
		const char* chfieldname1;
		strfieldname1 = CT2A(ite->GetFieldName().GetString());
		chfieldname1 = strfieldname1.c_str();



		_cprintf("FieldMsg %s %d %d %d\n", chfieldname1, ite->GetFieldType(), ite->GetPrimary(), ite->GetNotNull());
		/*if (ite->GetPrimary()) {
			_cprintf(" primary true");
		}
		else {
			_cprintf(" primary false");
		}
		if (ite->GetNotNull()) {
			_cprintf(" notnull true\n");
		}
		else {
			_cprintf(" notnull false\n");
		}*/

		if (ite->GetFieldName() == fieldname)
		{
			FieldExist = true;
			_cprintf("true\n");
			break;
		}
	}

	if (!FieldExist) {
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


		//strorder = std::to_string(order);
		//strtype = std::to_string(type);
		//strparam = std::to_string(param);

		/*char chprimary[2], chunique[2], chnotnull[2];
		chprimary[1] = '\0';
		if (primary) chprimary[0] = '1';
		else chprimary[0] = '0';

		chunique[1] = '\0';
		if (unique) chunique[0] = '1';
		else chunique[0] = '0';

		chnotnull[1] = '\0';
		if (notnull) chnotnull[0] = '1';
		else chnotnull[0] = '0';*/

		//_cprintf("888888888888 %s %s %s %s\n", strorder.c_str(), strfieldname.c_str(), chprimary, chnotnull);


		//outfile.write(strorder.c_str(), 4);
		outfile.write((char*)(&order), sizeof(int));

		outfile.write(strfieldname.c_str(), 128);
		//outfile.write(strtype.c_str(), 4);
		outfile.write((char*)(&type), sizeof(int));

		//outfile.write(strparam.c_str(), 4);
		outfile.write((char*)(&param), sizeof(int));

		outfile.write(strtime.c_str(), 20);
		outfile.write(strdefault.c_str(), 128);

		/*outfile.write(chprimary, 2);
		outfile.write(chunique, 2);
		outfile.write(chnotnull, 2);*/
		outfile.write((char*)(&primary), sizeof(bool));
		outfile.write((char*)(&unique), sizeof(bool));
		outfile.write((char*)(&notnull), sizeof(bool));


		outfile.close();
		return 1;
	}
	else {
		return 0;
	}
}
