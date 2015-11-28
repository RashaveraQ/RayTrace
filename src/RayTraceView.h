// RayTraceView.h : CRayTraceView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAYTRACEVIEW_H__B4EED83F_53CC_11D3_9AB7_000000000000__INCLUDED_)
#define AFX_RAYTRACEVIEW_H__B4EED83F_53CC_11D3_9AB7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CRayTraceView : public CView
{
	LPDIRECT3DDEVICE9       m_pd3dDevice;	// Our rendering device
	CListGeometry	m_lstGeometry;
	D3DXVECTOR3 m_vEyePt;
	D3DXVECTOR3 m_vLookatPt;
	void UpdateDevice();

protected: // シリアライズ機能のみから作成します。
	CRayTraceView();
	DECLARE_DYNCREATE(CRayTraceView)

// アトリビュート
public:
	class CRayTraceDoc* GetDocument();
	fsize		m_View;
	Viewport	m_Viewport;		//	視点開始オブジェクトのポインタ
	Node*		m_SelectedNode;
	CSize		m_ClientSize;
	COLORREF*	m_ColorRefs;
	void*		m_deviceAllocateMemory;

	enum {
		eD3DWireFrame = 0,
		eD3DFlatShading,
		eD3DGouraudShading,
		eRayTrace,
		eWireFrame,
		eWireFrameWithRayTrace,
	} m_ViewMode;

private:
	int Go_ahead(int& X, int& Y, int& S, int& X0, int& Y0, CSize& cs, int MAX);
	int			m_StartX, m_StartY, m_NowX, m_NowY, m_NowSize;
	BOOL		m_Job;
	BOOL		m_Iconized;
	CDC			m_MemoryDC;
	int			m_TimerID;

	struct {
		enum eType Type;
		enum eAxis Axis;
	} m_Manipulator;

	BOOL		m_Alt;
	POINT		m_AltStart;
	void GetVectorFromPoint(sp& k, sp& l, int px, int py);
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRayTraceView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CRayTraceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CRayTraceView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewRaytrace();
	afx_msg void OnUpdateViewRaytrace(CCmdUI* pCmdUI);
	afx_msg void OnViewWireframe();
	afx_msg void OnUpdateViewWireframe(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewD3dwireframe();
	afx_msg void OnUpdateViewD3dwireframe(CCmdUI* pCmdUI);
	afx_msg void OnViewFlatshading();
	afx_msg void OnUpdateViewFlatshading(CCmdUI* pCmdUI);
	afx_msg void OnViewGouraudshading();
	afx_msg void OnUpdateViewGouraudshading(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

friend Node;
friend class CScnTreeView;
public:
	afx_msg void OnViewWireframeWithRaytrace();
};

#ifndef _DEBUG  // RayTraceView.cpp ファイルがデバッグ環境の時使用されます。
inline CRayTraceDoc* CRayTraceView::GetDocument()
   { return (CRayTraceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RAYTRACEVIEW_H__B4EED83F_53CC_11D3_9AB7_000000000000__INCLUDED_)
