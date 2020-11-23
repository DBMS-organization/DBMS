// AlterRecord.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "AlterRecord.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// AlterRecord 对话框

IMPLEMENT_DYNAMIC(AlterRecord, CDialogEx)

AlterRecord::AlterRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AlterRecord, pParent)
{

}

AlterRecord::~AlterRecord()
{
}

void AlterRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ed1);
	DDX_Control(pDX, IDC_EDIT2, ed2);
	DDX_Control(pDX, IDC_EDIT3, ed3);
	DDX_Control(pDX, IDC_EDIT4, ed4);
}


BEGIN_MESSAGE_MAP(AlterRecord, CDialogEx)
	ON_BN_CLICKED(IDOK, &AlterRecord::OnBnClickedOk)
END_MESSAGE_MAP()


// AlterRecord 消息处理程序

/***************************************
* 函数名：OnBnClickedOk
* 参数：无
* 返回值：无
* 描述：修改记录弹窗部分的确定按钮响应函数：获取四个参数为：所查询的字段名、对应的值、修改的字段名、修改后的值
****************************************/
void AlterRecord::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ed1.GetWindowText(queryField);
	ed2.GetWindowText(fitValue);
	ed3.GetWindowText(modifiedField);
	ed4.GetWindowText(modifiedValue);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pFileTree->OnAlterRecord(queryField, fitValue, modifiedField, modifiedValue);

	CDialogEx::OnOK();
}
