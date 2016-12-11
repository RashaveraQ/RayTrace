#include "stdafx.h"
#include <float.h>

bool PolygonPrimitive::newDeviceNode()
{
	bool newDevPolygonPrimitive(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevPolygonPrimitive(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

PolygonPrimitive::PolygonPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int num_of_vertexes, const sp Color)
	: Object(root, NodeType, Name, num_of_vertexes, Color)
{
}

PolygonPrimitive::PolygonPrimitive(const PolygonPrimitive& other)
	: Object(other)
{
}

PolygonPrimitive::~PolygonPrimitive()
{
	for (int i = 0; i < m_NumberOfFaces; i++)
		delete m_pFaces[i];
	delete [] m_pFaces;
}

void PolygonPrimitive::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject(m_Selected ? WHITE_PEN : BLACK_PEN);
	for (int i = 0; i < m_NumberOfFaces; i++)
		m_pFaces[i]->Draw_Outline(pDC, raytraceview, m);
	Object::Draw_Outline(pDC, raytraceview, m);
}

bool PolygonPrimitive::IsInside(const sp& L) const
{
	// m_pFaces[0]へ視線を伸ばし、最初に衝突した m_pFaces の向きで内外判定が出来そう。
	float min = -1;
	int j = -1;
	for (int i = 0; i < m_NumberOfFaces; i++) {
		float l = m_pFaces[i]->GetMinDistanceFrom(L);
		if (min < 0 || l < min) {
			min = l;
			j = i;
		}
	}
	return m_pFaces[j]->IsFrontSide(L);
}

bool PolygonPrimitive::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (Object::GetInfo(K,L,info,pHint,fromOutSide))
		return true;

	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	Info	tmp;
	int		n;
	float	l = -1;

	for (int i = 0; i < m_NumberOfFaces; i++) {
		if (m_pFaces[i]->GetInfo(K, L, tmp, pHint, fromOutSide)) {
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

	info.pNode = this;

	return TRUE;
}
