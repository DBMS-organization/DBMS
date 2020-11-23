#include "pch.h"
#include "FieldDAO.h"
#include "FieldEntity.h"

CFieldDAO::CFieldDAO()
{
}

CFieldDAO::~CFieldDAO()
{
}

/***************************************
* 函数名：getFieldList
* 参数：tdfFilePath（tdf文件的地址）
* 返回值：返回一个vector类型的字段列表
* 描述：从文件读取字段数据，存入vector中
****************************************/
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

		int fieldnum;
		fieldnum = ceil((double)fileSize / (sizeof(int) * 3 + 128 * 2 + 20 + sizeof(bool) * 3));

		for (int i = 0; i < fieldnum; i++) {
			int order;
			char* fieldName = new char[128];
			int type;
			int param;

			char* modifytime = new char[20];
			char* defaultvalue = new char[128];

			bool primary;
			bool unique;
			bool notnull;

			infile.read((char*)&order, sizeof(int));

			infile.read(fieldName, 128);

			infile.read((char*)&type, sizeof(int));
			infile.read((char*)&param, sizeof(int));

			infile.read(modifytime, 20);
			infile.read(defaultvalue, 128);
			
			//读取bool值
			infile.read((char*)&primary, sizeof(bool));
			infile.read((char*)&unique, sizeof(bool));
			infile.read((char*)&notnull, sizeof(bool));

			CString fieldName_(fieldName);
			CString modifytime_(modifytime);
			CString defaultvalue_(defaultvalue);

			CFieldEntity fieldentity(fieldName_);
			fieldentity.SetFieldOrder(order);

			fieldentity.SetFieldName(fieldName_);

			fieldentity.SetFieldType(type);

			fieldentity.SetFieldParam(param);

			fieldentity.SetModifyTime(modifytime_);

			fieldentity.SetIntegrities(primary, unique, notnull);

			fieldentity.SetDefaultValue(defaultvalue_);

			fieldList.push_back(fieldentity);
		}
	}

	infile.close();


	return fieldList;
}
