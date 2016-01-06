#include "stdafx.h"
#include <float.h>

#define CV_WIDTH 4
#define CV_HEIGHT 4

IMPLEMENT_SERIAL(NurbsPlane, CObject, 1)

NurbsPlane::NurbsPlane(Node* const root, const TCHAR* const Name, const sp Color)
	: NurbsPrimitive(root, eNurbsPlane, Name, CV_WIDTH, CV_HEIGHT, Color)
{
	for (int i = 0; i < CV_WIDTH; i++)
		for (int j = 0; j < CV_HEIGHT; j++)
			m_ControlVertex[i][j] = Point(sp(-3 + 2 * i, 0, -3 + 2 * j));

	if (!newDeviceNode())
		exit(1);
}

NurbsPlane::NurbsPlane(const NurbsPlane& other)
	: NurbsPrimitive(other)
{
	if (!newDeviceNode())
		exit(1);
}

void NurbsPlane::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	const CSize& size = raytraceview.m_ClientSize;
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject(m_Selected ? WHITE_PEN : BLACK_PEN);

	POINT	P[CV_WIDTH][CV_HEIGHT];

	for (int i = 0; i < CV_WIDTH; i++)
		for (int j = 0; j < CV_HEIGHT; j++) {
			if (0 < i && i < CV_WIDTH - 1 && 0 < j && j < CV_HEIGHT - 1)
				continue;
			sp(m * (sp)m_ControlVertex[i][j]).getPOINT(P[i][j].x, P[i][j].y, size.cx, size.cy);
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
	NurbsPrimitive::Draw_Outline(pDC, raytraceview, m);
}
