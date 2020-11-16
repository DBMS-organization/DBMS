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
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CreateRecord, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CreateRecord::OnNMDblclkList1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CreateRecord::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CreateRecord 消息处理程序


BOOL CreateRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_edit.ShowWindow(SW_HIDE);
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


void CreateRecord::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	LVHITTESTINFO info;
	info.pt = pNMItemActivate->ptAction;

	if (list.SubItemHitTest(&info) != -1)
	{
		m_Row = info.iItem;
		m_Col = info.iSubItem;

		//if (m_edit.m_hWnd == NULL) // editItem为一输入框控件， 
		//{
		//	RECT rect;
		//	rect.left = 0;
		//	rect.top = 0;
		//	rect.bottom = 15;
		//	rect.right = 200;
		//	m_edit.Create(WS_CHILD | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE, rect, this, 101);
		//	GetDlgItem(IDC_EDIT1)->SetFont(this->GetFont(), FALSE);
		//}
		CRect rect;
		list.GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rect);
		m_edit.SetParent(&list);
		//int iSubItemWidth = list.GetColumnWidth(m_Col);
		//rect.top += 14;
		//rect.left += 15;

		//rect.right = rect.left + iSubItemWidth;

		//rect.bottom += 14;

		m_edit.SetWindowText(list.GetItemText(info.iItem, info.iSubItem));
		m_edit.MoveWindow(rect, TRUE);
		m_edit.ShowWindow(1);
		m_edit.SetFocus();
	}


	*pResult = 0;

}


void CreateRecord::OnEnKillfocusEdit1()
{
	CString str;
	m_edit.GetWindowTextW(str);
	list.SetItemText(m_Row, m_Col, str);
	m_edit.ShowWindow(SW_HIDE);

}


