#ifndef __MULTIPLE_H
#define __MULTIPLE_H

class Multiple : public BaseMultiple, public Gathering
{
protected:
	DECLARE_SERIAL(Multiple)
public:
	// コンストラクタ
	Multiple(const CRayTraceDoc* const pDoc = 0, const char* const Name = "乗法") : Gathering(pDoc, MULTIPLE, Name) {}
	Multiple(const Multiple& other) : Gathering(other) {}

	BOOL IsInside(const sp* L);
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Multiple(*this); }

	using Gathering::m_Member;
	using Gathering::m_Matrix;
	using Gathering::m_Node;
	using Gathering::m_Name;
};

#endif
