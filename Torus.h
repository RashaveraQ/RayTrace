#ifndef __TORUS_H
#define __TORUS_H

class Node;
struct matrix;

class Torus : public Node
{
	node_type getNodeType() const { return TORUS; }
protected:
	DECLARE_SERIAL(Torus)
private:
	double	m_R;
	double	m_r;
public:
	// コンストラクタ
	Torus(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Torus", const sp Color = sp(255,255,255));
	Torus(const Torus& other) : Node(other) ,m_R(other.m_R), m_r(other.m_r) {} 

	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	BOOL GetInfo(const sp& K, const sp& L, Info& info) const;
	BOOL IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Torus(*this); }
	BOOL Edit();
	void Serialize(CArchive& ar);
};

#endif
