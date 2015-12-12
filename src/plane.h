#ifndef __PLANE_H
#define __PLANE_H

class	Node;
struct	matrix;

class	Plane : public	Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(Plane)
	virtual bool newDeviceNode();
public:

	// コンストラクタ
	Plane(Node* const root = 0, const char* const Name = "Plane", const sp Color = sp(255, 255, 255));
	Plane(const Plane& other);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Plane(*this); }
};

#endif
