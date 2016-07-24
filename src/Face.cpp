#include "StdAfx.h"
#include "Face.h"

Face::Face(Vertex* pVertex1, Vertex* pVertex2, Vertex* pVertex3)
{
	m_pVertexes[0] = pVertex1;
	m_pVertexes[1] = pVertex2;
	m_pVertexes[2] = pVertex3;
}

Face::~Face()
{
}

void Face::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& m) const
{
	const CSize& size = raytraceview.m_ClientSize;
//	matrix m = mat * m_Matrix;
//	pDC->SelectStockObject(m_Selected ? WHITE_PEN : BLACK_PEN);

	POINT P[3];

	for (int i = 0; i < 3; i++)
		sp(m * (sp)(*m_pVertexes[i])).getPOINT(P[i].x, P[i].y, size.cx, size.cy);

	pDC->MoveTo(P[0]);
	pDC->LineTo(P[1]);
	pDC->LineTo(P[2]);
	pDC->LineTo(P[0]);
}
