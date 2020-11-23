#pragma once

class CFieldEntity
{
private:
	int fieldOrder;//字段顺序
	CString fieldName;//字段名
	int fieldType;//字段类型
	int fieldParam;//字段类型参数（varchar的长度）
	CString modifyTime;//修改时间
	//int Integrities;
	CString DefaultValue;//默认值
	bool Primary;//主键（主键true，非主键false）
	bool NotNull;//非空（非空true，允许空false）
	bool Unique;//唯一（唯一true，非唯一false）
	//int Integrities;//primary,notnull,unique

public:
	CFieldEntity(CString fieldname);//初始化字段实体
	~CFieldEntity(void);

	int GetFieldOrder() { return fieldOrder; };//获取字段顺序
	CString GetFieldName() { return fieldName; };//获取字段名
	int GetFieldType() { return fieldType; };//获取字段类型
	int GetFieldParam() { return fieldParam; };//获取字段类型参数
	CString GetModifyTime() { return modifyTime; };//获取修改时间
	CString GetDefaultValue() { return DefaultValue; };//获取默认值
	bool GetPrimary() { return Primary; };//获取字段是否是主键（主键true，非主键false）
	bool GetNotNull() { return NotNull; };//获取字段是否非空（非空true，允许空false）
	bool GetUnique() { return Unique; };//获取字段字段是否唯一（唯一true，非唯一false）
	//int GetIntegrities() { return Integrities; };

	void SetFieldOrder(int filed_order) { fieldOrder = filed_order; };//设置字段顺序
	void SetFieldName(CString field_name) { fieldName = field_name; };//设置字段名
	void SetFieldType(int field_type) { fieldType = field_type; };//设置字段类型
	void SetFieldParam(int field_param) { fieldParam = field_param; };//设置字段类型参数
	void SetModifyTime(CString modifytime) { modifyTime = modifytime; };//设置字段修改时间
	void SetDefaultValue(CString DefaultV) { DefaultValue = DefaultV; };//设置默认值
	void SetIntegrities(bool primary, bool unique, bool notnull);//设置约束条件（主键，非空，唯一）

};



