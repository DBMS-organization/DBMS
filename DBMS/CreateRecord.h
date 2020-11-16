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
	CRecordEntity recordEntity;
	int m_Row = -1, m_Col = -1;
	bool valid = true;
	CListCtrl list;
	virtual BOOL OnInitDialog();
	void AutoAdjustColumnWidth(CListCtrl* pListCtrl);


	
	CEdit m_edit;
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnBnClickedOk();
};
