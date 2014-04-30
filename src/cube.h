#ifndef __CUBE_H
#define __CUBE_H

class	Node;
struct	matrix;

class Cube : public Node
{
protected:
	DECLARE_SERIAL(Cube)
public:
	// コンストラクタ
	Cube(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Cube", const sp Color = sp(255,255,255))
		: Node(pDoc, CUBE, Name, Color) {}
	Cube(const Cube& other) : Node(other) {}
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	BOOL GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint) const;
	BOOL IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node* MakeCopy() const {return new Cube(*this);}
};

#endif
