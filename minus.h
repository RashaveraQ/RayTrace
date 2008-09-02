#ifndef __MINUS_H
#define __MINUS_H

class Minus : public BaseMinus, public Gathering
{
protected:
	DECLARE_SERIAL(Minus)

public:
	// コンストラクタ
	Minus(const CRayTraceDoc* const pDoc = 0, const char* const Name = "除法") : Gathering(pDoc, MINUS, Name) {}
	Minus(const Minus& other) : Gathering(other) {}

	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	BOOL IsInside(const sp* L);
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Minus(*this); }
	BOOL GetBoundary(sp *pCenter, double *pRadius) const;

	using Gathering::m_Member;
	using BaseMinus::m_Matrix;
	using Gathering::m_Node;
};

#endif
