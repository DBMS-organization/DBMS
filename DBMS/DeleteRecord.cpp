// DeleteRecord.cpp: 实现文件
//

#include "pch.h"
#include "DBMS.h"
#include "DeleteRecord.h"
#include "afxdialogex.h"


// DeleteRecord 对话框

IMPLEMENT_DYNAMIC(DeleteRecord, CDialogEx)

DeleteRecord::DeleteRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DeleteRecord, pParent)
{

}

DeleteRecord::~DeleteRecord()
{
}

void DeleteRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cb);
	DDX_Control(pDX, IDC_EDIT2, ed);
}


BEGIN_MESSAGE_MAP(DeleteRecord, CDialogEx)
END_MESSAGE_MAP()


// DeleteRecord 消息处理程序
