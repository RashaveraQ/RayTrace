#if !defined(AFX_DLGPOLYGON_H__240E84C2_E62B_11D4_9216_000000000000__INCLUDED_)
#define AFX_DLGPOLYGON_H__240E84C2_E62B_11D4_9216_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPolygon.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPolygon ダイアログ

class CDlgPolygon : public CDialog
{
	// コンストラクション
public:
	CDlgPolygon(CWnd* pParent = NULL);   // 標準のコンストラクタ

	// ダイアログ データ
	//{{AFX_DATA(CDlgPolygon)
	enum { IDD = IDD_POLYGON };
	// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgPolygon)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

	// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgPolygon)
	// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGPOLYGON_H__240E84C2_E62B_11D4_9216_000000000000__INCLUDED_)
