// CreateTable.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "CreateTable.h"
#include "afxdialogex.h"


// CreateTable 对话框

IMPLEMENT_DYNAMIC(CreateTable, CDialogEx)

CreateTable::CreateTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateTable, pParent)
{

}

CreateTable::~CreateTable()
{
}

void CreateTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ed);
}


BEGIN_MESSAGE_MAP(CreateTable, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateTable::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateTable 消息处理程序
/***************************************
* 函数名：OnBnClickedOk
* 参数：无
* 返回值：无
* 描述：新建表弹窗ok按钮响应函数：编辑框获取输入的表名
****************************************/
void CreateTable::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ed.GetWindowText(str);
	CDialogEx::OnOK();
}
