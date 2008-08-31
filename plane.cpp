#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Plane, CObject, 1)

BOOL Plane::IsInside(const sp* L)
{
	return (L->z >= 0.0);
}

BOOL Plane::GetInfo(const sp* K, const sp* L, Info* info)
{
	double	t = ( K->z ) ? -L->z / K->z : (( L->z > 0 ) ? DBL_MAX : -DBL_MAX);

	if (t <= 0)
		return FALSE;

	info->Cross = (*K) * t + (*L);
	info->Vertical = sp(0, 0, -1);
	info->Distance = t * sqrt((*K) * (*K));
	info->isEnter = (L->z < 0);
	info->Material = GetPixel(info->Cross.x, info->Cross.y).getMaterial();
	info->pNode = this;
	
	return TRUE;
}

void Plane::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD)this);
}
