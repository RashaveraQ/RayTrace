#if !defined(AFX_DLGMATERIAL_H__60E64592_D3DA_11D4_BE9E_00104B17BB23__INCLUDED_)
#define AFX_DLGMATERIAL_H__60E64592_D3DA_11D4_BE9E_00104B17BB23__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMaterial.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMaterial ダイアログ

class CDlgMaterial : public CDialog
{
// コンストラクション
public:
	CDlgMaterial(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void Set( Node& );
	void Get( Node& );
	
// ダイアログ データ
	//{{AFX_DATA(CDlgMaterial)
	enum { IDD = IDD_MATERIAL };
	double	m_Reflect;
	double	m_Through;
	double	m_Refractive;
	int		m_Material;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgMaterial)
	afx_msg void OnSelchangeMaterial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGMATERIAL_H__60E64592_D3DA_11D4_BE9E_00104B17BB23__INCLUDED_)
