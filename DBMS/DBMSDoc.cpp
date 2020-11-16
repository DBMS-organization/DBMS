
// DBMSDoc.cpp: CDBMSDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DBMS.h"
#endif

#include "DBMSDoc.h"

#include <propkey.h>

#include "CRecordEntity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDBMSDoc

IMPLEMENT_DYNCREATE(CDBMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBMSDoc, CDocument)
END_MESSAGE_MAP()


// CDBMSDoc 构造/析构

CDBMSDoc::CDBMSDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CDBMSDoc::~CDBMSDoc()
{
}

BOOL CDBMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	SetTitle(_T("无数据库"));						//设置标题前半段

	CString filePath = _T("DBMSROOT");
	CreateDirectory(filePath, NULL);					//创建文件夹
	filePath = _T("DBMSROOT\\data");
	CreateDirectory(filePath, NULL);					//创建文件夹

	ofstream dbfile;
	dbfile.open("DBMSROOT\\databases.db", ios::binary | ios::app);

	dbfile.close();

	//CDBLogic dbLogic;
	//dbLogic.CreateDatabase(CString("ABC"));

	//CTableLogic tableLogic(_T("ABC"));
	//tableLogic.CreateTable(CString("table4"));
	//CFieldLogic fieldLogic(_T("ABC"), _T("table4"));
	//fieldLogic.CreateField(CString("age"), 2, 0, _T("21"), false, true, true);

	//CRecordDao::AddRecordNum(_T("db1"), _T("t1"));




	//测试添加记录
	CRecordEntity recordEntity;
	recordEntity.SetValue(_T("sname"), _T("hjhhhhh"));
	recordEntity.SetValue(_T("ssex"), _T("true"));
	recordEntity.SetValue(_T("sage"), _T("20"));
	recordEntity.SetValue(_T("num"), _T("2.333"));


	CRecordLogic recordlogic;
	recordlogic.AddRecord(_T("db1"), _T("t1"), recordEntity);

	vector<CRecordEntity> recordlist = CRecordDao::getRecordList(_T("db1"), _T("t1"));
	_cprintf("LLLLLLLLLLLLLLLLLLLLLLL    %d\n",recordlist.size());
	for (vector<CRecordEntity>::iterator recordite = recordlist.begin(); recordite != recordlist.end(); ++recordite) {
		
		CString sname = recordite->GetValue(_T("sname"));
		CString ssex = recordite->GetValue(_T("ssex"));
		CString sage = recordite->GetValue(_T("sage"));
		CString num = recordite->GetValue(_T("num"));
		string strssex = CT2A(ssex.GetString());
		string strsname = CT2A(sname.GetString());
		string strsage = CT2A(sage.GetString());
		string strnum = CT2A(num.GetString());
		_cprintf("zzzzzzzzzzzzzzzzzzzzzz  %s  %s %s %s\n",  strssex.c_str(), strsage.c_str(), strnum.c_str(), strsname.c_str());
	}

	return TRUE;
}




// CDBMSDoc 序列化

void CDBMSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDBMSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDBMSDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDBMSDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDBMSDoc 诊断

#ifdef _DEBUG
void CDBMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDBMSDoc 命令
