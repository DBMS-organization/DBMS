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
	//ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CFileTree::OnTvnBeginlabeledit)
	//ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CFileTree::OnTvnEndlabeledit)
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


/******************************************
* 函数名：DisplayDBList
* 参数：无
* 返回值：无
* 描述：从文件中读取数据库信息（数据库、表、字段）并显示在树中
*******************************************/
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


/***********************
 右键点击树结构
************************/
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

/***************************************************************
* 函数名：OnCrtDB
* 参数：dbname(数据库名称）
* 返回值：无
* 描述：创建数据库的前后端接口，负责在左侧目录树中添加数据库节点，
		并调用创建数据库文件的函数，同时具有对数据库名的一些容错判断。
		还负责创建数据库的日志填写
****************************************************************/
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

		//写入日志
		ofstream logfile;
		logfile.open("DBMSROOT\\system.log", ios::app);
		string currentTime, strDBname;
		currentTime = CT2A(CTool::GetCurrTime());
		strDBname = CT2A(dbname);
		logfile << currentTime.c_str() << "\t" << "CREATE DATABASE " << strDBname.c_str() << endl;
		logfile.close();
	}
	else {
		AfxMessageBox(_T("数据库名已存在！"));
	}
	

}

/***************************************************************
* 函数名：OnCrtTable
* 参数：tbname(表名称）
* 返回值：无
* 描述：创建表的前后端接口，负责在左侧目录树中添加表节点，
		并调用创建表相关文件的函数，同时具有对表名的一些
		容错判断。还负责创建表的日志填写
****************************************************************/
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
	//this->OnOpenDB();
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

			//写入日志
			ofstream logfile;
			logfile.open("DBMSROOT\\system.log", ios::app);
			string currentTime, strDBname,strTBname;
			currentTime = CT2A(CTool::GetCurrTime());
			strDBname = CT2A(this->GetSelectedDBName());
			strTBname = CT2A(tbname);
			logfile << currentTime.c_str() << "\t" << "ON DATABASE "<<strDBname.c_str()<<" CREATE TABLE " << strTBname.c_str() << endl;
			logfile.close();
		}
		else {
			AfxMessageBox(_T("表名已存在！"));
		}
	}
}

/***************************************************************
* 函数名：OnCrtField
* 参数：fieldname(字段名称）
		type（数据类型转换成int后的值）
		param（当数据类型为字符串时，字符串的限制长度）
		cdefault（字段默认值）
		primary（是否为主键，是true，不是false）
		unique（是否唯一，是true，不是false）
		notnull（是否非空，是true，不是false）
* 返回值：无
* 描述：创建表的前后端接口，负责在左侧目录树中添加表节点，
		并调用创建表相关文件的函数，同时具有对表名的一些
		容错判断。还负责创建字段的日志填写
****************************************************************/
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

				//写入日志
				ofstream logfile;
				logfile.open("DBMSROOT\\system.log", ios::app);
				string currentTime, strDBname, strTBname,strFieldname;
				currentTime = CT2A(CTool::GetCurrTime());
				strDBname = CT2A(this->GetSelectedDBName());
				strTBname = CT2A(this->GetSelectedTBName());
				strFieldname = CT2A(fieldname);
				logfile << currentTime.c_str() << "\t" << "ON DATABASE " << strDBname.c_str() << " TABLE " << strTBname.c_str() <<" CREATE FIELD "<<strFieldname.c_str()<< endl;
				logfile.close();
			}
			else {
				AfxMessageBox(_T("字段名已存在！"));
			}
			
		}
	}
}

/*************************************************
* 函数名：GetSelectedDBName
* 参数：无
* 返回值：数据库名字（CString)
* 描述：判断当前用户选择的数据库是哪个，并返回数据库名
**************************************************/
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

/*************************************************
* 函数名：GetSelectedTBName
* 参数：无
* 返回值：表名（CString)
* 描述：判断当前用户选择的表是哪个，并返回表名
**************************************************/
CString CFileTree::GetSelectedTBName()
{
	if (this->m_hCurrTBItem)
		return m_pTreeCtrl->GetItemText(m_hCurrTBItem);
	else
		return NULL;
}


