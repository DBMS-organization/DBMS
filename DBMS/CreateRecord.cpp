// CreateRecord.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "CreateRecord.h"
#include "afxdialogex.h"


// CreateRecord 对话框

IMPLEMENT_DYNAMIC(CreateRecord, CDialogEx)

CreateRecord::CreateRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateRecord, pParent)
{

}

CreateRecord::~CreateRecord()
{
}

void CreateRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, list);
}


BEGIN_MESSAGE_MAP(CreateRecord, CDialogEx)
END_MESSAGE_MAP()


// CreateRecord 消息处理程序


BOOL CreateRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	list.GetClientRect(&rect);//获得当前listcontrol的宽度

	DWORD dwStyle = list.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	list.SetExtendedStyle(dwStyle); //设置扩展风格 

	// 插入列并设置每一列的宽度
	list.InsertColumn(0, _T("ID"), LVCFMT_CENTER, rect.Width() / 6, 0);
	list.InsertColumn(1, _T("Name"), LVCFMT_CENTER, rect.Width() / 6, 1);
	list.InsertColumn(2, _T("Sex"), LVCFMT_CENTER, rect.Width() / 6, 2);
	list.InsertColumn(3, _T("Brith"), LVCFMT_CENTER, rect.Width() / 6, 3);
	list.InsertColumn(4, _T("Department"), LVCFMT_CENTER, rect.Width() / 6, 4);
	list.InsertColumn(5, _T("Address"), LVCFMT_CENTER, rect.Width() / 6, 5);




	list.InsertItem(0, _T("JAVA"));
	list.SetItemText(0, 1, _T("01"));
	list.SetItemText(0, 2, _T("02"));

	list.InsertItem(1, _T("C++"));
	list.SetItemText(1, 1, _T("13"));
	list.SetItemText(1, 2, _T("14"));


	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
