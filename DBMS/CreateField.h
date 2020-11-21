#pragma once


// CreateField 对话框

class CreateField : public CDialogEx
{
	DECLARE_DYNAMIC(CreateField)

public:
	CreateField(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CreateField();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateField };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ed1str;
	CString ed2str;
	CString ed3str;
	CString ed4str;
	bool isPK, isNULL, isUNIQUE;
	int type;
	int length = 0;
	CEdit ed1;
	CEdit ed2;
	CEdit ed3;
	CComboBox cb;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnEditchangeCombo1();
};
