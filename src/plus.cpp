#include "stdafx.h"
#include "RayTrace.h"
#include "DlgMatrix.h"

IMPLEMENT_SERIAL(Plus, CObject, 1)

Boundary Plus::getBoundary()
{
	Boundary c1;
	for (int i = 0; i < m_Member; i++) {
		if (i == 0) {
			c1 = m_Node[0]->getBoundary2();
		} else {
			Boundary c2 = m_Node[i]->getBoundary2();
			if (c1.Center == c2.Center) {
				c1.Radius = max(c1.Radius, c2.Radius);
			} else {
				sp e = (c1.Center - c2.Center).e();
				sp p1 = c1.Center + c1.Radius * e;
				sp p2 = c2.Center - c2.Radius * e;
				c1.Center = (p1 + p2) / 2;
				c1.Radius = (p1 - p2).abs() / 2;
			}
		}
	}
	return c1;
}

BOOL Plus::IsInside( const sp& L ) const
{
	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->IsInside2(L))
			return TRUE;
	}

	return FALSE;
}

BOOL Plus::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	Info	tmp;
	int		n;
	double	l = -1;

	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->GetInfo2(K, L, tmp, pHint, fromOutSide)) {
			if (l == -1 || tmp.Distance < l) {
				l = tmp.Distance;
				n = i;
				info = tmp;
			}
		}
	}

	if (l < 0)
		return FALSE;

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

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
