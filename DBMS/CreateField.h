﻿#pragma once


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
	int type;
	int length;
	CEdit ed1;
	CEdit ed2;
	CEdit ed3;
	CComboBox cb;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButton ck1;
	CButton ck2;
	CButton ck3;
};
