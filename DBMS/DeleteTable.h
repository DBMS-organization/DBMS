#pragma once


// DeleteTable 对话框

class DeleteTable : public CDialogEx
{
	DECLARE_DYNAMIC(DeleteTable)

public:
	DeleteTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DeleteTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DeleteTable };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit ed;
	CString str;
	afx_msg void OnBnClickedOk();
};
