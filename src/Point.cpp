#include "StdAfx.h"
#include "Point.h"

Point::Point(const sp& isp) : sp(isp)
{
}

void Point::Draw_Outline(CDC* pDC, CRayTraceView& rtv, const matrix& m, const matrix& pivot) const
{
	static COLORREF s_color = RGB(255, 255, 0);

	CPen	x_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eX) ? s_color : RGB(255, 0, 0)),
		y_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eY) ? s_color : RGB(0, 255, 0)),
		z_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eZ) ? s_color : RGB(0, 0, 255)),
		c_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eNONE) ? s_color : RGB(0, 255, 255)),
		g_pen(PS_SOLID, 1, RGB(127, 127, 127));

	const CSize& size = rtv.m_ClientSize;
	const Viewport& viewport = rtv.m_Viewport;

	sp	p0 = m * pivot * sp(0, 0, 0);
	sp	px, py, pz;

	int r = 100;
	switch (rtv.m_Manipulator.Type) {
	case ePIVOT_MOVE:
		r = 50;
	case eMOVE:
	case eSCALE:
		px = p0 + viewport.m_Rotate * sp(0, 0, -2);
		py = p0 + viewport.m_Rotate * sp(0, -2, 0);
		pz = p0 + viewport.m_Rotate * sp(-2, 0, 0);
		break;
	default:
		px = m * pivot * sp(0, 0, -1);
		py = m * pivot * sp(0, -1, 0);
		pz = m * pivot * sp(-1, 0, 0);
	}

	POINT P0, PX, PY, PZ;
	p0.getPOINT(P0.x, P0.y, size.cx, size.cy);
	px.getPOINT(PX.x, PX.y, size.cx, size.cy);
	py.getPOINT(PY.x, PY.y, size.cx, size.cy);
	pz.getPOINT(PZ.x, PZ.y, size.cx, size.cy);

	// X-Axis
	CPen *old_pen;
	CPen *pens[] = { &x_pen, &z_pen, &y_pen };

	switch (rtv.m_Manipulator.Type)
	{
	case eMOVE:
	case ePIVOT_MOVE:
		old_pen =
			pDC->SelectObject(&z_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PZ);
		pDC->SelectObject(&y_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PY);
		pDC->SelectObject(&x_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PX);
		pDC->SelectObject(&c_pen);
		pDC->Rectangle(P0.x - 10, P0.y - 10, P0.x + 10, P0.y + 10);
		break;
	case eROTATE:
		old_pen =
			pDC->SelectObject(&g_pen);

		int i, j;
		float th, ph;
		POINT	P[50];

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 50; j++) {
				th = (i == 2) ? (6.28f * j / 50) : (3.14f * i / 2);
				ph = (i == 2) ? (3.14f / 2) : (6.28f * j / 50);
				sp p = m * sp(2 * cosf(th) * sinf(ph), 2 * cosf(ph), 2 * sinf(th) * sinf(ph));
				P[j].x = (long)(P0.x + (r - 10) * (p - p0).e().x);
				P[j].y = (long)(P0.y + (r - 10) * (p - p0).e().y);
			}
			pDC->SelectObject(pens[i]);
			pDC->Polygon(P, 50);
		}
		pDC->SelectObject(&c_pen);
		pDC->Ellipse(P0.x - r, P0.y - r, P0.x + r, P0.y + r);
		break;

	case eSCALE:
		old_pen =
			pDC->SelectObject(&g_pen);
		pDC->MoveTo(PX);
		pDC->LineTo(P0);
		pDC->LineTo(PY);
		pDC->MoveTo(P0);
		pDC->LineTo(PZ);
		pDC->SelectObject(&x_pen);
		pDC->Ellipse(PX.x - 5, PX.y - 5, PX.x + 5, PX.y + 5);
		pDC->SelectObject(&y_pen);
		pDC->Ellipse(PY.x - 5, PY.y - 5, PY.x + 5, PY.y + 5);
		pDC->SelectObject(&z_pen);
		pDC->Ellipse(PZ.x - 5, PZ.y - 5, PZ.x + 5, PZ.y + 5);
		pDC->SelectObject(&c_pen);
		pDC->Ellipse(P0.x - 5, P0.y - 5, P0.x + 5, P0.y + 5);
		break;
	default:
		return;
	}
	pDC->SelectObject(old_pen);
}
