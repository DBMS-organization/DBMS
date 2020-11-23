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
	//ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CFileTree::OnTvnBeginlabeledit)
	//ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CFileTree::OnTvnEndlabeledit)
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


/******************************************
* ��������DisplayDBList
* ��������
* ����ֵ����
* ���������ļ��ж�ȡ���ݿ���Ϣ�����ݿ⡢���ֶΣ�����ʾ������
*******************************************/
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


/***********************
 �Ҽ�������ṹ
************************/
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

/***************************************************************
* ��������OnCrtDB
* ������dbname(���ݿ����ƣ�
* ����ֵ����
* �������������ݿ��ǰ��˽ӿڣ����������Ŀ¼����������ݿ�ڵ㣬
		�����ô������ݿ��ļ��ĺ�����ͬʱ���ж����ݿ�����һЩ�ݴ��жϡ�
		�����𴴽����ݿ����־��д
****************************************************************/
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

		//д����־
		ofstream logfile;
		logfile.open("DBMSROOT\\system.log", ios::app);
		string currentTime, strDBname;
		currentTime = CT2A(CTool::GetCurrTime());
		strDBname = CT2A(dbname);
		logfile << currentTime.c_str() << "\t" << "CREATE DATABASE " << strDBname.c_str() << endl;
		logfile.close();
	}
	else {
		AfxMessageBox(_T("���ݿ����Ѵ��ڣ�"));
	}
	

}

/***************************************************************
* ��������OnCrtTable
* ������tbname(�����ƣ�
* ����ֵ����
* �������������ǰ��˽ӿڣ����������Ŀ¼������ӱ�ڵ㣬
		�����ô���������ļ��ĺ�����ͬʱ���жԱ�����һЩ
		�ݴ��жϡ������𴴽������־��д
****************************************************************/
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
	//this->OnOpenDB();
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

			//д����־
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
			AfxMessageBox(_T("�����Ѵ��ڣ�"));
		}
	}
}

/***************************************************************
* ��������OnCrtField
* ������fieldname(�ֶ����ƣ�
		type����������ת����int���ֵ��
		param������������Ϊ�ַ���ʱ���ַ��������Ƴ��ȣ�
		cdefault���ֶ�Ĭ��ֵ��
		primary���Ƿ�Ϊ��������true������false��
		unique���Ƿ�Ψһ����true������false��
		notnull���Ƿ�ǿգ���true������false��
* ����ֵ����
* �������������ǰ��˽ӿڣ����������Ŀ¼������ӱ�ڵ㣬
		�����ô���������ļ��ĺ�����ͬʱ���жԱ�����һЩ
		�ݴ��жϡ������𴴽��ֶε���־��д
****************************************************************/
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

				//д����־
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
				AfxMessageBox(_T("�ֶ����Ѵ��ڣ�"));
			}
			
		}
	}
}

/*************************************************
* ��������GetSelectedDBName
* ��������
* ����ֵ�����ݿ����֣�CString)
* �������жϵ�ǰ�û�ѡ������ݿ����ĸ������������ݿ���
**************************************************/
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

/*************************************************
* ��������GetSelectedTBName
* ��������
* ����ֵ��������CString)
* �������жϵ�ǰ�û�ѡ��ı����ĸ��������ر���
**************************************************/
CString CFileTree::GetSelectedTBName()
{
	if (this->m_hCurrTBItem)
		return m_pTreeCtrl->GetItemText(m_hCurrTBItem);
	else
		return NULL;
}


/*************************************************
* ��������OnLookTable
* ������dbname�����ݿ�����
		tbname��������
* ����ֵ����
* ��������ʾ���¼���Ҳ���еĽӿ�
**************************************************/
void CFileTree::OnLookTable(CString dbname, CString tbname)
{
	// TODO: �ڴ���������������
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayTable(dbname, tbname);
}

/*************************************************
* ��������OnLookField
* ������dbname�����ݿ�����
		tbname��������
* ����ֵ����
* ��������ʾ���ֶ�������Ϣ���Ҳ���еĽӿ�
**************************************************/
void CFileTree::OnLookField(CString dbname,CString tbname)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->displayFieldMsg(dbname,tbname);
}

