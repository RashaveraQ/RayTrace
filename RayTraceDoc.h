// RayTraceDoc.h : CRayTraceDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAYTRACEDOC_H__B4EED83D_53CC_11D3_9AB7_000000000000__INCLUDED_)
#define AFX_RAYTRACEDOC_H__B4EED83D_53CC_11D3_9AB7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRayTraceDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CRayTraceDoc();
	DECLARE_DYNCREATE(CRayTraceDoc)
	void            InitDocument();

// アトリビュート
public:
	sp			m_Light;		//	光源の方向ベクトル
	Plus		m_Root;

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRayTraceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CRayTraceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CRayTraceDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RAYTRACEDOC_H__B4EED83D_53CC_11D3_9AB7_000000000000__INCLUDED_)
