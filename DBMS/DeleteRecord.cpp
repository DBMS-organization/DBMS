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

/***************************************
* 函数名：OnInitDialog
* 参数：无
* 返回值：无
* 描述：删除记录弹窗部分的初始化函数：下拉框初始化所存在的字段名称
****************************************/
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

/***************************************
* 函数名：OnBnClickedOk
* 参数：无
* 返回值：无
* 描述：删除记录弹窗部分的确定按钮响应函数：下拉框获取选中要删除的记录名，编辑框获取所要删除的记录值,
* 点击确定删除数据库
****************************************/
void DeleteRecord::OnBnClickedOk()
{
	CString str1,str2;
	int num = cb.GetCurSel();
	cb.GetLBText(num, str1);
	ed.GetWindowText(str2);
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pFileTree->OnDelRecord(str1,str2);


	CDialogEx::OnOK();
}
