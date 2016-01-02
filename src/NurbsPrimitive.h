#ifndef __NURBS_PRIMITIVE_H
#define __NURBS_PRIMITIVE_H

class	Node;
struct	matrix;

#define CV_MAX_WIDTH 100
#define CV_MAX_HEIGHT 100

class NurbsPrimitive : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	bool m_IsControlVertexEditable;
	int m_ControlVertexWidth;
	int m_ControlVertexHeight;
	sp m_ControlVertex[CV_MAX_WIDTH][CV_MAX_HEIGHT];
	DECLARE_SERIAL(NurbsPrimitive)
	virtual bool newDeviceNode();
public:
	// コンストラクタ
	NurbsPrimitive();
	NurbsPrimitive(Node* const root, const TCHAR* const Name, const sp Color = sp(255, 255, 255));
	NurbsPrimitive(const NurbsPrimitive& other);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& matrix) const {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new NurbsPrimitive(*this); }
};

#endif
