#ifndef __TORUS_H
#define __TORUS_H

class	Node;
struct	matrix;

class Torus : public BaseTorus, public Node
{
protected:
	DECLARE_SERIAL(Torus)
public:
	// コンストラクタ
	Torus(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Torus", const sp Color = sp(255,255,255));
	Torus(const Torus& other) : BaseTorus(other.m_R, other.m_r), Node(other) {} 

	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	BOOL IsInside(const sp* L);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Torus(*this); }
	BOOL Edit();
	void Serialize(CArchive& ar);
	using BaseTorus::m_Matrix;
	using Node::m_Name;
};

#endif
