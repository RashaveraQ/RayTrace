#ifndef __CONE_H
#define __CONE_H

class	Node;
struct	matrix;
struct	sp;
struct	Info;

class Cone : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(Cone)
	virtual bool newDeviceNode();
public:
	// コンストラクタ
	Cone(Node* const root = 0, const char* const Name = "Cone", const sp Color = sp(255, 255, 255));
	Cone(const Cone& other);
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Cone(*this); }
};

#endif
