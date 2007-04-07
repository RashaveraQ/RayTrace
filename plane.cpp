#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Plane, CObject, 1)

BOOL	Plane::IsInside( const sp& L ) const
{
	return ( L.z >= 0.0 );
}

BOOL	Plane::GetInfo( const sp& K, const sp& L, Info* const r_info, const Node const *) const
{
	double	t = ( K.z ) ? -L.z / K.z : (( L.z > 0 ) ? DBL_MAX : -DBL_MAX);

	if ( t <= 0 )
		return FALSE;

	r_info->Cross = K * t + L;
	r_info->Vertical = sp( 0, 0, -1 );
	r_info->Distance = t * sqrt( K * K );
	r_info->isEnter = ( L.z < 0 );
	r_info->Material = GetPixel(r_info->Cross.x,r_info->Cross.y).getMaterial();
	r_info->pNode = this;
	
	return TRUE;
}

void Plane::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD)this);
}
