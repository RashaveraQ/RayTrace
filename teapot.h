#ifndef __TEAPOT_H
#define __TEAPOT_H

class	Node;
struct	sp;
struct	matrix;

class Teapot : public BaseTeapot, public Node
{
protected:
	DECLARE_SERIAL(Teapot)
public:
	// コンストラクタ
	Teapot(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Teapot", const sp Color = sp(255,255,255));
	Teapot(const Teapot& other) : Node(other) {}

	BOOL IsInside(const sp* L);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Teapot(*this); }
	using BaseTeapot::m_Matrix;
	using Node::m_Name;
};

#endif
