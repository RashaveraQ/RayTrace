#include "stdafx.h"
#include "RayTrace.h"
#include "DlgMatrix.h"

IMPLEMENT_SERIAL(Plus, CObject, 1)

BOOL	Plus::IsInside( const sp& L ) const
{
	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->IsInside(m_Matrix * L))
			return TRUE;
	}

	return FALSE;
}

BOOL	Plus::GetInfo(const sp& K, const sp& L, Info* const info, const Node const * pOmit) const
{
	Info	tmp;
	int		n;
	double	l = -1;

	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->GetInfo2(K, L, &tmp, pOmit)) {
			if (l == -1 || tmp.Distance < l) {
				l = tmp.Distance;
				n = i;
			}
		}
	}

	if (l < 0)
		return FALSE;

	m_Node[n]->GetInfo2(K, L, info, pOmit);

	if (info->Material.Diffuse.r < 0)
		info->Material = m_Material;

	return TRUE;
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
