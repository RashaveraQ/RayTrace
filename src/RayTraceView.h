// RayTraceView.h : CRayTraceView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
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

protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CRayTraceView();
	DECLARE_DYNCREATE(CRayTraceView)

// �A�g���r���[�g
public:
	class CRayTraceDoc* GetDocument();
	fsize		m_View;
	Viewport	m_Viewport;		//	���_�J�n�I�u�W�F�N�g�̃|�C���^
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
// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRayTraceView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CRayTraceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
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

#ifndef _DEBUG  // RayTraceView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CRayTraceDoc* CRayTraceView::GetDocument()
   { return (CRayTraceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RAYTRACEVIEW_H__B4EED83F_53CC_11D3_9AB7_000000000000__INCLUDED_)
