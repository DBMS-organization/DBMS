#pragma once

#include <afxcview.h>
#include "CRecordEntity.h"

#define DBVIEW_DB_ITEM		0
#define DBVIEW_TABLE_ITEM	1
#define DBVIEW_FIELD_ITEM	2

// CDBView ��ͼ

class CFileTree : public CTreeView
{
	DECLARE_DYNCREATE(CFileTree)

protected:
	CFileTree();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFileTree();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
private:
	CImageList m_treeImageList;
public:

	HTREEITEM m_hCurrDBItem;
	HTREEITEM m_hCurrTBItem;
	HTREEITEM m_hCurrFIELDItem;

	CTreeCtrl* m_pTreeCtrl;

	bool m_bAddDB;  //״̬�Ƿ�Ϊ������ݿ�
	bool m_bAddTB;  //״̬�Ƿ�Ϊ��ӱ�

	CString GetSelectedDBName();//�õ���ǰ��ѡ������ݿ���
	CString GetSelectedTBName();//�õ���ǰ��ѡ��ı���
	void DisplayDBList();		//��ʾ���ݿ��б�

	void GetDBAndTableName(CString& dbName, CString& tbName);
	bool canCreateTable();
	bool canCreateField();
	bool canCreateRecord();

	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCrtDB(CString dbname);
	afx_msg void OnCrtTable(CString dbname);
	afx_msg void OnCrtField(CString dbname, int type, int param, CString cdefault, bool primary, bool unique, bool notnull);
	afx_msg void OnLookTable(CString dbname, CString tbname);
	afx_msg void OnLookField(CString dbname, CString tbname);
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRenameDB();
	afx_msg void OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnCrtRecord(CRecordEntity& recordEntity);
	afx_msg void OnDelRecord(CString fieldname, CString value);

	afx_msg void OnAlterRecord(CString primaryfieldname, CString primaryvalue, CString fieldname, CString value);
	afx_msg void OnDeleteDataBase(CString dbname);
	afx_msg void OnDeleteTable(CString tablename);
};





