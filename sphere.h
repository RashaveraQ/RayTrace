#ifndef __SPHERE_H
#define __SPHERE_H

class	Node;
struct	sp;
struct	matrix;

class Sphere : public BaseSphere, public Node
{
protected:
	DECLARE_SERIAL(Sphere)
public:
	// コンストラクタ
	Sphere(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Sphere", const sp Color = sp(255,255,255));
	Sphere(const Sphere& other) : Node(other) {}

	BOOL IsInside(const sp* L);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Sphere(*this); }
	using BaseSphere::m_Matrix;
	using Node::m_Name;
};

#endif
