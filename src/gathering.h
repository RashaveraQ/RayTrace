#ifndef __GATHERING_H
#define __GATHERING_H

class	Gathering : public Node
{
protected:

	short	m_Member;
	Node*	m_Node[1000];

public:

	// コンストラクタ
	Gathering(Node* const root, node_type NodeType, const char* const Name)
		: Node(root, NodeType, Name), m_Member(0) {}
	Gathering(const Gathering& other);

	// デストラクタ
	~Gathering();

	void SetRoot(Node* const root);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	bool Delete(Node*);
	void Serialize(CArchive& ar);
};

#endif
