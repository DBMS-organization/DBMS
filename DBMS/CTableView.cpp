#include "pch.h"
#include "CTableView.h"


// TableView.cpp : ʵ���ļ�
//

#include "DBMS.h"
#include "MainFrm.h"


// CTableView

IMPLEMENT_DYNCREATE(CTableView, CListView)

CTableView::CTableView()
{

}

CTableView::~CTableView()
{
}

BEGIN_MESSAGE_MAP(CTableView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CTableView::OnNMRClick)
END_MESSAGE_MAP()


// CTableView ���

#ifdef _DEBUG
void CTableView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CTableView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTableView ��Ϣ�������


void CTableView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	m_ListCtrl = &GetListCtrl();

	LONG lStyle;
	lStyle = GetWindowLong(m_ListCtrl->m_hWnd, GWL_STYLE);   // ��ȡ��ǰ���ڷ��
	lStyle &= ~LVS_TYPEMASK;                              // �����ʾ��ʽλ
	lStyle |= LVS_REPORT;
	SetWindowLong(m_ListCtrl->m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListCtrl->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;                      // ѡ��ĳ��ʹ���и���
	dwStyle |= LVS_EX_GRIDLINES;                          // ������
	m_ListCtrl->SetExtendedStyle(dwStyle);                   // ������չ���

	//��ʼ������ͼ����Ϊ�Ƿ���ͼ
	m_curView = TABLEVIEW_VALID;


	m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 0);
	m_ListCtrl->InsertColumn(1, CString("�ֶ���"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(2, CString("˳��"), LVCFMT_LEFT, 0);
	m_ListCtrl->InsertColumn(3, CString("����"), LVCFMT_LEFT, 80);
	m_ListCtrl->InsertColumn(4, CString("����"), LVCFMT_LEFT, 50);
	m_ListCtrl->InsertColumn(5, CString("��Сֵ"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(6, CString("���ֵ"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(7, CString("Ĭ��ֵ"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(8, CString("����"), LVCFMT_LEFT, 40);
	m_ListCtrl->InsertColumn(9, CString("�����ֵ"), LVCFMT_LEFT, 70);
	m_ListCtrl->InsertColumn(10, CString("Ψһֵ"), LVCFMT_LEFT, 60);
	m_ListCtrl->InsertColumn(11, CString("ע��"), LVCFMT_LEFT, 100);

}


void CTableView::ClearTable()
{
	//��ձ����ݼ���ͷ
	m_ListCtrl->DeleteAllItems();
	while (m_ListCtrl->GetHeaderCtrl()->GetItemCount())
	{
		m_ListCtrl->DeleteColumn(0);
	}
}

void CTableView::displayTable()
{
	m_ListCtrl->InsertColumn(11, CString("����"), LVCFMT_LEFT, 100);
}

void CTableView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_iRow = pNMListView->iItem;
	m_iColumn = pNMListView->iSubItem;
	m_iCount = m_ListCtrl->GetItemCount();


	CPoint point;
	GetCursorPos(&point);
	CMenu MyMenu;
	MyMenu.LoadMenu(IDR_MAINFRAME);

	if (m_iRow >= 0 && m_iRow < m_iCount)
	{
		m_ListCtrl->SetItemState(m_iRow, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_ListCtrl->SetSelectionMark(m_iRow);

	}
	else
	{
		if (m_curView == TABLEVIEW_FIELD)
		{
			MyMenu.GetSubMenu(3)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED);
		}
		else if (m_curView == TABLEVIEW_RECORD)
		{
			MyMenu.GetSubMenu(4)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED);
		}
	}

	if (m_curView == TABLEVIEW_FIELD)
	{
		CMenu* popup = MyMenu.GetSubMenu(3);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());
	}
	else if (m_curView == TABLEVIEW_RECORD)
	{
		CMenu* popup = MyMenu.GetSubMenu(4);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());
	}

	*pResult = 0;
}



