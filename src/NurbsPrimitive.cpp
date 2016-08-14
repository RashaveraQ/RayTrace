#include "stdafx.h"
#include <float.h>


bool NurbsPrimitive::newDeviceNode()
{
	bool newDevNurbsPrimitive(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevNurbsPrimitive(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

NurbsPrimitive::NurbsPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int cv_width, int cv_height, const sp Color)
	: Object(root, NodeType, Name, cv_width * cv_height, Color), m_ControlVertexWidth(cv_width), m_ControlVertexHeight(cv_height)
{
}

NurbsPrimitive::NurbsPrimitive(const NurbsPrimitive& other)
	: Object(other), m_ControlVertexWidth(other.m_ControlVertexWidth), m_ControlVertexHeight(other.m_ControlVertexHeight)
{
}

Vertex& NurbsPrimitive::getControlVertex(int w, int h) const
{
	return m_pVertexes[w + m_ControlVertexWidth * h];
}

bool NurbsPrimitive::IsInside(const sp& L) const
{
	return false; // 暫定
}

bool NurbsPrimitive::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (Object::GetInfo(K,L,info,pHint,fromOutSide))
		return true;

	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	// ▽▽ 暫定 ▽▽ 
	float	t = (K.y) ? -L.y / K.y : ((L.y> 0) ? FLT_MAX : -FLT_MAX);

	if (t <= 0)
		return false;

	info.Cross = K * t + L;
	info.Vertical = sp(0, 1, 0);
	info.Distance = t * sqrt(K * K);
	info.Material = GetPixel(info.Cross.x, info.Cross.z).getMaterial();
	info.pNode = this;

	return -3 < info.Cross.x && info.Cross.x < 3 && -3 < info.Cross.z && info.Cross.z < 3;
	// △△ 暫定 △△
}