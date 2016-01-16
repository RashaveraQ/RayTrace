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
