#ifndef __PLUS_H
#define __PLUS_H

class	Plus : public Gathering
{
	Boundary getBoundary();
protected:
	DECLARE_SERIAL(Plus)
	bool newDeviceNode(DevNode**);
public:
	// コンストラクタ
	Plus(Node* const root = 0, const char* const Name = "Plus") : Gathering(root, PLUS, Name) {}
	Plus(const Plus& other) : Gathering(other) {}

	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Plus(*this); }
};

#endif