/************************************
* ��������OnTvnSelchanged
* ������pNMHDR
		pResult
* ����ֵ����
* ������������л���Ŀ¼ѡ��ʱ���õĺ�����
		������µ�ǰѡ������ݿ�ڵ㣬��
		��ڵ�
*************************************/
void CFileTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();			//��ȡ������
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // ���ݿ�ڵ�
	{
		m_hCurrDBItem = hItem;

		// �����ֶκͼ�¼�˵���ˢ�²˵���
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->DrawMenuBar();

		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();

		
		pMainWnd->SetWindowText(_T("DBMS - ")+this->GetSelectedDBName());
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
* ��������GetDBAndTableName
* ������dbName
		tbName
* ����ֵ����
* ��������ȡ��ǰѡ��ı��������ñ����ڵ����ݿ���
*************************************/
void CFileTree::GetDBAndTableName(CString& dbName, CString& tbName)
{
	dbName = this->GetSelectedDBName();
	tbName = this->GetSelectedTBName();
}

/********************************************
* ��������canCreateTable
* ��������
* ����ֵ���ܷ񴴽���bool��
* ���������ݵ�ǰѡ�����Ŀ¼�еĽڵ��ж�
		�Ƿ��ܴ��������򷵻�true���򷵻�false
		�ú���һ��ֻ�ڱ�DBMSϵͳδ�����κ����ݿ�
		ʱ���ã���δ�����κ����ݿ�ʱ��Ҫ�����û�
		�ִ������ݿ⣬������������ҵ���ǰѡ���
		��Ŀ¼�ڵ��Ӧ�����ݿ⣬Ҳ�����ڸ����ݿ�
		�д�����
**********************************************/
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

/********************************************
* ��������canCreateField
* ��������
* ����ֵ���ܷ񴴽��ֶΣ�bool��
* ���������ݵ�ǰѡ�����Ŀ¼�еĽڵ��ж�
		�Ƿ��ܴ����ֶΣ����򷵻�true���򷵻�false
		ֻ�е��û�����Ŀ¼��ѡ���˱���ֶνڵ�ʱ�ŷ���true��
**********************************************/
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

/********************************************
* ��������canCreateRecord
* ��������
* ����ֵ���ܷ���Ӽ�¼��bool��
* ���������ݵ�ǰѡ�����Ŀ¼�еĽڵ��ж�
		�Ƿ�����Ӽ�¼�����򷵻�true���򷵻�false
		ֻ�е��û�����Ŀ¼��ѡ���˱�ڵ�ʱ�ŷ���true��
**********************************************/
bool CFileTree::canCreateRecord()
{
	if (m_hCurrDBItem == NULL) {
		AfxMessageBox(_T("��ѡ���"));
		return false;
	}
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM || m_pTreeCtrl->GetItemData(hItem) == DBVIEW_FIELD_ITEM)          // ���ݿ�ڵ�
	{
		AfxMessageBox(_T("��ѡ���"));
		return false;
	}
	else {
		return true;
	}
}

/*******************************************************
* ��������OnCrtRecord
* ������recordEntity����¼ʵ�壩
* ����ֵ����
* ��������Ϊǰ���ʵ����Ӽ�¼���ܵĽӿڣ�ǰ�˴����¼��ʵ�壬
		�ڸú����л�ȡ��ǰѡ������ݿ�������������������ݿ�
		������������¼ʵ����ú�˴�����¼ʱ���д�ļ������ĺ�
		�����Լ����Ҳ������ʾ����Ӽ�¼��Ľ�������������
		��¼����־��д
*********************************************************/
void CFileTree::OnCrtRecord(CRecordEntity& recordEntity)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("��ѡ�����ݱ�"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("��ѡ���"));
		}
		else {
			CRecordLogic recordlogic;
			
			if (recordlogic.AddRecord(this->GetSelectedDBName(), this->GetSelectedTBName(), recordEntity)) {
				CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
				pMainWnd->m_pTableView->displayTable(this->GetSelectedDBName(),this->GetSelectedTBName());

				//д����־
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

//ɾ����¼
/*******************************************************
* ��������OnDelRecord
* ������fieldname��ɾ����¼ʱ��ѯ���ֶΣ�
		value���ֶε�ֵ��
* ����ֵ����
* ��������Ϊǰ���ʵ��ɾ����¼���ܵĽӿڣ�ǰ�˴����ѯ���ֶ�
		���ֶε�ֵ�ڸú����л�ȡ��ǰѡ������ݿ����������
		���������ݿ�������������ѯ���ֶΡ��ֶε�ֵ���ú��
		ɾ����¼ʱ��ض�д�ļ������ĺ������Լ����Ҳ������ʾ
		��ɾ����¼��Ľ����������ɾ����¼����־��д
*********************************************************/
void CFileTree::OnDelRecord (CString fieldname, CString value)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("��ѡ�����ݱ�"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("��ѡ���"));
		}
		else {
			CString dbname, tbname;
			dbname = this->GetSelectedDBName();
			tbname = this->GetSelectedTBName();
			/*************************************
			�������ɾ����¼�ĺ���,�����Ǳ��������ݿ���
			****************************************/
			CRecordLogic recordlogic;
			if (recordlogic.DeleteRecord(dbname, tbname, fieldname, value)) {
				CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
				pMainWnd->m_pTableView->displayTable(this->GetSelectedDBName(), this->GetSelectedTBName());

				//д����־
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
				AfxMessageBox(_T("�ü�¼�����ڣ�"));
			}
		}
	}
}

