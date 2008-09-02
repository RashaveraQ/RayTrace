#ifndef __PLANE_H
#define __PLANE_H

class	Node;
struct	matrix;

class Plane : public BasePlane, public Node
{
protected:
	DECLARE_SERIAL(Plane)

public:

	// コンストラクタ
	Plane(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Plane", const sp Color = sp(255,255,255))
		: Node( pDoc, PLANE, Name, Color) {}
	Plane(const Plane& other) : Node(other) {}
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const {}
	BOOL IsInside(const sp* L);
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Plane(*this); }
	using Node::m_Name;
	using BasePlane::m_Matrix;
};

#endif
