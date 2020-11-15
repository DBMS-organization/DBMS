#include "pch.h"
#include "CFileTree.h"

// DBView.cpp : 实现文件
//

#include "DBMS.h"
#include "Resource.h"
#include "MainFrm.h"

// CDBView

IMPLEMENT_DYNCREATE(CFileTree, CTreeView)

CFileTree::CFileTree()
{

}

CFileTree::~CFileTree()
{
}

BEGIN_MESSAGE_MAP(CFileTree, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CFileTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CFileTree::OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CFileTree::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CFileTree::OnTvnEndlabeledit)
END_MESSAGE_MAP()


// CDBView 诊断

#ifdef _DEBUG
void CFileTree::AssertValid() const
{
	CTreeView::AssertValid();
	CString ss;
}

#ifndef _WIN32_WCE
void CFileTree::Dump(CDumpContext& dc) const
{
	CFileTree::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBView 消息处理程序


void CFileTree::OnInitialUpdate()
{

	// TODO: 在此添加专用代码和/或调用基类
	CTreeView::OnInitialUpdate();

	//Get the treeCtrl associated with CTreeView
	m_pTreeCtrl = &GetTreeCtrl();

	DWORD dwStyles = GetWindowLong(m_pTreeCtrl->m_hWnd, GWL_STYLE);
	dwStyles |= TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_pTreeCtrl->m_hWnd, GWL_STYLE, dwStyles);


	//Associate ICON with the control
	m_treeImageList.Create(16, 16, ILC_MASK, 4, 1);
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDI_DB_ICON));
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDI_TB_ICON));
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDI_FIELD_ICON));
	m_pTreeCtrl->SetImageList(&m_treeImageList, TVSIL_NORMAL);

	//初始化为NULL
	m_hCurrDBItem = NULL;
	m_hCurrTBItem = NULL;

	//显示已有的数据库列表
	this->DisplayDBList();

	m_bAddDB = FALSE;
	m_bAddTB = FALSE;
}

//从文件中读取数据库信息并显示在树中
void CFileTree::DisplayDBList()
{
	CDBDao dbDao;
	vector<CDBEntity> dbList = dbDao.getDatabasesList(SYSPATH);
	//删除树状图中的数据库显示
	HTREEITEM hNextItem;
	HTREEITEM hPreItem = m_pTreeCtrl->GetRootItem();
	while (hPreItem != NULL)
	{
		hNextItem = m_pTreeCtrl->GetNextItem(hPreItem, TVGN_NEXT);
		m_pTreeCtrl->DeleteItem(hPreItem);
		hPreItem = hNextItem;
	}

	for (vector<CDBEntity>::iterator ite = dbList.begin(); ite != dbList.end(); ++ite)
	{
		//MessageBox(CUtil::IntegerToString(ite->GetId()),L"",MB_OK);
		HTREEITEM hRoot = m_pTreeCtrl->InsertItem(ite->GetdbName(), 0, 0, TVI_ROOT, TVI_LAST);
		m_pTreeCtrl->SetItemData(hRoot, DBVIEW_DB_ITEM);

		CTableDAO tbDao;
		vector<CTableEntity> tbList = tbDao.getTableList(ite->GetDBpath()+_T("\\") + ite->GetdbName()+ _T(".tb"));
		for (vector<CTableEntity>::iterator ite2 = tbList.begin(); ite2 != tbList.end(); ++ite2) {
			HTREEITEM hTB = m_pTreeCtrl->InsertItem(ite2->getTableName(), 1, 1, hRoot, TVI_LAST);
			m_pTreeCtrl->SetItemData(hTB, DBVIEW_TABLE_ITEM);

			CFieldDAO fieldDao;
			vector<CFieldEntity> fieldList = fieldDao.getFieldList(ite2->gettdf());
			for (vector<CFieldEntity>::iterator ite3 = fieldList.begin(); ite3 != fieldList.end(); ++ite3) {
				HTREEITEM hFIELD = m_pTreeCtrl->InsertItem(ite3->GetFieldName(), 2, 2, hTB, TVI_LAST);
				m_pTreeCtrl->SetItemData(hFIELD, DBVIEW_FIELD_ITEM);
			}
		}
	}
	
}

