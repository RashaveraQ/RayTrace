#pragma once

class	DevGathering : public DevNode
{
protected:

	short	m_Member;
	DevNode*	m_Node[1000];

public:

	// コンストラクタ
	DevGathering(DevNode* const root, node_type NodeType, const char* const Name)
		: DevNode(root, NodeType, Name), m_Member( 0 ) {}
	DevGathering(const DevGathering& other);

	// デストラクタ
	~DevGathering();

	void SetRoot(DevNode* const root);
	//void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	//bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	bool AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	bool Delete(DevNode*);	
	//void Serialize(CArchive& ar);
};
