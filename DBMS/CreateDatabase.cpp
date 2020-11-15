// CreateDatabase.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "CreateDatabase.h"
#include "afxdialogex.h"


// CreateDatabase 对话框

IMPLEMENT_DYNAMIC(CreateDatabase, CDialogEx)

CreateDatabase::CreateDatabase(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateDatabase, pParent)
{

}

CreateDatabase::~CreateDatabase()
{
}

void CreateDatabase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ed);
}


BEGIN_MESSAGE_MAP(CreateDatabase, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateDatabase::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateDatabase 消息处理程序


void CreateDatabase::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ed.GetWindowText(str);
	CDialogEx::OnOK();
}
