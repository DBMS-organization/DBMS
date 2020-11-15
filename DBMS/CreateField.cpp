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
	DDX_Control(pDX, IDC_CHECK1, ck1);
	DDX_Control(pDX, IDC_CHECK2, ck2);
	DDX_Control(pDX, IDC_CHECK3, ck3);
}


BEGIN_MESSAGE_MAP(CreateField, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateField::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateField 消息处理程序




BOOL CreateField::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	cb.AddString(_T("BOOL"));
	cb.AddString(_T("DATETIME"));
	cb.AddString(_T("DOUBLE"));
	cb.AddString(_T("INTEGER"));
	cb.AddString(_T("VARCHAR"));
	

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CreateField::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ed1.GetWindowText(ed1str);
	ed2.GetWindowText(ed2str);
	ed3.GetWindowText(ed3str);

	type = cb.GetCurSel();
	length = _ttoi(ed2str);



	CDialogEx::OnOK();
}
