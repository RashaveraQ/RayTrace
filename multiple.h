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

	using BaseMultiple::m_Member;
	using BaseMultiple::m_Matrix;
	using BaseMultiple::m_Node;
	using BaseMultiple::m_Name;
};

#endif
