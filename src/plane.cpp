#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(Plane, CObject, 1)

Boundary Plane::sBoundary = Boundary(FLT_MAX); // 

bool Plane::newDeviceNode()
{
	bool newDevPlane(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevPlane(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

Plane::Plane(Node* const root, const TCHAR* const Name, const sp Color)
	: Node(root, PLANE, Name, Color)
{
	if (!newDeviceNode())
		exit(1);
}

Plane::Plane(const Plane& other) 
	: Node(other)
{
	if (!newDeviceNode())
		exit(1);
}

bool Plane::IsInside(const sp& L) const
{
	return ( L.y >= 0.0 );
}

bool Plane::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
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

void Plane::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}
