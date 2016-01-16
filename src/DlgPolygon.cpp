// DlgPolygon.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "RayTrace.h"
#include "DlgPolygon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPolygon ダイアログ


CDlgPolygon::CDlgPolygon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPolygon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPolygon)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDlgPolygon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPolygon)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPolygon, CDialog)
	//{{AFX_MSG_MAP(CDlgPolygon)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPolygon メッセージ ハンドラ
