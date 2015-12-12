#ifndef __SPHERE_H
#define __SPHERE_H

class	Node;
struct	sp;
struct	matrix;

class Sphere : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(Sphere)
	virtual bool newDeviceNode();
public:
	// コンストラクタ
	Sphere(Node* const root = 0, const char* const Name = "Sphere", const sp Color = sp(255, 255, 255));
	Sphere(const Sphere& other);

	bool GetInfo(const struct sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Sphere(*this); }
};

#endif
