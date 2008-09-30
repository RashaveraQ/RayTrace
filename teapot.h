#ifndef __TEAPOT_H
#define __TEAPOT_H

class	Node;
struct	sp;
struct	matrix;

class	Teapot : public	Node
{
protected:
	DECLARE_SERIAL(Teapot)
public:
	// コンストラクタ
	Teapot(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Teapot", const sp Color = sp(255,255,255));
	Teapot(const Teapot& other) : Node(other) {}

	BOOL GetInfo(const struct sp& K, const sp& L, Info& info) const;
	BOOL IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Teapot(*this); }
};

#endif