/*************************************************
* 函数名：OnLookTable
* 参数：dbname（数据库名）
		tbname（表名）
* 返回值：无
* 描述：显示表记录到右侧表中的接口
**************************************************/
void CFileTree::OnLookTable(CString dbname, CString tbname)
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayTable(dbname, tbname);
}

/*************************************************
* 函数名：OnLookField
* 参数：dbname（数据库名）
		tbname（表名）
* 返回值：无
* 描述：显示表字段描述信息到右侧表中的接口
**************************************************/
void CFileTree::OnLookField(CString dbname,CString tbname)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayFieldMsg(dbname,tbname);
}

/************************************
* 函数名：OnTvnSelchanged
* 参数：pNMHDR
		pResult
* 返回值：无
* 描述：鼠标点击切换树目录选择时调用的函数，
		负责更新当前选择的数据库节点，与
		表节点
*************************************/
void CFileTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();			//获取主界面
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // 数据库节点
	{
		m_hCurrDBItem = hItem;

		// 禁用字段和记录菜单并刷新菜单栏
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->DrawMenuBar();

		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();

		
		pMainWnd->SetWindowText(_T("DBMS - ")+this->GetSelectedDBName());
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

		this->OnLookTable(this->GetSelectedDBName(), this->GetSelectedTBName());

		pMainWnd->SetWindowText(_T("DBMS - ")+this->GetSelectedDBName()+_T(" / ")+this->GetSelectedTBName());
	}
	else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_FIELD_ITEM) {
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->DrawMenuBar();

		m_hCurrFIELDItem = hItem;
		m_hCurrTBItem = m_pTreeCtrl->GetParentItem(m_hCurrFIELDItem);
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);

		this->OnLookField(this->GetSelectedDBName(),this->GetSelectedTBName());
	}

	*pResult = 1;
}

/************************************
* 函数名：GetDBAndTableName
* 参数：dbName
		tbName
* 返回值：无
* 描述：获取当前选择的表名，及该表所在的数据库名
*************************************/
void CFileTree::GetDBAndTableName(CString& dbName, CString& tbName)
{
	dbName = this->GetSelectedDBName();
	tbName = this->GetSelectedTBName();
}

/********************************************
* 函数名：canCreateTable
* 参数：无
* 返回值：能否创建表（bool）
* 描述：根据当前选择的树目录中的节点判断
		是否能创建表，能则返回true否则返回false
		该函数一般只在本DBMS系统未创建任何数据库
		时有用，当未创建任何数据库时需要提醒用户
		现创建数据库，其他情况总能找到当前选择的
		树目录节点对应的数据库，也就能在该数据库
		中创建表
**********************************************/
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

/********************************************
* 函数名：canCreateField
* 参数：无
* 返回值：能否创建字段（bool）
* 描述：根据当前选择的树目录中的节点判断
		是否能创建字段，能则返回true否则返回false
		只有当用户在树目录中选择了表或字段节点时才返回true。
**********************************************/
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

/********************************************
* 函数名：canCreateRecord
* 参数：无
* 返回值：能否添加记录（bool）
* 描述：根据当前选择的树目录中的节点判断
		是否能添加记录，能则返回true否则返回false
		只有当用户在树目录中选择了表节点时才返回true。
**********************************************/
bool CFileTree::canCreateRecord()
{
	if (m_hCurrDBItem == NULL) {
		AfxMessageBox(_T("请选择表！"));
		return false;
	}
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM || m_pTreeCtrl->GetItemData(hItem) == DBVIEW_FIELD_ITEM)          // 数据库节点
	{
		AfxMessageBox(_T("请选择表！"));
		return false;
	}
	else {
		return true;
	}
}

