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
	ON_CBN_SELCHANGE(IDC_COMBO1, &CreateField::OnCbnSelchangeCombo1)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &CreateField::OnCbnEditchangeCombo1)
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
	
	ed2.EnableWindow(false);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


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

	if (!CTool::judgeType(ed2str, ed4str, length)) {
		if (ed2str == _T("BOOL")) {
			AfxMessageBox(_T("默认值与数据类型不符,\n请输入true或false！"));
		}
		else if (ed2str == _T("DATETIME")) {
			AfxMessageBox(_T("默认值与数据类型不符,请按格式yyyy-mm-dd输入！"));
		}
		else if (ed2str == _T("VARCHAR")) {
			AfxMessageBox(_T("默认值超过VARCHAR的最大长度！"));
		}
		else if (ed2str == _T("INTEGER")) {
			AfxMessageBox(_T("默认值不是整型数！"));
		}
		else if (ed2str == _T("DOUBLE")) {
			AfxMessageBox(_T("默认值不是双精度数！"));
		}
		
		ed3.SetWindowText(_T(""));
	}
	else {
		CDialogEx::OnOK();
	}
	
}


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


void CreateField::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString CSType;
	int nIndex = cb.GetCurSel();
	cb.GetLBText(nIndex, CSType);

	ed2.EnableWindow(false);
	ed2.SetWindowText(_T(""));
	if (CSType == _T("VARCHAR")) {
		ed2.EnableWindow(true);
		ed2.SetWindowText(_T("20"));
	}
}


void CreateField::OnCbnEditchangeCombo1()
{
	//AfxMessageBox(_T("EDITCHANGE"));
	// TODO: 在此添加控件通知处理程序代码
	CString CSType;
	cb.GetWindowText(CSType);


	if (CSType == _T("VARCHAR")) {
		ed2.EnableWindow(true);
		ed2.SetWindowText(_T("20"));
	}
	else {
		ed2.EnableWindow(false);
		ed2.SetWindowText(_T(""));
	}
}
