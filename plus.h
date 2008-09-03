#ifndef __PLUS_H
#define __PLUS_H

class Plus : public BasePlus, public Gathering
{
protected:
	DECLARE_SERIAL(Plus)
public:
	// コンストラクタ
	Plus(const CRayTraceDoc* const pDoc = 0, const char* const Name = "加法") : Gathering(pDoc, PLUS, Name) {}
	Plus(const Plus& other) : Gathering(other) {}

	BOOL IsInside(const sp* L);
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Plus(*this); }
	
	using BasePlus::m_Rotate;
	using BasePlus::m_Move;
	using BasePlus::m_Scale;
	using Gathering::m_Matrix;
	using Gathering::m_Name;
	using BasePlus::getMatrix;
	using BasePlus::GetInfo;
	using Gathering::m_Node;
	using Gathering::m_Member;
};

#endif
