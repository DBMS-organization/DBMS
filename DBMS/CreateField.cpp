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
	ON_BN_CLICKED(IDOK, &CreateField::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CreateField::OnBnClickedCheck1)
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

/*新建字段弹窗ok按钮响应函数 编辑框获取输入的字段属性*/
void CreateField::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK1);
	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_CHECK2);
	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_CHECK3);

	ed1.GetWindowText(ed1str);
	ed3.GetWindowText(ed4str);
	int index = cb.GetCurSel();
	cb.GetLBText(index, ed2str);
	if (ed2str == _T("VARCHAR")) {
		ed2.GetWindowText(ed3str);
		length = _ttoi(ed3str);
	}
	type = CTool::dataType(ed2str);
	isPK = pBtn1->GetCheck();
	isNULL = pBtn2->GetCheck();
	isUNIQUE = pBtn3->GetCheck();


	CDialogEx::OnOK();
}

/*新建字段弹窗单选框响应函数 确定选中状态*/
void CreateField::OnBnClickedCheck1()
{
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK1);
	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_CHECK2);
	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_CHECK3);
	int state = pBtn1->GetCheck();
	if (state == 1) {
		pBtn2->SetCheck(1);
		pBtn3->SetCheck(1);
		pBtn2->EnableWindow(false);
		pBtn3->EnableWindow(false);
	}
	else {
		pBtn2->SetCheck(0);
		pBtn3->SetCheck(0);
		pBtn2->EnableWindow(true);
		pBtn3->EnableWindow(true);
	}
}
