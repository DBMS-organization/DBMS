#pragma once
class CFieldLogic
{
private:
	CString tdfPath;
	CString DBName;
	CString TableName;
	int order = 0;
	CString FieldName;
	int FieldType;
	int FieldParam;
	CString ModifyTime;
	/*int Integrities;*/
	CString DefaultValue;
	int Primary;
	int NotNull;
	int Unique;

public:
	CFieldLogic(CString dbName, CString tableName);
	~CFieldLogic();

	int CreateField(CString& fieldname, int type, int param, CString default, bool primary, bool unique, bool notnull);
};

