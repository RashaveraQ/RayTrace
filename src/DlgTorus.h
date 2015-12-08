#if !defined(AFX_DLGTORUS_H__6073BB82_DB16_11D4_BEAD_00104B17BB23__INCLUDED_)
#define AFX_DLGTORUS_H__6073BB82_DB16_11D4_BEAD_00104B17BB23__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTorus.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTorus ダイアログ

class CDlgTorus : public CDialog
{
// コンストラクション
public:
	CDlgTorus(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgTorus)
	enum { IDD = IDD_TORUS };
	float	m_R;
	float	m_r;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgTorus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgTorus)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGTORUS_H__6073BB82_DB16_11D4_BEAD_00104B17BB23__INCLUDED_)
