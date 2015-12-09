#ifndef __MULTIPLE_H
#define __MULTIPLE_H

class	Multiple : public Gathering
{
	Boundary getBoundary();
protected:
	DECLARE_SERIAL(Multiple)
public:
	// コンストラクタ
	Multiple(Node* const root = 0, const char* const Name = "舟ultiple") : Gathering(root, MULTIPLE, Name) {}
	Multiple(const Multiple& other) : Gathering(other) {}

	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Multiple(*this); }
};

#endif
