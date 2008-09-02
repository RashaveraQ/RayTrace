#include "stdafx.h"

IMPLEMENT_SERIAL(Minus, CObject, 1)

BOOL Minus::AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target)
{
	if (m_Member >= 2)
		return FALSE;

	return Gathering::AddNode(c, SelectItem, Target);
}

BOOL Minus::IsInside(const sp* L)
{
	sp l = m_Matrix * (*L);
	return ((m_Member >= 1 && m_Node[0]->IsInside(&l)) 
			  && !(m_Member >= 2 && m_Node[1]->IsInside(&l)));
}

void Minus::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hitem = c.InsertItem(((BaseMinus*)this)->m_Name, 15, 14, hParent, hInsertAfter);
	c.SetItemData(hitem, (DWORD)this);
	Gathering::InsertItem(c, hitem, hInsertAfter);
}

/*
Node*	Minus::MakeCopy()
{
	Minus*	obj = new Minus( m_pDoc, m_Name );

	Gathering::MakeCopy( obj, this );

	return obj;
}
*/