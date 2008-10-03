#ifndef __PLUS_H
#define __PLUS_H

class	Plus : public Gathering
{
	node_type getNodeType() { return PLUS; }
protected:
	DECLARE_SERIAL(Plus)
public:
	// コンストラクタ
	Plus( const CRayTraceDoc* const pDoc = 0, const char* const Name = "加法" ) : Gathering( pDoc, PLUS, Name ) {}
	Plus( const Plus& other ) : Gathering( other ) {}

	BOOL GetInfo(const sp& K, const sp& L, Info& info) const;
	BOOL IsInside(const sp& L) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new Plus(*this); }
	Task* MakeTask();
};

#endif
