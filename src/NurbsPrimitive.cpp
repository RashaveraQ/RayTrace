#include "stdafx.h"
#include <float.h>

IMPLEMENT_SERIAL(NurbsPrimitive, CObject, 1)

Boundary NurbsPrimitive::sBoundary = Boundary(FLT_MAX); // 

bool NurbsPrimitive::newDeviceNode()
{
	bool newDevNurbsPrimitive(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevNurbsPrimitive(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

NurbsPrimitive::NurbsPrimitive()
	: Node(0, NURBS_PRIMITIVE, _T("NurbsPrimitive"))
{
}

NurbsPrimitive::NurbsPrimitive(Node* const root, const TCHAR* const Name, const sp Color)
	: Node(root, NURBS_PRIMITIVE, Name, Color)
{
	if (!newDeviceNode())
		exit(1);
}

NurbsPrimitive::NurbsPrimitive(const NurbsPrimitive& other)
	: Node(other)
{
	if (!newDeviceNode())
		exit(1);
}

bool NurbsPrimitive::IsInside(const sp& L) const
{
	return ( L.y >= 0.0 );
}

bool NurbsPrimitive::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
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

	return -3 < info.Cross.x && info.Cross.x < 3 && -3 < info.Cross.z && info.Cross.z < 3;
}

void NurbsPrimitive::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	POINT	P[4][4];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (0 < i && i < 3 && 0 < j && j < 3)
				continue;
			sp(m * m_ControlVertex[i][j]).getPOINT(P[i][j].x, P[i][j].y, size.cx, size.cy);
		}

	for (int i = 0; i < 4; i += 3) {
		pDC->MoveTo(P[i][0]);
		for (int j = 1; j < 4; j++)
			pDC->LineTo(P[i][j]);
	}
	for (int i = 0; i < 4; i += 3) {
		pDC->MoveTo(P[0][i]);
		for (int j = 1; j < 4; j++)
			pDC->LineTo(P[j][i]);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void NurbsPrimitive::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}
