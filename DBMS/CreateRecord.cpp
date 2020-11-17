// CreateRecord.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "CreateRecord.h"
#include "afxdialogex.h"
#include "FieldDAO.h"
#include "CFileTree.h"
#include "FieldLogic.h"
#include "MainFrm.h"
#include "CRecordEntity.h"

// CreateRecord 对话框

IMPLEMENT_DYNAMIC(CreateRecord, CDialogEx)

CreateRecord::CreateRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateRecord, pParent)
{

}

CreateRecord::~CreateRecord()
{
}

void CreateRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, list);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CreateRecord, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CreateRecord::OnNMDblclkList1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CreateRecord::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDOK, &CreateRecord::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateRecord 消息处理程序


BOOL CreateRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_edit.ShowWindow(SW_HIDE);
	CRect rect;
	list.GetClientRect(&rect);//获得当前listcontrol的宽度

	DWORD dwStyle = list.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	list.SetExtendedStyle(dwStyle); //设置扩展风格 

	// 插入列并设置每一列的宽度
	list.InsertColumn(0, _T("Order"), LVCFMT_CENTER, rect.Width() / 8, 0);
	list.InsertColumn(1, _T("FieldName"), LVCFMT_CENTER, rect.Width() / 8, 1);
	list.InsertColumn(2, _T("Type"), LVCFMT_CENTER, rect.Width() / 8, 2);
	list.InsertColumn(3, _T("Length"), LVCFMT_CENTER, rect.Width() / 8, 3);
	list.InsertColumn(4, _T("Primary"), LVCFMT_CENTER, rect.Width() / 8, 4);
	list.InsertColumn(5, _T("Unique"), LVCFMT_CENTER, rect.Width() / 8, 5);
	list.InsertColumn(6, _T("Not null"), LVCFMT_CENTER, rect.Width() / 8, 6);
	list.InsertColumn(7, _T("Value"), LVCFMT_CENTER, rect.Width() / 8, 7);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CString dbname= pMainWnd->m_pFileTree->GetSelectedDBName();
	CString tbname= pMainWnd->m_pFileTree->GetSelectedTBName();
	
	CFieldDAO fieldDao;
	int i = 0;
	vector<CFieldEntity> fieldList = fieldDao.getFieldList(DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".tdf"));
	for (vector<CFieldEntity>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite, i++) {
		list.InsertItem(i, CTool::IntToCString(ite->GetFieldOrder()));
		list.SetItemText(i, 1, ite->GetFieldName());
		list.SetItemText(i, 2, CTool::IntTodataType(ite->GetFieldType()));
		list.SetItemText(i, 3, CTool::IntToCString(ite->GetFieldParam()));
		list.SetItemText(i, 4, CTool::BoolToCString(ite->GetPrimary()));
		list.SetItemText(i, 5, CTool::BoolToCString(ite->GetUnique()));
		list.SetItemText(i, 6, CTool::BoolToCString(ite->GetNotNull()));
	}


	/*list.InsertItem(0, _T("JAVA"));
	list.SetItemText(0, 1, _T("01"));
	list.SetItemText(0, 2, _T("02"));

	list.InsertItem(1, _T("C++"));
	list.SetItemText(1, 1, _T("13"));
	list.SetItemText(1, 2, _T("14"));

	list.InsertItem(2, _T("PHP"));
	list.SetItemText(2, 1, _T("22"));
	list.SetItemText(2, 2, _T("23"));*/

	// TODO:  在此添加额外的初始化

	AutoAdjustColumnWidth(&list);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CreateRecord::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	LVHITTESTINFO info;
	info.pt = pNMItemActivate->ptAction;

	if (list.SubItemHitTest(&info) != -1)
	{
		m_Row = info.iItem;
		m_Col = info.iSubItem;

		//if (m_edit.m_hWnd == NULL) // editItem为一输入框控件， 
		//{
		//	RECT rect;
		//	rect.left = 0;
		//	rect.top = 0;
		//	rect.bottom = 15;
		//	rect.right = 200;
		//	m_edit.Create(WS_CHILD | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE, rect, this, 101);
		//	GetDlgItem(IDC_EDIT1)->SetFont(this->GetFont(), FALSE);
		//}
		CRect rect;
		list.GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rect);
		m_edit.SetParent(&list);
		//int iSubItemWidth = list.GetColumnWidth(m_Col);
		//rect.top += 14;
		//rect.left += 15;

		//rect.right = rect.left + iSubItemWidth;

		//rect.bottom += 14;

		m_edit.SetWindowText(list.GetItemText(info.iItem, info.iSubItem));
		m_edit.MoveWindow(rect, TRUE);
		m_edit.ShowWindow(1);
		m_edit.SetFocus();

		CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
		CString dbname = pMainWnd->m_pFileTree->GetSelectedDBName();
		CString tbname = pMainWnd->m_pFileTree->GetSelectedTBName();
		CString tdfFilePath = DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".tdf");
		vector<CFieldEntity> fieldlist = CFieldDAO::getFieldList(tdfFilePath);
		
		

		
		//读取每一个字段信息
		/*for (vector<CFieldEntity>::iterator ite_1 = fieldlist.begin(); ite_1 != fieldlist.end(); ++ite_1)
		{*/
			
		//}
		
	}


	*pResult = 0;

}