//右键点击树结构
void CFileTree::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint point(GetMessagePos());
	unsigned int nFlags = 0;
	m_pTreeCtrl->ScreenToClient(&point);
	HTREEITEM hItem = m_pTreeCtrl->HitTest(point, &nFlags);

	if ((nFlags & TVHT_ONITEM) && (hItem != NULL))
	{
		m_pTreeCtrl->SelectItem(hItem);
		CMenu MyMenu;
		MyMenu.LoadMenu(IDR_MAINFRAME);
		GetCursorPos(&point);
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)
		{
			CMenu* popup = MyMenu.GetSubMenu(2);
			popup->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_TABLE_ITEM)
		{
			CMenu* popup = MyMenu.GetSubMenu(3);
			popup->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
		}
	}
	*pResult = 0;
}


void CFileTree::OnCrtDB(CString dbname)
{
	// TODO: 在此添加命令处理程序代码
	if (!CTool::isValidFileName(dbname)) {
		AfxMessageBox(_T("数据库名中不能带有|\\/:*?<>|\""));
		return;
	}

	if (dbname == _T("")) {
		AfxMessageBox(_T("数据库名中不能为空"));
		return;
	}

	CDBLogic dbLogic;
	if (dbLogic.CreateDatabase(dbname)) {
		HTREEITEM hItem;
		hItem = m_pTreeCtrl->InsertItem(dbname, 0, 0, TVI_ROOT, TVI_LAST);
		if (hItem != NULL)
		{
			m_bAddDB = TRUE;
			m_pTreeCtrl->SetItemData(hItem, DBVIEW_DB_ITEM);
			//m_pTreeCtrl->EditLabel(hItem);
		}
	}
	else {
		AfxMessageBox(_T("数据库名已存在！"));
	}
	

}


void CFileTree::OnOpenDB()
{
	// TODO: 在此添加命令处理程序代码
}


void CFileTree::OnDeleteDB()
{
	// TODO: 在此添加命令处理程序代码
}


void CFileTree::OnCrtTable(CString tbname)
{
	// TODO: 在此添加命令处理程序代码
	if (!CTool::isValidFileName(tbname)) {
		AfxMessageBox(_T("表名中不能带有|\\/:*?<>|\""));
		return;
	}

	if (tbname == _T("")) {
		AfxMessageBox(_T("表名不能为空"));
		return;
	}

	// 同时打开数据库
	this->OnOpenDB();
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("请选择数据库！"));
	}
	else {
		CTableLogic tableLogic(this->GetSelectedDBName());
		if (tableLogic.CreateTable(tbname)) {
			HTREEITEM hTableItem = m_pTreeCtrl->InsertItem(tbname, 1, 1, m_hCurrDBItem, TVI_LAST);
			if (hTableItem != NULL)
			{
				m_pTreeCtrl->SetItemData(hTableItem, DBVIEW_TABLE_ITEM);
				m_pTreeCtrl->Expand(m_hCurrDBItem, TVE_EXPAND);
				m_bAddTB = TRUE;
				//m_pTreeCtrl->EditLabel(hTableItem);
			}
		}
		else {
			AfxMessageBox(_T("表名已存在！"));
		}
		
	}
}

void CFileTree::OnCrtField(CString fieldname, int type, int param, CString cdefault, bool primary, bool unique, bool notnull)
{
	if (fieldname == _T("")) {
		AfxMessageBox(_T("字段名不能为空"));
		return;
	}

	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("请选择数据表！"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("请选择表！"));
		}
		else {
			CFieldLogic fieldLogic(this->GetSelectedDBName(), this->GetSelectedTBName());
			if (fieldLogic.CreateField(fieldname, type, param, cdefault, primary, unique, notnull)) {
				HTREEITEM hFieldItem = m_pTreeCtrl->InsertItem(fieldname, 2, 2, m_hCurrTBItem, TVI_LAST);
				if (hFieldItem != NULL)
				{
					this->OnLookField(this->GetSelectedDBName(), this->GetSelectedTBName());
					m_pTreeCtrl->SetItemData(hFieldItem, DBVIEW_FIELD_ITEM);
					m_pTreeCtrl->Expand(m_hCurrTBItem, TVE_EXPAND);
					m_bAddTB = TRUE;
					//m_pTreeCtrl->EditLabel(hFieldItem);
				}
			}
			else {
				AfxMessageBox(_T("字段名已存在！"));
			}
			
		}
	}
}


