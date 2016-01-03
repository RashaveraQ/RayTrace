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
	: Node(0, eNURBS, _T("Nurbs")), m_IsControlVertexEditable(false)
{
}

NurbsPrimitive::NurbsPrimitive(Node* const root, const TCHAR* const Name, const sp Color)
	: Node(root, eNURBS, Name, Color), m_IsControlVertexEditable(false)
{
	if (!newDeviceNode())
		exit(1);
}

NurbsPrimitive::NurbsPrimitive(const NurbsPrimitive& other)
	: Node(other), m_IsControlVertexEditable(false)
{
	if (!newDeviceNode())
		exit(1);
}

NurbsPrimitive::~NurbsPrimitive()
{
	for (int i = 0; i < m_ControlVertexWidth; i++)
		delete[] m_ControlVertex[i];
	delete m_ControlVertex;
}

bool NurbsPrimitive::IsInside(const sp& L) const
{
	return ( L.y >= 0.0 );
}

bool NurbsPrimitive::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	for (int i = 0; i < m_ControlVertexWidth; i++)
		for (int j = 0; j < m_ControlVertexHeight; j++)
			if (m_ControlVertex[i][j].GetInfo(K, L, info, NULL, true))
				return true;

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
	//if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Draw_Outline(pDC, raytraceview, mat);
	//}
	Node::Draw_Outline(pDC, raytraceview, mat);
}

void NurbsPrimitive::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}
