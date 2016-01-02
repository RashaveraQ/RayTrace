#ifndef __NURBS_PLANE_H
#define __NURBS_PLANE_H

class	Node;
struct	matrix;

class	NurbsPlane : public	Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(NurbsPlane)
	virtual bool newDeviceNode();
public:

	// コンストラクタ
	NurbsPlane(Node* const root = 0, const TCHAR* const Name = _T("NurbsPlane"), const sp Color = sp(255, 255, 255));
	NurbsPlane(const NurbsPlane& other);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& matrix) const {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new NurbsPlane(*this); }
};

#endif
