#pragma once

class CFieldEntity
{
private:
	int fieldOrder;//�ֶ�˳��
	CString fieldName;//�ֶ���
	int fieldType;//�ֶ�����
	int fieldParam;//�ֶ����Ͳ�����varchar�ĳ��ȣ�
	CString modifyTime;//�޸�ʱ��
	//int Integrities;
	CString DefaultValue;//Ĭ��ֵ
	bool Primary;//����������true��������false��
	bool NotNull;//�ǿգ��ǿ�true�������false��
	bool Unique;//Ψһ��Ψһtrue����Ψһfalse��
	//int Integrities;//primary,notnull,unique

public:
	CFieldEntity(CString fieldname);//��ʼ���ֶ�ʵ��
	~CFieldEntity(void);

	int GetFieldOrder() { return fieldOrder; };//��ȡ�ֶ�˳��
	CString GetFieldName() { return fieldName; };//��ȡ�ֶ���
	int GetFieldType() { return fieldType; };//��ȡ�ֶ�����
	int GetFieldParam() { return fieldParam; };//��ȡ�ֶ����Ͳ���
	CString GetModifyTime() { return modifyTime; };//��ȡ�޸�ʱ��
	CString GetDefaultValue() { return DefaultValue; };//��ȡĬ��ֵ
	bool GetPrimary() { return Primary; };//��ȡ�ֶ��Ƿ�������������true��������false��
	bool GetNotNull() { return NotNull; };//��ȡ�ֶ��Ƿ�ǿգ��ǿ�true�������false��
	bool GetUnique() { return Unique; };//��ȡ�ֶ��ֶ��Ƿ�Ψһ��Ψһtrue����Ψһfalse��
	//int GetIntegrities() { return Integrities; };

	void SetFieldOrder(int filed_order) { fieldOrder = filed_order; };//�����ֶ�˳��
	void SetFieldName(CString field_name) { fieldName = field_name; };//�����ֶ���
	void SetFieldType(int field_type) { fieldType = field_type; };//�����ֶ�����
	void SetFieldParam(int field_param) { fieldParam = field_param; };//�����ֶ����Ͳ���
	void SetModifyTime(CString modifytime) { modifyTime = modifytime; };//�����ֶ��޸�ʱ��
	void SetDefaultValue(CString DefaultV) { DefaultValue = DefaultV; };//����Ĭ��ֵ
	void SetIntegrities(bool primary, bool unique, bool notnull);//����Լ���������������ǿգ�Ψһ��

};



