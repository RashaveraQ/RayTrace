// ScnTreeView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CScnTreeView ビュー


class CScnTreeView : public CTreeView
{
protected:
	CScnTreeView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CScnTreeView)

private:
	CImageList	m_normal_IL,
				m_state_IL;

	BOOL		AddNode( Node* );

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CScnTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CScnTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CScnTreeView)
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddSphere();
	afx_msg void OnAddCube();
	afx_msg void OnAddMinus();
	afx_msg void OnAddMultiple();
	afx_msg void OnAddPlane();
	afx_msg void OnAddPlus();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditAfin();
	afx_msg void OnEditTexture();
	afx_msg void OnEditColor();
	afx_msg void OnEditMaterial();
	afx_msg void OnProperty();
	afx_msg void OnEditPaste();
	afx_msg void OnAddCone();
	afx_msg void OnAddCylinder();
	afx_msg void OnAddTorus();
	afx_msg void OnAddPolygon();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddTeapot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
