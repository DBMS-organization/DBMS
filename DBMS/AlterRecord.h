#pragma once


// AlterRecord 对话框

class AlterRecord : public CDialogEx
{
	DECLARE_DYNAMIC(AlterRecord)

public:
	AlterRecord(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AlterRecord();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AlterRecord };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit ed1;
	CEdit ed2;
	CEdit ed3;
	CEdit ed4;

	CString queryField;
	CString fitValue;
	CString modifiedField;
	CString modifiedValue;
	afx_msg void OnBnClickedOk();
};
