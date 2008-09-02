#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Plane, CObject, 1)

BOOL Plane::IsInside(const sp* L)
{
	return (L->z >= 0.0);
}

void Plane::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD)this);
}
