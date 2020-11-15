// CreateField.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "CreateField.h"
#include "afxdialogex.h"


// CreateField 对话框

IMPLEMENT_DYNAMIC(CreateField, CDialogEx)

CreateField::CreateField(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateField, pParent)
{

}

CreateField::~CreateField()
{
}

void CreateField::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, ed1);
	DDX_Control(pDX, IDC_EDIT3, ed2);
	DDX_Control(pDX, IDC_EDIT4, ed3);
	DDX_Control(pDX, IDC_COMBO1, cb);
}


BEGIN_MESSAGE_MAP(CreateField, CDialogEx)
END_MESSAGE_MAP()


// CreateField 消息处理程序




BOOL CreateField::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	cb.AddString(_T("INTEGER"));
	cb.AddString(_T("DOUBLE"));
	cb.AddString(_T("DATETIME"));
	cb.AddString(_T("VARCHAR"));
	cb.AddString(_T("BOOL"));

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
