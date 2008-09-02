#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Multiple, CObject, 1)

BOOL Multiple::IsInside(const sp* L)
{
	if (m_Member == 0)
		return FALSE;

	for (int i = 0; i < m_Member; i++) {
		sp l = m_Matrix * (*L);
		if (!m_Node[i]->IsInside(&l))
			return FALSE;
	}
	return TRUE;
}


void Multiple::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hitem = c.InsertItem(m_Name, 5, 4, hParent, hInsertAfter);
	c.SetItemData(hitem, (DWORD)this);
	Gathering::InsertItem(c, hitem, hInsertAfter);
}

/*
Node*	Multiple::MakeCopy()
{
	Multiple*	obj = new Multiple( m_pDoc, m_Name );

	Gathering::MakeCopy( obj, this );

	return obj;
}
*/