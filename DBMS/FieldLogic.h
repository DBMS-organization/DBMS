#pragma once
class CFieldLogic
{
private:
	CString tdfPath;//tdf�ļ�·��
	CString DBName;//���ݿ���
	CString TableName;//����
	int order = 0;//�ֶ������ʼֵΪ0
	CString FieldName;//�ֶ���
	int FieldType;//�ֶ�����
	int FieldParam;//�ֶ����Ͳ���
	CString ModifyTime;//�ֶ��޸�ʱ��
	/*int Integrities;*/
	CString DefaultValue;//�ֶ�Ĭ��ֵ
	int Primary;//����������true��������false��
	int NotNull;//�ǿգ��ǿ�true�������false��
	int Unique;//Ψһ��Ψһtrue����Ψһfalse��

public:
	CFieldLogic(CString dbName, CString tableName);//��ʼ���ֶ��߼���
	~CFieldLogic();

	int CreateField(CString& fieldname, int type, int param, CString default, bool primary, bool unique, bool notnull);
};

