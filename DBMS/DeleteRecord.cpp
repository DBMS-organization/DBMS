// DeleteRecord.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "DeleteRecord.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// DeleteRecord 对话框

IMPLEMENT_DYNAMIC(DeleteRecord, CDialogEx)

DeleteRecord::DeleteRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DeleteRecord, pParent)
{

}

DeleteRecord::~DeleteRecord()
{
}

void DeleteRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cb);
	DDX_Control(pDX, IDC_EDIT2, ed);
}


BEGIN_MESSAGE_MAP(DeleteRecord, CDialogEx)
	ON_BN_CLICKED(IDOK, &DeleteRecord::OnBnClickedOk)
END_MESSAGE_MAP()


// DeleteRecord 消息处理程序


BOOL DeleteRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CString dbname = pMainWnd->m_pFileTree->GetSelectedDBName();
	CString tbname = pMainWnd->m_pFileTree->GetSelectedTBName();

	CFieldDAO fieldDao;
	int i = 0;
	vector<CFieldEntity> fieldList = fieldDao.getFieldList(DATAFILEPATH + _T("\\") + dbname + _T("\\") + tbname + _T(".tdf"));
	for (vector<CFieldEntity>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite, i++) {
		cb.AddString(ite->GetFieldName());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void DeleteRecord::OnBnClickedOk()
{
	CString str1,str2;
	int num = cb.GetCurSel();
	cb.GetLBText(num, str1);
	ed.GetWindowText(str2);
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pFileTree->OnDelRecord();


	CDialogEx::OnOK();
}
