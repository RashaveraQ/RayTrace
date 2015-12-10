#ifndef __MINUS_H
#define __MINUS_H

class	Minus : public Gathering
{
	Boundary getBoundary();
protected:
	DECLARE_SERIAL(Minus)
	bool newDeviceNode(DevNode**);
public:
	// コンストラクタ
	Minus(Node* const root = 0, const char* const Name = "Minus") : Gathering(root, MINUS, Name) {}
	Minus(const Minus& other) : Gathering(other) {}

	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	int cmp_distance(float,float) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Minus(*this); }
	BOOL GetBoundary(sp *pCenter, float *pRadius) const;
};

#endif