CString CFileTree::GetSelectedDBName()
{
	if (m_hCurrDBItem)
	{
		return GetTreeCtrl().GetItemText(m_hCurrDBItem);
	}
	else
	{
		//如果当前没有选择database，则返回第一个数据库名
		return GetTreeCtrl().GetItemText(m_pTreeCtrl->GetRootItem());;
	}

}

CString CFileTree::GetSelectedTBName()
{
	if (this->m_hCurrTBItem)
		return m_pTreeCtrl->GetItemText(m_hCurrTBItem);
	else
		return NULL;
}



//查看表记录
void CFileTree::OnLookTable()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayTable();
}

//查看字段描述信息
void CFileTree::OnLookField(CString dbname,CString tbname)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayFieldMsg(dbname,tbname);
}


//按照多字段查询
void CFileTree::OnConditionQuery()
{

}


void CFileTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // 数据库节点
	{
		m_hCurrDBItem = hItem;

		// 禁用字段和记录菜单并刷新菜单栏
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->DrawMenuBar();

		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
	}
	else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_TABLE_ITEM)   // 表节点
	{
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
		GetParentFrame()->DrawMenuBar();

		//当前选择的表与数据库
		m_hCurrTBItem = hItem;
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);
		//显示当前表的字段信息
		this->OnDesignTable();

		this->OnLookTable();
	}
	else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_FIELD_ITEM) {
		m_hCurrFIELDItem = hItem;
		m_hCurrTBItem = m_pTreeCtrl->GetParentItem(m_hCurrFIELDItem);
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);

		this->OnLookField(this->GetSelectedDBName(),this->GetSelectedTBName());
	}

	*pResult = 1;

	string s;
	s = CT2A(GetSelectedDBName().GetString());
	_cprintf("%s\n", s.c_str());
}


void CFileTree::OnDeleteTable()
{
	// TODO: 在此添加命令处理程序代码

}


void CFileTree::OnDesignTable()
{
	// TODO: 在此添加命令处理程序代码
	
}


void CFileTree::GetDBAndTableName(CString& dbName, CString& tbName)
{
	dbName = this->GetSelectedDBName();
	tbName = this->GetSelectedTBName();
}

//判断是否能创建表，能则返回true否则返回false
bool CFileTree::canCreateTable()
{
	if (m_hCurrDBItem == NULL) {
		AfxMessageBox(_T("请选择数据库！"));
		return false;
	}
	else {
		return true;
	}
}

bool CFileTree::canCreateField()
{
	if (m_hCurrDBItem == NULL) {
		AfxMessageBox(_T("请选择表！"));
		return false;
	}

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // 数据库节点
	{
		AfxMessageBox(_T("请选择表！"));
		return false;
	}
	else {
		return true;
	}
}


void CFileTree::OnRenameDB()
{
	// TODO: 在此添加命令处理程序代码
	//CDBLogic dbLogic;
	//CString oldDBName = GetSelectedDBName();
	//dbLogic.ModifyDBName(oldDBName,);
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
}


void CFileTree::OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//树形结构中当对节点编辑结束时执行该函数
void CFileTree::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	CString name = pTVDispInfo->item.pszText;

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (name != "") {
		m_pTreeCtrl->SetItemText(hItem, name);
	}
	

	*pResult = 0;
}


//在数据库中增加一张表
void CFileTree::OnAddTable()
{
	// TODO:  在此添加命令处理程序代码
	//this->OnCrtTable();
}


//备份指定的数据库
void CFileTree::OnBackupDB()
{
	// TODO: 在此添加命令处理程序代码

}

//将数据库文件导入到创建的新库
void CFileTree::OnRecoveryDB()
{
	// TODO: 在此添加命令处理程序代码
	
}

//清除表中的数据
void CFileTree::OnClearTable()
{
	// TODO: 在此添加命令处理程序代码
}


void CFileTree::OnLookLog()
{
	// TODO: 在此添加命令处理程序代码
}
