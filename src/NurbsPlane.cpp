#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(NurbsPlane, CObject, 1)

Boundary NurbsPlane::sBoundary = Boundary(FLT_MAX); // 

bool NurbsPlane::newDeviceNode()
{
	bool newDevNurbsPlane(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevNurbsPlane(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

NurbsPlane::NurbsPlane(Node* const root, const TCHAR* const Name, const sp Color)
	: Node(root, NURBS_PLANE, Name, Color)
{
	if (!newDeviceNode())
		exit(1);
}

NurbsPlane::NurbsPlane(const NurbsPlane& other)
	: Node(other)
{
	if (!newDeviceNode())
		exit(1);
}

bool NurbsPlane::IsInside(const sp& L) const
{
	return ( L.y >= 0.0 );
}

bool NurbsPlane::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float	t = (K.y) ? -L.y / K.y : ((L.y> 0) ? FLT_MAX : -FLT_MAX);

	if (t <= 0)
		return false;

	info.Cross = K * t + L;
	info.Vertical = sp(0, 0, -1);
	info.Distance = t * sqrt(K * K);
	info.isEnter = (L.y < 0);
	info.Material = GetPixel(info.Cross.x, info.Cross.z).getMaterial();
	info.pNode = this;

	return -1 < info.Cross.x && info.Cross.x < 1 && -1 < info.Cross.z && info.Cross.z < 1;
}

void NurbsPlane::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}
