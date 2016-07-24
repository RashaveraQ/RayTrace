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
