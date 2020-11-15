#pragma once


// CreateDatabase 对话框

class CreateDatabase : public CDialogEx
{
	DECLARE_DYNAMIC(CreateDatabase)

public:
	CreateDatabase(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CreateDatabase();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateDatabase };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit ed;
	CString str;
	afx_msg void OnBnClickedOk();
};
