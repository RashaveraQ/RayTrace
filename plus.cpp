#include "stdafx.h"
#include "RayTrace.h"
#include "DlgMatrix.h"

IMPLEMENT_SERIAL(Plus, CObject, 1)

BOOL Plus::IsInside(const sp* L)
{
	for (int i = 0; i < m_Member; i++) {
		sp l = m_Matrix * (*L);
		if (m_Node[i]->IsInside(&l))
			return TRUE;
	}
	return FALSE;
}

void Plus::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hitem = c.InsertItem(m_Name, 3, 2, hParent, hInsertAfter);
	c.SetItemData(hitem, (DWORD)this);
	Gathering::InsertItem(c, hitem, hInsertAfter);
}

/*
Node*	Plus::MakeCopy()
{
	Plus*	obj = new Plus( m_pDoc, m_Name );

	Gathering::MakeCopy( obj, this );

	return obj;
}
*/