//�޸ļ�¼
/*******************************************************
* ��������OnAlterRecord
* ������primaryfieldname����ѯ���ֶ�����
		primaryvalue����ѯ�ֶε�ֵ��
		fieldname��Ҫ�޸ĵ��ֶ�����
		value���޸ĺ��ֵ��
* ����ֵ����
* ��������Ϊǰ���ʵ���޸ļ�¼���ܵĽӿڣ�ǰ�˴����ѯ���ֶΡ�
		��ѯ�ֶε�ֵ����Ҫ�޸ĵ��ֶΡ��޸ĺ��ֵ���ڸú���
		�л�ȡ��ǰѡ������ݿ�������������������ݿ���������
		�Լ�����Ĳ������ú���޸ļ�¼ʱ��ض�д�ļ������ĺ�
		�����Լ����Ҳ������ʾ���޸ļ�¼��Ľ�����������޸�
		��¼����־��д��
*********************************************************/
void CFileTree::OnAlterRecord(CString primaryfieldname, CString primaryvalue, CString fieldname, CString value)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("��ѡ�����ݱ�"));
	}
	else {
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM) {
			//AfxMessageBox(_T("��ѡ���"));
		}
		else {
			CString dbname, tbname;
			dbname = this->GetSelectedDBName();
			tbname = this->GetSelectedTBName();
			
			CRecordLogic recordlogic;
			if (recordlogic.AlterRecord(dbname,tbname, primaryfieldname, primaryvalue, fieldname, value)) {
				CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
				pMainWnd->m_pTableView->displayTable(this->GetSelectedDBName(), this->GetSelectedTBName());

				//д����־
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
* ��������OnDeleteDataBase
* ������dbname��Ҫɾ�������ݿ�����
* ����ֵ����
* ��������Ϊǰ���ʵ��ɾ�����ݿ⹦�ܵĽӿڣ�ǰ�˴���Ҫɾ����
		���ݿ������������ݿ������ú��ɾ�����ݿ�ʱ��ض�д
		�ļ������ĺ������Լ��������Ŀ¼�и���ɾ�����ݿ��Ľ����
		�������޸�ɾ�����ݿ����־��д��
*********************************************************/
void CFileTree::OnDeleteDataBase(CString dbname)
{
	// TODO: �ڴ���������������
	if (!CTool::isValidFileName(dbname)) {
		AfxMessageBox(_T("���ݿ����в��ܴ���|\\/:*?<>|\""));
		return;
	}

	if (dbname == _T("")) {
		AfxMessageBox(_T("���ݿ�������Ϊ��"));
		return;
	}

	CDBLogic dbLogic;
	if (dbLogic.DeleteDatabase(dbname)) {
		DisplayDBList();

		//д����־
		ofstream logfile;
		logfile.open("DBMSROOT\\system.log", ios::app);
		string currentTime, strDBname;
		currentTime = CT2A(CTool::GetCurrTime());
		strDBname = CT2A(dbname);
		logfile << currentTime.c_str() << "\t" << "DROP DATABASE " << strDBname.c_str() << endl;
		logfile.close();
	}
	else {
		AfxMessageBox(_T("Ҫɾ�������ݿⲻ���ڣ�"));
	}
}

/*******************************************************
* ��������OnDeleteTable
* ������tablename��Ҫɾ���ı�����
* ����ֵ����
* ��������Ϊǰ���ʵ��ɾ�����ܵĽӿڣ�ǰ�˴���Ҫɾ���ı�����
		���ڸú����л�ȡ���ݿ������������ݿ������������ú��
		ɾ����ʱ��ض�д�ļ������ĺ������Լ��������Ŀ¼��
		����ɾ�����Ľ�����������޸�ɾ�������־��д��
*********************************************************/
void CFileTree::OnDeleteTable(CString tablename)
{
	if (m_hCurrDBItem == NULL) {
		//AfxMessageBox(_T("��ѡ�����ݱ�"));
	}
	else {
		CString dbname;
		dbname = this->GetSelectedDBName();
		if (!CTool::isValidFileName(dbname)) {
			AfxMessageBox(_T("�����в��ܴ���|\\/:*?<>|\""));
			return;
		}

		if (dbname == _T("")) {
			AfxMessageBox(_T("��������Ϊ��"));
			return;
		}

		CTableLogic tablelogic(dbname);
		if (tablelogic.DeleteTable(tablename)) {
			DisplayDBList();

			//д����־
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
			AfxMessageBox(_T("Ҫɾ���ı����ڣ�"));
		}

	}
}
