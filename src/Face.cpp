#include "StdAfx.h"
#include "Face.h"

Face::Face(Vertex* pVertex1, Vertex* pVertex2, Vertex* pVertex3)
{
	m_pVertexes[0] = pVertex1;
	m_pVertexes[1] = pVertex2;
	m_pVertexes[2] = pVertex3;

	sp a = (sp)*pVertex2 - (sp)*pVertex1;
	sp b = (sp)*pVertex3 - (sp)*pVertex2;
	m_Normal = sp(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); 
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

bool Face::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	float t = (((sp)*m_pVertexes[0] - L) * m_Normal) / (K * m_Normal);
	sp p = K * t + L;

	sp v1 = (sp)*m_pVertexes[1] - (sp)*m_pVertexes[0];
	sp v2 = (sp)*m_pVertexes[2] - (sp)*m_pVertexes[1];
	sp v3 = (sp)*m_pVertexes[0] - (sp)*m_pVertexes[2];

	// 外積
	sp g1 = sp(p.y * v1.z - p.z * v1.y, p.z * v1.x - p.x * v1.z, p.x * v1.y - p.y * v1.x);
	sp g2 = sp(p.y * v2.z - p.z * v2.y, p.z * v2.x - p.x * v2.z, p.x * v2.y - p.y * v2.x);
	sp g3 = sp(p.y * v3.z - p.z * v3.y, p.z * v3.x - p.x * v3.z, p.x * v3.y - p.y * v3.x);

	float i1 = g1 * g2;
	float i2 = g2 * g3;
	float i3 = g3 * g1;

	float i = i2 * i3;
	// 交差しない場合、
	if (i < 0)
		return false;

	info.Cross = p;
	info.Vertical = info.Cross;
	info.Distance = t * sqrt(K * K);

	return true;
}


