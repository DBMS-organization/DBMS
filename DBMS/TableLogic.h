#pragma once
#include "Global.h"


class CTableLogic
{
private:
	CString tbPath;//tb�ļ�·��
	CString DBName;//���ݿ���

public:
	CTableLogic(CString dbName);//��ʼ�����߼���
	~CTableLogic();

	int CreateTable(CString& tablename);//���ݱ�������һ���±�
	int DeleteTable(CString tablename);//ɾ����
};

