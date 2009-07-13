// RayTraceDoc.cpp : CRayTraceDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "RayTrace.h"

#include "RayTraceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc

IMPLEMENT_DYNCREATE(CRayTraceDoc, CDocument)

BEGIN_MESSAGE_MAP(CRayTraceDoc, CDocument)
	//{{AFX_MSG_MAP(CRayTraceDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc クラスの構築/消滅

CRayTraceDoc::CRayTraceDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CRayTraceDoc::~CRayTraceDoc()
{
}

BOOL CRayTraceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	InitDocument();

	return TRUE;
}

void CRayTraceDoc::InitDocument()
{
	m_Root.SetDocument( this );
	m_Light = sp( 1,1,1 );
	d_pTask = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc シリアライゼーション

void CRayTraceDoc::Serialize(CArchive& ar)
{
	WORD	id;

	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
		ar >> id;
	}
	m_Root.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc クラスの診断

#ifdef _DEBUG
void CRayTraceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRayTraceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc コマンド

BOOL CRayTraceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	InitDocument();
	
	return TRUE;
}

void CRayTraceDoc::UpdateAllViews(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// タスクの初期化
	taskIndex_ = 0;

	m_Root.MakeTask(matrix());

	CDocument::UpdateAllViews(pSender,lHint,pHint);
}
