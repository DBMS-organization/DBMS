// DeleteTable.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "DeleteTable.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// DeleteTable 对话框

IMPLEMENT_DYNAMIC(DeleteTable, CDialogEx)

DeleteTable::DeleteTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DeleteTable, pParent)
{

}

DeleteTable::~DeleteTable()
{
}

void DeleteTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ed);
}


BEGIN_MESSAGE_MAP(DeleteTable, CDialogEx)
	ON_BN_CLICKED(IDOK, &DeleteTable::OnBnClickedOk)
END_MESSAGE_MAP()


// DeleteTable 消息处理程序
/***************************************
* 函数名：OnBnClickedOk
* 参数：无
* 返回值：无
* 描述：删除表弹窗部分的确定按钮响应函数:编辑框获取输入要删除的表名
* 点击确定删除表
* 设置除VARCHAR外 大小一栏无法编辑以及加入容错判断
****************************************/
/*删除表弹窗部分的ok按钮响应函数 参数为：所删除的表名*/
void DeleteTable::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ed.GetWindowText(str);
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pFileTree->OnDeleteTable(str);
	CDialogEx::OnOK();
}
