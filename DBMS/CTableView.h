#pragma once
#include "afxcmn.h"

#define TABLEVIEW_VALID  0
#define TABLEVIEW_FIELD  1
#define TABLEVIEW_RECORD 2

// CTableView ��ͼ

class CTableView : public CListView
{
	DECLARE_DYNCREATE(CTableView)

protected:
	CTableView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTableView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	CListCtrl* m_ListCtrl;
public:
	virtual void OnInitialUpdate();
	int m_curView;       //�ж��ұߵı����ֶλ��Ǽ�¼

	int m_iRow;
	int m_iColumn;
	int m_iCount;


	void ClearTable();
	void displayTable(CString dbname, CString tbname);
	void displayFieldMsg(CString dbname, CString tbname);
	void AutoAdjustColumnWidth(CListCtrl* pListCtrl);//����Ӧ����
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
};


