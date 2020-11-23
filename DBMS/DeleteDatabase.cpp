// DeleteDatabase.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "DeleteDatabase.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// DeleteDatabase 对话框

IMPLEMENT_DYNAMIC(DeleteDatabase, CDialogEx)

DeleteDatabase::DeleteDatabase(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DeleteDatabase, pParent)
{

}

DeleteDatabase::~DeleteDatabase()
{
}

void DeleteDatabase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ed);
}


BEGIN_MESSAGE_MAP(DeleteDatabase, CDialogEx)
	ON_BN_CLICKED(IDOK, &DeleteDatabase::OnBnClickedOk)
END_MESSAGE_MAP()


// DeleteDatabase 消息处理程序
/***************************************
* 函数名：OnBnClickedOk
* 参数：无
* 返回值：无
* 描述：删除数据库弹窗部分的确定按钮响应函数：编辑框获取所要删除的数据库名,点击确定删除数据库
****************************************/
void DeleteDatabase::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ed.GetWindowText(str);
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pFileTree->OnDeleteDataBase(str);
	CDialogEx::OnOK();
}
