#ifndef __PLUS_H
#define __PLUS_H

class	Plus : public Gathering
{
	Boundary getBoundary();
protected:
	DECLARE_SERIAL(Plus)
	virtual bool newDeviceNode();
public:
	// コンストラクタ
	Plus(Node* const root = 0, const TCHAR* const Name = _T("Plus"));
	Plus(const Plus& other);

	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Plus(*this); }
};

#endif
