#ifndef __GATHERING_H
#define __GATHERING_H

class Gathering : public BaseGathering, public Node
{
public:
	// �R���X�g���N�^
	Gathering(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name);
	Gathering(const Gathering& other);

	// �f�X�g���N�^
	~Gathering();

	void SetDocument(const CRayTraceDoc* const pDoc);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	BOOL Delete(Node*);	
	void Serialize(CArchive& ar);
	void updateDeviceData();
	using Node::m_Matrix;
	using Node::m_Name;
};

#endif
