#include "stdafx.h"

IMPLEMENT_SERIAL(Minus, CObject, 1)

BOOL Minus::AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target)
{
	if (m_Member >= 2)
		return FALSE;

	return Gathering::AddNode(c, SelectItem, Target);
}

BOOL Minus::IsInside(const sp& L) const
{
	return ((m_Member >= 1 && m_Node[0]->IsInside(m_Matrix * L)) 
			  && !( m_Member >= 2 && m_Node[1]->IsInside(m_Matrix * L)));
}

BOOL Minus::GetInfo(const sp& K, const sp& L, Info& info, const Node* pOmit, const Node& viewport) const
{
	Info	l_info;
	Info	r_info;
	sp		l = L;
	int		left, right;

	if (!(m_Member >= 1 && m_Node[0]->GetInfo2(K, l, l_info, pOmit, viewport)))
		return FALSE;
	do {
		left  = (m_Member >= 1) ? m_Node[0]->GetInfo2(K, l, l_info, pOmit, viewport) : 0;
		right = (m_Member >= 2) ? m_Node[1]->GetInfo2(K, l, r_info, pOmit, viewport) : 0;
		
		if (left == 0 && right == 0)
			return FALSE;

		// ç∂Ç™ÅAâEÇÊÇËãﬂÇ¢èÍçáÅB
		if (cmp_distance(l_info.Distance, r_info.Distance) < 0) {
			l = l_info.Cross;
			info.Material = l_info.Material;
		} else {
			l = r_info.Cross;
			info.Material = r_info.Material;
		}
	
	} while (!IsInside(m_Matrix * l));

	info.isEnter = 1;
	info.Distance = sqrt((L-l)*(L-l));
	info.Cross = l;

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

	return TRUE;
}

int	Minus::cmp_distance(double a, double b) const
{
	if (a >= 0) {
		if (b >= 0)
			return (a < b) ? -1 : 1;
		else
			return -1;
	} else {
		if (b >= 0)
			return 1;
		else
			return 0;
	}
}

void Minus::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hitem = c.InsertItem(m_Name, 15, 14, hParent, hInsertAfter);
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