/*******************************************************
* 函数名：OnCrtRecord
* 参数：recordEntity（记录实体）
* 返回值：无
* 描述：作为前后端实现添加记录功能的接口，前端传入记录的实体，
		在该函数中获取当前选择的数据库名与表名，并根据数据库
		名、表名、记录实体调用后端创建记录时相关写文件操作的函
		数，以及在右侧表中显示表添加记录后的结果。还负责添加
		记录的日志填写
*********************************************************/
void CFileTree::OnCrtRecord(CRecordEntity& recordEntity)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("请选择数据表！"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("请选择表！"));
		}
		else {
			CRecordLogic recordlogic;
			
			if (recordlogic.AddRecord(this->GetSelectedDBName(), this->GetSelectedTBName(), recordEntity)) {
				CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
				pMainWnd->m_pTableView->displayTable(this->GetSelectedDBName(),this->GetSelectedTBName());

				//写入日志
				ofstream logfile;
				logfile.open("DBMSROOT\\system.log", ios::app);
				string currentTime, strDBname, strTBname, strRecordmsg;
				currentTime = CT2A(CTool::GetCurrTime());
				strDBname = CT2A(this->GetSelectedDBName());
				strTBname = CT2A(this->GetSelectedTBName());
				strRecordmsg = "";

				map < CString, CString > recordvalues = recordEntity.GetValues();
				map<CString, CString>::iterator iter;
				CString recordMsg = _T("");
				for (iter = recordvalues.begin(); iter != recordvalues.end(); iter++) {
					recordMsg += _T("  ")+ iter->first + _T(" = ") + iter->second;
				}
				strRecordmsg = CT2A(recordMsg);
				logfile << currentTime.c_str() << "\t" << "ON DATABASE " << strDBname.c_str() << " TABLE " << strTBname.c_str() << " ADD RECORD " << strRecordmsg.c_str() << endl;
				logfile.close();
			}
		}

	}
	
}

//删除记录
/*******************************************************
* 函数名：OnDelRecord
* 参数：fieldname（删除记录时查询的字段）
		value（字段的值）
* 返回值：无
* 描述：作为前后端实现删除记录功能的接口，前端传入查询的字段
		与字段的值在该函数中获取当前选择的数据库名与表名，
		并根据数据库名、表名、查询的字段、字段的值调用后端
		删除记录时相关读写文件操作的函数，以及在右侧表中显示
		表删除记录后的结果。还负责删除记录的日志填写
*********************************************************/
void CFileTree::OnDelRecord (CString fieldname, CString value)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("请选择数据表！"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("请选择表！"));
		}
		else {
			CString dbname, tbname;
			dbname = this->GetSelectedDBName();
			tbname = this->GetSelectedTBName();
			/*************************************
			在这调用删除记录的函数,上面是表名和数据库名
			****************************************/
			CRecordLogic recordlogic;
			if (recordlogic.DeleteRecord(dbname, tbname, fieldname, value)) {
				CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
				pMainWnd->m_pTableView->displayTable(this->GetSelectedDBName(), this->GetSelectedTBName());

				//写入日志
				ofstream logfile;
				logfile.open("DBMSROOT\\system.log", ios::app);
				string currentTime, strDBname, strTBname, strRecordmsg;
				currentTime = CT2A(CTool::GetCurrTime());
				strDBname = CT2A(this->GetSelectedDBName());
				strTBname = CT2A(this->GetSelectedTBName());
				strRecordmsg = CT2A(_T("WHERE ")+ fieldname+_T(" = ")+ value);;

				logfile << currentTime.c_str() << "\t" << "ON DATABASE " << strDBname.c_str() << " TABLE " << strTBname.c_str() << " DELETE RECORD " << strRecordmsg.c_str() << endl;
				logfile.close();
			}
			else {
				AfxMessageBox(_T("该记录不存在！"));
			}
		}
	}
}

