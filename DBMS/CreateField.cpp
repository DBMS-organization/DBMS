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



/***************************************
* 函数名：OnInitDialog
* 参数：无
* 返回值：无
* 描述：新建字段弹窗初始化函数：下拉框设有默认的字段类型 BOOL、DATETIME、DOUBLE、INTEGER、VARVHAR
****************************************/
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

/***************************************
* 函数名：OnBnClickedOk
* 参数：无
* 返回值：无
* 描述：新建字段弹窗确定按钮响应函数:编辑框获取输入的字段属性以及单选框对象
* 设置除VARCHAR外 大小一栏无法编辑以及加入容错判断
****************************************/
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
/***************************************
* 函数名：OnBnClickedCheck1
* 参数：无
* 返回值：无
* 描述：新建字段弹窗单选框响应函数:获取选中状态,当字段为主键时另外两个框也自动选中
****************************************/
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

/***************************************
* 函数名：OnCbnSelchangeCombo1
* 参数：无
* 返回值：无
* 描述：新建字段弹窗下拉框响应函数:当创建字段时如果选VARCHAR的时候会动填一个限制长度
* 防止没填长度就成功创建字段
****************************************/
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

/***************************************
* 函数名：OnCbnEditchangeCombo1
* 参数：无
* 返回值：无
* 描述：新建字段弹窗ok按钮响应函数:当创建字段时如果选VARCHAR的时候会动填一个限制长度
* 防止没填长度就成功创建字段
****************************************/
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
