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
	DDX_Control(pDX, IDC_LIST1, m_List1);
	//m_List1.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);//设置列表控件格式(有栅格线，整行选中)
	//m_List1.SetBkColor(RGB(255, 255, 255));//设置列表控件背景为白色
	//m_List1.InsertColumn(0, _T("姓名"), LVCFMT_CENTER, 160);//添加列，并设置列的宽度为160
	//m_List1.InsertColumn(1, _T("性别"), LVCFMT_CENTER, 160);
	//m_List1.InsertColumn(2, _T("年龄"), LVCFMT_CENTER, 160);
	//m_List1.InsertItem(0, _T("张三")); m_List1.SetItemText(0, 1, _T("男")); m_List1.SetItemText(0, 2, _T("21"));//添加数据
}


BEGIN_MESSAGE_MAP(CreateRecord, CDialogEx)
END_MESSAGE_MAP()


// CreateRecord 消息处理程序


BOOL CreateRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	m_List1.GetClientRect(&rect);


	DWORD dwStyle = m_List1.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	m_List1.SetExtendedStyle(dwStyle); //设置扩展风格 



	m_List1.InsertColumn(0, _T("ID"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_List1.InsertColumn(1, _T("Name"), LVCFMT_CENTER, rect.Width() / 6, 1);
	m_List1.InsertColumn(2, _T("Sex"), LVCFMT_CENTER, rect.Width() / 6, 2);
	m_List1.InsertColumn(3, _T("Brith"), LVCFMT_CENTER, rect.Width() / 6, 3);
	m_List1.InsertColumn(4, _T("Department"), LVCFMT_CENTER, rect.Width() / 6, 4);
	m_List1.InsertColumn(5, _T("Address"), LVCFMT_CENTER, rect.Width() / 6, 5);
	// TODO:  在此添加额外的初始化
	m_List1.InsertItem(0, _T("JAVA"));
	m_List1.SetItemText(0, 1, _T("01"));
	m_List1.SetItemText(0, 2, _T("02"));

	m_List1.InsertItem(1, _T("C++"));
	m_List1.SetItemText(1, 1, _T("13"));
	m_List1.SetItemText(1, 2, _T("14"));

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
