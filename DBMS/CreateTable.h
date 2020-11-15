#pragma once


// CreateTable 对话框

class CreateTable : public CDialogEx
{
	DECLARE_DYNAMIC(CreateTable)

public:
	CreateTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CreateTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateTable };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit ed;
	CString str;
	afx_msg void OnBnClickedOk();
};