//修改记录
/*******************************************************
* 函数名：OnAlterRecord
* 参数：primaryfieldname（查询的字段名）
		primaryvalue（查询字段的值）
		fieldname（要修改的字段名）
		value（修改后的值）
* 返回值：无
* 描述：作为前后端实现修改记录功能的接口，前端传入查询的字段、
		查询字段的值、将要修改的字段、修改后的值，在该函数
		中获取当前选择的数据库名与表名，并根据数据库名、表名
		以及传入的参数调用后端修改记录时相关读写文件操作的函
		数，以及在右侧表中显示表修改记录后的结果。还负责修改
		记录的日志填写。
*********************************************************/
void CFileTree::OnAlterRecord(CString primaryfieldname, CString primaryvalue, CString fieldname, CString value)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("请选择数据表！"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("请选择表！"));
		}
		else {
			CString dbname, tbname;
			dbname = this->GetSelectedDBName();
			tbname = this->GetSelectedTBName();
			
			CRecordLogic recordlogic;
			if (recordlogic.AlterRecord(dbname,tbname, primaryfieldname, primaryvalue, fieldname, value)) {
				CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
				pMainWnd->m_pTableView->displayTable(this->GetSelectedDBName(), this->GetSelectedTBName());

				//写入日志
				ofstream logfile;
				logfile.open("DBMSROOT\\system.log", ios::app);
				string currentTime, strDBname, strTBname, strRecordmsg;
				currentTime = CT2A(CTool::GetCurrTime());
				strDBname = CT2A(this->GetSelectedDBName());
				strTBname = CT2A(this->GetSelectedTBName());
				strRecordmsg = CT2A(_T("WHERE ") + primaryfieldname + _T(" = ") + primaryvalue +_T("  FIELDNAME = ")+ fieldname+ _T("  NEW = ")+ value);

				logfile << currentTime.c_str() << "\t" << "ON DATABASE " << strDBname.c_str() << " TABLE " << strTBname.c_str() << " MODIFY RECORD " << strRecordmsg.c_str() << endl;
				logfile.close();
			}
		}

	}
}

/*******************************************************
* 函数名：OnDeleteDataBase
* 参数：dbname（要删除的数据库名）
* 返回值：无
* 描述：作为前后端实现删除数据库功能的接口，前端传入要删除的
		数据库名，根据数据库名调用后端删除数据库时相关读写
		文件操作的函数，以及在左侧树目录中更新删除数据库后的结果。
		还负责修改删除数据库的日志填写。
*********************************************************/
void CFileTree::OnDeleteDataBase(CString dbname)
{
	// TODO: 在此添加命令处理程序代码
	if (!CTool::isValidFileName(dbname)) {
		AfxMessageBox(_T("数据库名中不能带有|\\/:*?<>|\""));
		return;
	}

	if (dbname == _T("")) {
		AfxMessageBox(_T("数据库名不能为空"));
		return;
	}

	CDBLogic dbLogic;
	if (dbLogic.DeleteDatabase(dbname)) {
		DisplayDBList();

		//写入日志
		ofstream logfile;
		logfile.open("DBMSROOT\\system.log", ios::app);
		string currentTime, strDBname;
		currentTime = CT2A(CTool::GetCurrTime());
		strDBname = CT2A(dbname);
		logfile << currentTime.c_str() << "\t" << "DROP DATABASE " << strDBname.c_str() << endl;
		logfile.close();
	}
	else {
		AfxMessageBox(_T("要删除的数据库不存在！"));
	}
}

/*******************************************************
* 函数名：OnDeleteTable
* 参数：tablename（要删除的表名）
* 返回值：无
* 描述：作为前后端实现删除表功能的接口，前端传入要删除的表名，
		并在该函数中获取数据库名，根据数据库名及表名调用后端
		删除表时相关读写文件操作的函数，以及在左侧树目录中
		更新删除表后的结果。还负责修改删除表的日志填写。
*********************************************************/
void CFileTree::OnDeleteTable(CString tablename)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("请选择数据表！"));
	}
	else {
		CString dbname;
		dbname = this->GetSelectedDBName();
		if (!CTool::isValidFileName(dbname)) {
			AfxMessageBox(_T("表名中不能带有|\\/:*?<>|\""));
			return;
		}

		if (dbname == _T("")) {
			AfxMessageBox(_T("表名不能为空"));
			return;
		}

		CTableLogic tablelogic(dbname);
		if (tablelogic.DeleteTable(tablename)) {
			DisplayDBList();

			//写入日志
			ofstream logfile;
			logfile.open("DBMSROOT\\system.log", ios::app);
			string currentTime, strDBname, strTBname;
			currentTime = CT2A(CTool::GetCurrTime());
			strDBname = CT2A(dbname);
			strTBname = CT2A(tablename);
			logfile << currentTime.c_str() << "\t" << "ON DATABASE " << strDBname.c_str() << " DROP TABLE " << strTBname.c_str() << endl;
			logfile.close();
		}
		else {
			AfxMessageBox(_T("要删除的表不存在！"));
		}

	}
}
