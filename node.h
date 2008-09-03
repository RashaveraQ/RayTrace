#ifndef __NODE_H
#define __NODE_H

#include "BaseNode.h"

class Node : public BaseNode, public CObject
{
protected:
	CRayTraceDoc* m_pDoc;
public:
	Node(const CRayTraceDoc* pDoc, node_type NodeType, const char* Name, const sp Color = sp(-1,-1,-1))
		: BaseNode(NodeType, Name, Color)
	{
		m_pDoc = (CRayTraceDoc*)pDoc;
		//MakeMemoryDCfromTextureFileName();
		updateDeviceData();
	}
	Node(const Node &other);

	virtual	~Node();

	// オペレーション
	virtual void SetDocument(const CRayTraceDoc* const pDoc) { m_pDoc = (CRayTraceDoc*)pDoc; }
	sp GetColor2(const sp& K, const sp& L, int nest);
	sp GetPixel(double x, double y) const;
	//virtual BOOL GetInfo(const sp* K, const sp* L, Info* info) = 0;
	virtual BOOL IsInside(const sp* L) = 0;

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
	virtual	BOOL Delete( Node* ) { return FALSE; }
	virtual	void Serialize(CArchive& ar);
	virtual void updateDeviceData();
	void updateMatrix();

	// インプリメンテーション
	friend CDlgMatrix;
	friend CDlgMaterial;
	friend Geometry;
};

#endif	// __NODE_H
