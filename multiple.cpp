#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Multiple, CObject, 1)

BOOL Multiple::IsInside(const sp& L) const
{
	if (!m_Member)
		return FALSE;

	for (int i = 0; i < m_Member; i++) {
		if (!m_Node[i]->IsInside(m_Matrix * L))
			return FALSE;
	}
	return TRUE;
}

BOOL Multiple::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint) const
{
	Info	tmp;
	double	l = -1;
	int		i, j, n = -1;
	BOOL	flag;

	if (!m_Member)
		return FALSE;

	// すべての要素について、
	for (i = 0; i < m_Member; i++)
	{
		// 視点の先に、交点がない場合。
		if (!m_Node[i]->GetInfo2(K, L, tmp, pHint))
			return FALSE;

		if (tmp.Distance <= l || !tmp.isEnter)
			continue;

		info.Cross = tmp.Cross;

		for (flag = TRUE, j = 0; j < m_Member; j++) {
			if (i == j)
				continue;

			if (!m_Node[j]->IsInside(m_Matrix * info.Cross))
				flag = FALSE;
		}

		if (flag)
			l = tmp.Distance, n = i;
	}

	if (n < 0)
		return FALSE;

	m_Node[n]->GetInfo2(K, L, info, pHint);

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

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