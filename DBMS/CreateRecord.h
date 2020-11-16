#pragma once


// CreateRecord 对话框

class CreateRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CreateRecord)

public:
	CreateRecord(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CreateRecord();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateRecord };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl list;
	virtual BOOL OnInitDialog();
};
