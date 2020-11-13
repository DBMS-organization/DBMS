#include "pch.h"
#include "FieldDAO.h"
#include "FieldEntity.h"

CFieldDAO::CFieldDAO()
{
}

CFieldDAO::~CFieldDAO()
{
}

//从文件读取字段数据
vector<CFieldEntity>CFieldDAO::getFieldList(CString& tdfFilePath)
{

	vector<CFieldEntity> fieldList;
	ifstream infile(tdfFilePath, ios::binary);
	if (!infile) {					//文件打开异常

	}
	else {							//文件正常打开
		//infile.seekg(0, ios::cur);	//文件指针指向开头

		int fileSize;
		infile.seekg(0, ios::end);
		fileSize = infile.tellg();
		infile.seekg(0, ios::beg);
		//_cprintf("fileSize-----%d\n", fileSize);

		int fieldnum;
		fieldnum = ceil((double)fileSize / (4 * 3 + 128 * 2 + 20 + 2 * 3));


		for (int i = 0; i < fieldnum; i++) {
			char* order = new char[4];
			char* fieldName = new char[128];
			char* type = new char[4];
			char* param = new char[4];
			char* modifytime = new char[20];
			char* defaultvalue = new char[128];
			char* primary = new char[2];		//bool
			char* unique = new char[2];		//bool
			char* notnull = new char[2];		//bool

			infile.read(order, 4);
			infile.read(fieldName, 128);
			infile.read(type, 4);
			infile.read(param, 4);
			infile.read(modifytime, 20);
			infile.read(defaultvalue, 20);
			infile.read(primary, 2);
			infile.read(unique, 2);
			infile.read(notnull, 2);

			int order_ = atoi(order);
			int type_ = atoi(type);
			int param_ = atoi(param);
			CString fieldName_(fieldName);
			CString modifytime_(modifytime);
			CString defaultvalue_(defaultvalue);
			bool primary_ = false;
			if (primary[0] == '1') {
				primary_ = true;
			}
			bool unique_ = false;
			if (unique[0] == '1') {
				unique_ = true;
			}
			bool notnull_ = false;
			if (notnull[0] == '1') {
				notnull_ = true;
			}



			//_cprintf("33333333 %s\n", dbName);

			CFieldEntity fieldentity(fieldName_);
			fieldentity.SetFieldOrder(order_);
			fieldentity.SetFieldName(fieldName_);
			fieldentity.SetFieldType(type_);
			fieldentity.SetFieldParam(param_);
			fieldentity.SetModifyTime(modifytime_);
			fieldentity.SetIntegrities(primary_, unique_, notnull_);
			fieldentity.SetDefaultValue(defaultvalue_);


			fieldList.push_back(fieldentity);
			//_cprintf("222222222 %s\n", fieldList.at(0).getFieldName().GetString());
		}
	}

	infile.close();


	return fieldList;
}
