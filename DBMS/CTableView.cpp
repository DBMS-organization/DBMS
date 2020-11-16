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


	//m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 0);
	//m_ListCtrl->InsertColumn(1, CString("�ֶ���"), LVCFMT_LEFT, 100);
	//m_ListCtrl->InsertColumn(2, CString("˳��"), LVCFMT_LEFT, 0);
	//m_ListCtrl->InsertColumn(3, CString("����"), LVCFMT_LEFT, 80);
	//m_ListCtrl->InsertColumn(4, CString("����"), LVCFMT_LEFT, 50);
	//m_ListCtrl->InsertColumn(5, CString("��Сֵ"), LVCFMT_LEFT, 100);
	//m_ListCtrl->InsertColumn(6, CString("���ֵ"), LVCFMT_LEFT, 100);
	//m_ListCtrl->InsertColumn(7, CString("Ĭ��ֵ"), LVCFMT_LEFT, 100);
	//m_ListCtrl->InsertColumn(8, CString("����"), LVCFMT_LEFT, 40);
	//m_ListCtrl->InsertColumn(9, CString("�����ֵ"), LVCFMT_LEFT, 70);
	//m_ListCtrl->InsertColumn(10, CString("Ψһֵ"), LVCFMT_LEFT, 60);
	//m_ListCtrl->InsertColumn(11, CString("ע��"), LVCFMT_LEFT, 100);

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

//�ڱ�����ʾ��¼
void CTableView::displayTable(CString dbname, CString tbname)
{
	this->ClearTable();

	CString tdfFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tbname + _T(".tdf");
	CString trdFilePath = _T("DBMSROOT\\data\\") + dbname + _T("\\") + tbname + _T(".trd");

	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(dbname, tbname);
	vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);

	/*for (vector<CFieldEntity>::iterator fieldite = fieldlist.begin(); fieldite != fieldlist.end(); ++fieldite) {
		m_ListCtrl->InsertColumn(fieldite->GetFieldOrder()-1, fieldite->GetFieldName(), LVCFMT_LEFT, 100);
	}

	int round = 0;
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		for (vector<CFieldEntity>::iterator fieldite_1 = fieldlist.begin(); fieldite_1 != fieldlist.end(); ++fieldite_1) {
			if (fieldite_1->GetFieldOrder()==1) {
				m_ListCtrl->InsertItem(round, recordite->GetValue(fieldite_1->GetFieldName()));
			}
			else {
				m_ListCtrl->SetItemText(round, fieldite_1->GetFieldOrder()-1, recordite->GetValue(fieldite_1->GetFieldName()));
			}
		}
		round++;
	}*/

	m_ListCtrl->InsertColumn(0, _T("���"), LVCFMT_LEFT, 100);
	for (vector<CFieldEntity>::iterator fieldite = fieldlist.begin(); fieldite != fieldlist.end(); ++fieldite) {
		m_ListCtrl->InsertColumn(fieldite->GetFieldOrder(), fieldite->GetFieldName(), LVCFMT_LEFT, 100);
	}
	int order = 0;
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		m_ListCtrl->InsertItem(order, CTool::IntToCString(order+1));
		for (vector<CFieldEntity>::iterator fieldite_1 = fieldlist.begin(); fieldite_1 != fieldlist.end(); ++fieldite_1) {
			m_ListCtrl->SetItemText(order, fieldite_1->GetFieldOrder(), recordite->GetValue(fieldite_1->GetFieldName()));
		}
		order++;
	}
	
	AutoAdjustColumnWidth(m_ListCtrl);
}

//�ڱ�����ʾ�ֶ�������Ϣ
void CTableView::displayFieldMsg(CString dbname, CString tbname)
{
	this->ClearTable();
	m_ListCtrl->InsertColumn(0, CString("Order"), LVCFMT_LEFT, 60);
	m_ListCtrl->InsertColumn(1, CString("FieldName"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(2, CString("Type"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(3, CString("Length"), LVCFMT_LEFT, 80);
	m_ListCtrl->InsertColumn(4, CString("ModifyTime"), LVCFMT_LEFT, 180);
	m_ListCtrl->InsertColumn(5, CString("default"), LVCFMT_LEFT, 80);
	m_ListCtrl->InsertColumn(6, CString("Primary"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(7, CString("Unique"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(8, CString("Not null"), LVCFMT_LEFT, 100);

	CFieldDAO fieldDao;
	vector<CFieldEntity> fieldList = fieldDao.getFieldList(DATAFILEPATH+_T("\\")+dbname+ _T("\\")+tbname+ _T(".tdf"));
	int i=0;
	for (vector<CFieldEntity>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite,i++) {
		m_ListCtrl->InsertItem(i, CTool::IntToCString(ite->GetFieldOrder()));
		m_ListCtrl->SetItemText(i, 1, ite->GetFieldName());
		m_ListCtrl->SetItemText(i, 2, CTool::IntTodataType(ite->GetFieldType()));
		m_ListCtrl->SetItemText(i, 3, CTool::IntToCString(ite->GetFieldParam()));
		m_ListCtrl->SetItemText(i, 4, ite->GetModifyTime());
		m_ListCtrl->SetItemText(i, 5, ite->GetDefaultValue());
		m_ListCtrl->SetItemText(i, 6, CTool::BoolToCString(ite->GetPrimary()));
		m_ListCtrl->SetItemText(i, 7, CTool::BoolToCString(ite->GetUnique()));
		m_ListCtrl->SetItemText(i, 8, CTool::BoolToCString(ite->GetNotNull()));
	}

	AutoAdjustColumnWidth(m_ListCtrl);
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

void CTableView::AutoAdjustColumnWidth(CListCtrl* pListCtrl)
{
	pListCtrl->SetRedraw(FALSE);
	CHeaderCtrl* pHeader = pListCtrl->GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = pListCtrl->GetColumnWidth(i);
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = pListCtrl->GetColumnWidth(i);
		pListCtrl->SetColumnWidth(i, max(nColumnWidth, nHeaderWidth) + 5);
	}
	pListCtrl->SetRedraw(TRUE);
}

