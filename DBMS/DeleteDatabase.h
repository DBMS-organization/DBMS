#pragma once


// DeleteDatabase 对话框

class DeleteDatabase : public CDialogEx
{
	DECLARE_DYNAMIC(DeleteDatabase)

public:
	DeleteDatabase(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DeleteDatabase();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DeleteDatabase };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit ed;
	CString str;

	afx_msg void OnBnClickedOk();
};
