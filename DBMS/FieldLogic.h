#pragma once
class CFieldLogic
{
private:
	CString tdfPath;//tdf文件路径
	CString DBName;//数据库名
	CString TableName;//表名
	int order = 0;//字段数序初始值为0
	CString FieldName;//字段名
	int FieldType;//字段类型
	int FieldParam;//字段类型参数
	CString ModifyTime;//字段修改时间
	/*int Integrities;*/
	CString DefaultValue;//字段默认值
	int Primary;//主键（主键true，非主键false）
	int NotNull;//非空（非空true，允许空false）
	int Unique;//唯一（唯一true，非唯一false）

public:
	CFieldLogic(CString dbName, CString tableName);//初始化字段逻辑层
	~CFieldLogic();

	int CreateField(CString& fieldname, int type, int param, CString default, bool primary, bool unique, bool notnull);
};

