#ifndef __CYLINDER_H
#define __CYLINDER_H

class	Node;
struct	matrix;
struct	sp;
struct	Info;

class	Cylinder : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(Cylinder)
public:
	// コンストラクタ
	Cylinder(const Node* root = 0, const char* const Name = "Cylinder", const sp Color = sp(255,255,255))
		: Node(root, CYLINDER, Name, Color) {}
	Cylinder(const Cylinder& other) : Node(other) {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Cylinder(*this); }
};

#endif
