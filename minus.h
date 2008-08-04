#ifndef __MINUS_H
#define __MINUS_H

class	Minus : public Gathering
{
protected:
	DECLARE_SERIAL(Minus)

public:
	// コンストラクタ
	Minus(const CRayTraceDoc* const pDoc = 0, const char* const Name = "除法") : Gathering(pDoc, MINUS, Name) {}
	Minus(const Minus& other) : Gathering(other) {}

	BOOL	AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	BOOL	GetInfo(const sp& K, const sp& L, Info& info, const Node* pOmit, const Node& viewport) const;
	BOOL	IsInside(const sp& L) const;
	int		cmp_distance(double,double) const;
	void	InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*		MakeCopy() const { return new Minus(*this ); }
	BOOL		GetBoundary(sp *pCenter, double *pRadius) const;
};

#endif
