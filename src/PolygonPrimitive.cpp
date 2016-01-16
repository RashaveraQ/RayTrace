#include "stdafx.h"
#include <float.h>

bool PolygonPrimitive::newDeviceNode()
{
	bool newDevPolygonPrimitive(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevPolygonPrimitive(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

PolygonPrimitive::PolygonPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int num_of_vertexes, Vertex** pVertexes, const sp Color)
	: Object(root, NodeType, Name, num_of_vertexes, Color)
{
}

PolygonPrimitive::PolygonPrimitive(const PolygonPrimitive& other)
	: Object(other)
{
}
