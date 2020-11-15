#include "pch.h"
#include "CFileTree.h"

// DBView.cpp : ʵ���ļ�
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


// CDBView ���

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


// CDBView ��Ϣ�������


void CFileTree::OnInitialUpdate()
{

	// TODO: �ڴ����ר�ô����/����û���
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

	//��ʼ��ΪNULL
	m_hCurrDBItem = NULL;
	m_hCurrTBItem = NULL;

	//��ʾ���е����ݿ��б�
	this->DisplayDBList();

	m_bAddDB = FALSE;
	m_bAddTB = FALSE;
}

//���ļ��ж�ȡ���ݿ���Ϣ����ʾ������
void CFileTree::DisplayDBList()
{
	CDBDao dbDao;
	vector<CDBEntity> dbList = dbDao.getDatabasesList(SYSPATH);
	//ɾ����״ͼ�е����ݿ���ʾ
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

//�Ҽ�������ṹ
void CFileTree::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���������������
	if (!CTool::isValidFileName(dbname)) {
		AfxMessageBox(_T("���ݿ����в��ܴ���|\\/:*?<>|\""));
		return;
	}

	if (dbname == _T("")) {
		AfxMessageBox(_T("���ݿ����в���Ϊ��"));
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
		AfxMessageBox(_T("���ݿ����Ѵ��ڣ�"));
	}
	

}


void CFileTree::OnOpenDB()
{
	// TODO: �ڴ���������������
}


void CFileTree::OnDeleteDB()
{
	// TODO: �ڴ���������������
}


void CFileTree::OnCrtTable(CString tbname)
{
	// TODO: �ڴ���������������
	if (!CTool::isValidFileName(tbname)) {
		AfxMessageBox(_T("�����в��ܴ���|\\/:*?<>|\""));
		return;
	}

	if (tbname == _T("")) {
		AfxMessageBox(_T("��������Ϊ��"));
		return;
	}

	// ͬʱ�����ݿ�
	this->OnOpenDB();
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("��ѡ�����ݿ⣡"));
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
			AfxMessageBox(_T("�����Ѵ��ڣ�"));
		}
		
	}
}

void CFileTree::OnCrtField(CString fieldname, int type, int param, CString cdefault, bool primary, bool unique, bool notnull)
{
	if (fieldname == _T("")) {
		AfxMessageBox(_T("�ֶ�������Ϊ��"));
		return;
	}

	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("��ѡ�����ݱ�"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("��ѡ���"));
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
				AfxMessageBox(_T("�ֶ����Ѵ��ڣ�"));
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
		//�����ǰû��ѡ��database���򷵻ص�һ�����ݿ���
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



//�鿴���¼
void CFileTree::OnLookTable()
{
	// TODO: �ڴ���������������
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayTable();
}

//�鿴�ֶ�������Ϣ
void CFileTree::OnLookField(CString dbname,CString tbname)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayFieldMsg(dbname,tbname);
}


//���ն��ֶβ�ѯ
void CFileTree::OnConditionQuery()
{

}


void CFileTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // ���ݿ�ڵ�
	{
		m_hCurrDBItem = hItem;

		// �����ֶκͼ�¼�˵���ˢ�²˵���
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->DrawMenuBar();

		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
	}
	else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_TABLE_ITEM)   // ��ڵ�
	{
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
		GetParentFrame()->DrawMenuBar();

		//��ǰѡ��ı������ݿ�
		m_hCurrTBItem = hItem;
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);
		//��ʾ��ǰ����ֶ���Ϣ
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
	// TODO: �ڴ���������������

}


void CFileTree::OnDesignTable()
{
	// TODO: �ڴ���������������
	
}


void CFileTree::GetDBAndTableName(CString& dbName, CString& tbName)
{
	dbName = this->GetSelectedDBName();
	tbName = this->GetSelectedTBName();
}

//�ж��Ƿ��ܴ��������򷵻�true���򷵻�false
bool CFileTree::canCreateTable()
{
	if (m_hCurrDBItem == NULL) {
		AfxMessageBox(_T("��ѡ�����ݿ⣡"));
		return false;
	}
	else {
		return true;
	}
}

bool CFileTree::canCreateField()
{
	if (m_hCurrDBItem == NULL) {
		AfxMessageBox(_T("��ѡ���"));
		return false;
	}

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // ���ݿ�ڵ�
	{
		AfxMessageBox(_T("��ѡ���"));
		return false;
	}
	else {
		return true;
	}
}


void CFileTree::OnRenameDB()
{
	// TODO: �ڴ���������������
	//CDBLogic dbLogic;
	//CString oldDBName = GetSelectedDBName();
	//dbLogic.ModifyDBName(oldDBName,);
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
}


void CFileTree::OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

//���νṹ�е��Խڵ�༭����ʱִ�иú���
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


//�����ݿ�������һ�ű�
void CFileTree::OnAddTable()
{
	// TODO:  �ڴ���������������
	//this->OnCrtTable();
}


//����ָ�������ݿ�
void CFileTree::OnBackupDB()
{
	// TODO: �ڴ���������������

}

//�����ݿ��ļ����뵽�������¿�
void CFileTree::OnRecoveryDB()
{
	// TODO: �ڴ���������������
	
}

//������е�����
void CFileTree::OnClearTable()
{
	// TODO: �ڴ���������������
}


void CFileTree::OnLookLog()
{
	// TODO: �ڴ���������������
}
