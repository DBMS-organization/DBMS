#pragma once

#include "Global.h"
class CDBLogic
{
private:
	CString syspath = SYSPATH;//db�ļ�·��
public:
	int CreateDatabase(CString& dbName);		//�������ݿ�
	int DeleteDatabase(CString dbName);		//ɾ�����ݿ�
};

