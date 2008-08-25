#ifndef __NODE_H
#define __NODE_H

#include "NodeBase.h"

class	CDlgMatrix;
class	CDlgMaterial;

class Node : public CObject, public NodeBase
{
protected:
	TARGET void updateMatrix() {
		m_Matrix = m_Move * m_Rotate * m_Scale;
	}

public:

	Node(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name, const sp Color = sp(-1,-1,-1));
	Node(const Node &other);

	virtual	~Node() {}

	// オペレーション
	TARGET virtual void SetDocument(const CRayTraceDoc* const pDoc) { m_pDoc = (const CRayTraceDoc*)pDoc; }

	void Move(eAxis axis, double d);
	void Move(POINT d);
	void Rotate(eAxis axis, double d);
	void Rotate(POINT d);
	void Scale(eAxis axis, double d);
	void MovePivot(eAxis axis, double d);

	virtual void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	virtual bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	virtual	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target) { return FALSE; }
	virtual void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	virtual void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST) = 0;
	virtual BOOL Edit();
			BOOL EditAfin();
			BOOL EditColor();
			BOOL EditMaterial();
			BOOL EditTexture();
	virtual const Node*		MakeCopy() const = 0;
	TARGET virtual	BOOL Delete( Node* ) { return FALSE; }
	virtual	void Serialize(CArchive& ar);

	// インプリメンテーション
	friend CDlgMatrix;
	friend CDlgMaterial;
	friend Geometry;
};

#endif	// __NODE_H
