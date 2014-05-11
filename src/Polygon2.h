#ifndef __POLYGON2_H
#define __POLYGON2_H

class	Node;
struct	matrix;

class Polygon2 : public Node
{
	int	m_N;
	sp	*m_P;
protected:
	DECLARE_SERIAL(Polygon2)

public:
	Polygon2(const CRayTraceDoc* const pDoc = 0, const char* const Name = "Polygon", const sp Color = sp(255,255,255))
		: Node(pDoc, POLYGON, Name, Color), m_N(0), m_P(0) {}
	Polygon2(const Polygon2& other);
	~Polygon2() { delete m_P; }
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const {}
	BOOL GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	BOOL IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Polygon2(*this); }
	BOOL Edit();
	void Serialize(CArchive& ar);	
};

#endif
