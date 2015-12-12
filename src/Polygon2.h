#ifndef __POLYGON2_H
#define __POLYGON2_H

class	Node;
struct	matrix;

class Polygon2 : public Node
{
	int	m_N;
	sp	*m_P;
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
protected:
	DECLARE_SERIAL(Polygon2)
	virtual bool newDeviceNode();
public:
	Polygon2(Node* const root = 0, const char* const Name = "Polygon", const sp Color = sp(255, 255, 255));
	Polygon2(const Polygon2& other);
	~Polygon2() { delete m_P; }
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Polygon2(*this); }
	BOOL Edit();
	void Serialize(CArchive& ar);	
};

#endif
