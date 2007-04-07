// ScnTreeView.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CScnTreeView �r���[


class CScnTreeView : public CTreeView
{
protected:
	CScnTreeView();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^
	DECLARE_DYNCREATE(CScnTreeView)

private:
	CImageList	m_normal_IL,
				m_state_IL;

	BOOL		AddNode( Node* );

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CScnTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CScnTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
