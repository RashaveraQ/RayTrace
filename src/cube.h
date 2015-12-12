#ifndef __CUBE_H
#define __CUBE_H

class	Node;
struct	matrix;

class Cube : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(Cube)
	virtual bool newDeviceNode(DevNode**);
public:
	// コンストラクタ
	Cube(Node* const root = 0, const char* const Name = "Cube", const sp Color = sp(255, 255, 255));
	Cube(const Cube& other);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node* MakeCopy() const { return new Cube(*this); }
};

#endif
