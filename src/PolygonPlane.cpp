#include "stdafx.h"
#include <float.h>

#define CV_WIDTH 4
#define CV_HEIGHT 4

IMPLEMENT_SERIAL(PolygonPlane, CObject, 1)

PolygonPlane::PolygonPlane(Node* const root, const TCHAR* const Name, const sp Color)
	: PolygonPrimitive(root, ePolygonPlane, Name, CV_WIDTH, CV_HEIGHT, Color)
{
	for (int i = 0; i < CV_WIDTH; i++)
		for (int j = 0; j < CV_HEIGHT; j++)
			getControlVertex(i,j) = Vertex(sp(-3 + 2 * i, 0, -3 + 2 * j));

	m_NumberOfFaces = 2 * (CV_WIDTH - 1) * (CV_HEIGHT - 1);
	m_pFaces = new Face*[m_NumberOfFaces];
	int n = 0;
	for (int i = 0; i < CV_WIDTH - 1; i++)
		for (int j = 0; j < CV_HEIGHT - 1; j++) {
			m_pFaces[n++] = new Face(&getControlVertex(i,j), &getControlVertex(i+1,j), &getControlVertex(i,j+1));
			m_pFaces[n++] = new Face(&getControlVertex(i+1,j+1), &getControlVertex(i,j+1), &getControlVertex(i+1,j));
		}


	if (!newDeviceNode())
		exit(1);
}

PolygonPlane::PolygonPlane(const PolygonPlane& other)
	: PolygonPrimitive(other)
{
	if (!newDeviceNode())
		exit(1);
}

void PolygonPlane::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject(m_Selected ? WHITE_PEN : BLACK_PEN);
	for (int i = 0; i < m_NumberOfFaces; i++)
		m_pFaces[i]->Draw_Outline(pDC, raytraceview, m);
	/*
	const CSize& size = raytraceview.m_ClientSize;
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject(m_Selected ? WHITE_PEN : BLACK_PEN);

	POINT	P[CV_WIDTH][CV_HEIGHT];

	for (int i = 0; i < CV_WIDTH; i++)
		for (int j = 0; j < CV_HEIGHT; j++) {
			if (0 < i && i < CV_WIDTH - 1 && 0 < j && j < CV_HEIGHT - 1)
				continue;
			sp(m * (sp)getControlVertex(i,j)).getPOINT(P[i][j].x, P[i][j].y, size.cx, size.cy);
		}

	for (int i = 0; i < CV_WIDTH; i += CV_WIDTH - 1) {
		pDC->MoveTo(P[i][0]);
		for (int j = 1; j < CV_HEIGHT; j++)
			pDC->LineTo(P[i][j]);
	}
	for (int i = 0; i < CV_HEIGHT; i += CV_HEIGHT - 1) {
		pDC->MoveTo(P[0][i]);
		for (int j = 1; j < CV_WIDTH; j++)
			pDC->LineTo(P[j][i]);
	}
	*/
	PolygonPrimitive::Draw_Outline(pDC, raytraceview, m);
}
