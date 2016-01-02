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
	: Node(root, ePLANE, Name, Color)
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
	info.Vertical = sp(0, 1, 0);
	info.Distance = t * sqrt(K * K);
	info.isEnter = (L.y < 0);
	info.Material = GetPixel(info.Cross.x, info.Cross.z).getMaterial();
	info.pNode = this;

	return -1 < info.Cross.x && info.Cross.x < 1 && -1 < info.Cross.z && info.Cross.z < 1;
}

void Plane::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	sp p[] = { sp(1, 0, 1), sp(-1, 0, 1), sp(-1, 0, -1), sp(1, 0, -1) };

	POINT	P[4];

	for (int i = 0; i < 4; i++) {
		sp(m * p[i]).getPOINT(P[i].x, P[i].y, size.cx, size.cy);
	}

	pDC->MoveTo(P[0]);
	pDC->LineTo(P[1]);
	pDC->LineTo(P[2]);
	pDC->LineTo(P[3]);
	pDC->LineTo(P[0]);
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Plane::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}