void CreateRecord::OnEnKillfocusEdit1()
{
	CString str;
	m_edit.GetWindowTextW(str);
	list.SetItemText(m_Row, m_Col, str);
	m_edit.ShowWindow(SW_HIDE);

	
	CString fieldName = _T("");
	CString fieldValue = _T("");
	CString type = _T("");
	fieldName = list.GetItemText(m_Row, 1);
	fieldValue = list.GetItemText(m_Row, m_Col);
	type = list.GetItemText(m_Row, 2);

	string s1, s2,s3;
	s1 = CT2A(fieldName);
	s2 = CT2A(fieldValue);
	s3 = CT2A(type);

	valid = true;
	/*if (fieldName == _T("VARCHAR")) {
		int len = CTool::CStringToInt(list.GetItemText(m_Row, 3));
		_cprintf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT int s %d %s \n", CTool::CStringToInt(list.GetItemText(m_Row, 3)), list.GetItemText(m_Row, 3));
		if (!CTool::judgeType(type, fieldValue, len)) {
			valid = false;
			AfxMessageBox(_T("输入值不符合约束！"));
			list.SetItemText(m_Row, m_Col, _T(""));
		}
		else {
			recordEntity.SetValue(fieldName, fieldValue);
		}
	}
	else {
		if (!CTool::judgeType(type, fieldValue)) {
			valid = false;
			AfxMessageBox(_T("输入值不符合约束！"));
			list.SetItemText(m_Row, m_Col, _T(""));
		}
		else {
			recordEntity.SetValue(fieldName, fieldValue);
		}
	}*/

	int len = CTool::CStringToInt(list.GetItemText(m_Row, 3));
	_cprintf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT int s %d %s \n", CTool::CStringToInt(list.GetItemText(m_Row, 3)), list.GetItemText(m_Row, 3));
	_cprintf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT s1, s2,s3  %s %s %s\n", s1.c_str(), s2.c_str(),s3.c_str());
		


	if (!CTool::judgeType(type, fieldValue, len)) {
		valid = false;
		AfxMessageBox(_T("输入值不符合约束！"));
		list.SetItemText(m_Row, m_Col, _T(""));
	}
	else {
		recordEntity.SetValue(fieldName, fieldValue);
	}
	
}

void CreateRecord::AutoAdjustColumnWidth(CListCtrl* pListCtrl)
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




void CreateRecord::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (valid) {
		CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
		pMainWnd->m_pFileTree->OnCrtRecord(recordEntity);
	}
	

	CDialogEx::OnOK();
}
