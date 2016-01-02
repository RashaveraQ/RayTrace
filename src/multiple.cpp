#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Multiple, CObject, 1)

bool Multiple::newDeviceNode()
{
	bool newDevMultiple(DevNode*** out, DevNode** const root);
	return newDevMultiple(&m_devNode, m_Root ? m_Root->m_devNode : 0);
}

Multiple::Multiple(Node* const root, const TCHAR* const Name)
	: Gathering(root, eMULTIPLE, Name)
{
	if (!newDeviceNode())
		exit(1);
}

Multiple::Multiple(const Multiple& other)
	: Gathering(other)
{
	if (!newDeviceNode())
		exit(1);
}

Boundary Multiple::getBoundary()
{
	Boundary c1;
	for (int i = 0; i < m_Member; i++) {
		if (i == 0) {
			c1 = m_Node[0]->getBoundary2();
		}
		else {
			Boundary c2 = m_Node[i]->getBoundary2();
			if (c1.Center == c2.Center) {
				c1.Radius = min(c1.Radius, c2.Radius);
			}
			else {
				float l = (c1.Center - c2.Center).abs();
				if (l > c1.Radius + c2.Radius) {
					return Boundary();
				}
				float x = (l*l + c2.Radius * c2.Radius - c1.Radius * c1.Radius) / (2 * l);
				sp c = c2.Center + x / l * (c1.Center - c2.Center);
				float r = sqrt(c2.Radius * c2.Radius - x * x);
				c1.Center = c;
				c1.Radius = r;
			}
		}
	}
	return c1;
}

bool Multiple::IsInside(const sp& L) const
{
	if (!m_Member)
		return FALSE;

	for (int i = 0; i < m_Member; i++) {
		if (!m_Node[i]->IsInside2(L))
			return FALSE;
	}
	return TRUE;
}

bool Multiple::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (!m_Member)
		return FALSE;

	// すべての要素について、
	int	i;
	for (i = 0; i < m_Member; i++) {
		// 視点の先に、交点がない場合。
		if (!m_Node[i]->GetInfo2(K, L, info, pHint, fromOutSide))
			return FALSE;

		int j;
		for (j = 0; j < m_Member; j++) {
			if (i == j)
				continue;
			if (!m_Node[j]->IsInside2(info.Cross))
				break;
		}

		if (j == m_Member)
			break;
	}

	if (i == m_Member)
		return FALSE;

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

	return TRUE;
}

void Multiple::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hitem = c.InsertItem(m_Name, 5, 4, hParent, hInsertAfter);
	c.SetItemData(hitem, (DWORD_PTR)this);
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