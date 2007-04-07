#ifndef __PLANE_H
#define __PLANE_H

class	Node;
struct	matrix;

class	Plane : public	Node
{
protected:
	DECLARE_SERIAL(Plane)

public:

	// コンストラクタ
	Plane( const CRayTraceDoc* const pDoc = 0, const char* const Name = "Plane", const sp Color = sp(255,255,255) )
		: Node( pDoc, PLANE, Name, Color ) {}
	Plane( const Plane& other ) : Node( other ) {}
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const {}
	BOOL GetInfo( const sp& K, const sp& L, Info* const r_info, const Node* pOmit) const;
	BOOL IsInside( const sp& L ) const;
	void InsertItem( CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST );
	const Node*		MakeCopy() const { return new Plane( *this ); }
};

#endif
