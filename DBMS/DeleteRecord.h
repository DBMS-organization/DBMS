#pragma once


// DeleteRecord 对话框

class DeleteRecord : public CDialogEx
{
	DECLARE_DYNAMIC(DeleteRecord)

public:
	DeleteRecord(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DeleteRecord();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DeleteRecord };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox cb;
	CEdit ed;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
