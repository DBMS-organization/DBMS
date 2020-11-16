#pragma once

class CFieldEntity
{
private:
	int fieldOrder;
	CString fieldName;
	int fieldType;
	int fieldParam;
	CString modifyTime;
	//int Integrities;
	CString DefaultValue;
	bool Primary;
	bool NotNull;
	bool Unique;
	//int Integrities;//primary,notnull,unique

public:
	CFieldEntity(CString fieldname);
	~CFieldEntity(void);

	int GetFieldOrder() { return fieldOrder; };
	CString GetFieldName() { return fieldName; };
	int GetFieldType() { return fieldType; };
	int GetFieldParam() { return fieldParam; };
	CString GetModifyTime() { return modifyTime; };
	CString GetDefaultValue() { return DefaultValue; };
	bool GetPrimary() { return Primary; };
	bool GetNotNull() { return NotNull; };
	bool GetUnique() { return Unique; };
	//int GetIntegrities() { return Integrities; };

	void SetFieldOrder(int filed_order) { fieldOrder = filed_order; };
	void SetFieldName(CString field_name) { fieldName = field_name; };
	void SetFieldType(int field_type) { fieldType = field_type; };
	void SetFieldParam(int field_param) { fieldParam = field_param; };
	void SetModifyTime(CString modifytime) { modifyTime = modifytime; };
	void SetDefaultValue(CString DefaultV) { DefaultValue = DefaultV; };
	void SetIntegrities(bool primary, bool unique, bool notnull);

};



