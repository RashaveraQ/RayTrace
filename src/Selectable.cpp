#include "StdAfx.h"
#include "Selectable.h"


Selectable::Selectable()
{
}

Selectable::Selectable(const Selectable& other)
	: m_Scale(other.m_Scale), m_Rotate(other.m_Rotate), m_Move(other.m_Move), m_Pivot(other.m_Pivot)
{
	updateMatrix();
}

Selectable::~Selectable()
{
}

bool Selectable::GetInfo3(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide)
{
	matrix& m = m_Matrix;
	matrix& Inv_m = m.Inv();
	sp L2 = Inv_m * L;
	sp K2 = Inv_m * (K + L) - L2;
	return GetInfo(K2, L2, info, pHint, fromOutSide);
}

Selectable* Selectable::GetSelectable(const sp& K, const sp& L)
{
	Info	info;
	if (GetInfo3(K, L, info, NULL, true)) {
		return (Selectable*)(info.pNode ? info.pNode : info.pSelectable);
	}
	return NULL;
}

void Selectable::updateMatrix()
{
	m_Matrix = m_Move * m_Rotate * m_Scale;
}

void Selectable::Move(eAxis axis, float d)
{
	switch (axis) {
	case eX:	m_Move.set_data(3, 4, m_Move.get_data(3, 4) - (float)d / 20);	break;
	case eY:	m_Move.set_data(2, 4, m_Move.get_data(2, 4) - (float)d / 20);	break;
	case eZ:	m_Move.set_data(1, 4, m_Move.get_data(1, 4) - (float)d / 20);	break;
	}
	updateMatrix();
}

void Selectable::Move(POINT d)
{
	m_Move.set_data(1, 4, m_Move.get_data(1, 4) - (float)d.x / 20);
	m_Move.set_data(2, 4, m_Move.get_data(2, 4) - (float)d.y / 20);
	updateMatrix();
}

void Selectable::Rotate(eAxis axis, float d)
{
	switch (axis) {
	case eX:
		m_Rotate = m_Rotate * m_Pivot * rotate(0, 0, 1, d) * m_Pivot.Inv();
		break;
	case eY:
		m_Rotate = m_Rotate * m_Pivot * rotate(0, 1, 0, d) * m_Pivot.Inv();
		break;
	case eZ:
		m_Rotate = m_Rotate * m_Pivot * rotate(1, 0, 0, d) * m_Pivot.Inv();
		break;
	}
	updateMatrix();
}

void Selectable::Rotate(POINT d)
{
	rotate	r((float)-d.y, (float)-d.x, 0, sqrt((float)(d.x * d.x + d.y * d.y)) / 2);
	m_Rotate = r * m_Rotate;
	updateMatrix();
}


void Selectable::Scale(eAxis axis, float d)
{
	if (axis == eX || axis == eNONE)
		m_Scale.set_data(3, 3, m_Scale.get_data(3, 3) - d / 50);
	if (axis == eY || axis == eNONE)
		m_Scale.set_data(2, 2, m_Scale.get_data(2, 2) - d / 50);
	if (axis == eZ || axis == eNONE)
		m_Scale.set_data(1, 1, m_Scale.get_data(1, 1) - d / 50);
	updateMatrix();
}

void Selectable::MovePivot(eAxis axis, float d)
{
	switch (axis) {
	case eX:	m_Pivot.set_data(3, 4, m_Pivot.get_data(3, 4) - (float)d / 20);	break;
	case eY:	m_Pivot.set_data(2, 4, m_Pivot.get_data(2, 4) - (float)d / 20);	break;
	case eZ:	m_Pivot.set_data(1, 4, m_Pivot.get_data(1, 4) - (float)d / 20);	break;
	}
}

void Selectable::Draw_Outline(CDC* pDC, CRayTraceView& rtv, const matrix& m) const
{
	static COLORREF s_color = RGB(255, 255, 0);

	CPen	x_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eX) ? s_color : RGB(255, 0, 0)),
		y_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eY) ? s_color : RGB(0, 255, 0)),
		z_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eZ) ? s_color : RGB(0, 0, 255)),
		c_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eNONE) ? s_color : RGB(0, 255, 255)),
		g_pen(PS_SOLID, 1, RGB(127, 127, 127));

	const CSize& size = rtv.m_ClientSize;
	const Viewport& viewport = rtv.m_Viewport;

	sp	p0 = m * m_Pivot * sp(0, 0, 0);
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
		px = m * m_Pivot * sp(0, 0, -1);
		py = m * m_Pivot * sp(0, -1, 0);
		pz = m * m_Pivot * sp(-1, 0, 0);
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

bool Selectable::SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& mat) const
{
	if (rtv.m_SelectedNode != this || rtv.m_Manipulator.Type == eSELECT)
		return false;

	const CSize& size = rtv.m_ClientSize;
	const Viewport& viewport = rtv.m_Viewport;

	matrix m = mat * m_Matrix;

	sp	p0 = m * m_Pivot * sp(0, 0, 0);
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
		px = m * m_Pivot * sp(0, 0, -1 / m_Scale.get_data(3, 3));
		py = m * m_Pivot * sp(0, -1 / m_Scale.get_data(2, 2), 0);
		pz = m * m_Pivot * sp(-1 / m_Scale.get_data(1, 1), 0, 0);
	}

	POINT P0, PX, PY, PZ;
	p0.getPOINT(P0.x, P0.y, size.cx, size.cy);
	px.getPOINT(PX.x, PX.y, size.cx, size.cy);
	py.getPOINT(PY.x, PY.y, size.cx, size.cy);
	pz.getPOINT(PZ.x, PZ.y, size.cx, size.cy);

	switch (rtv.m_Manipulator.Type)
	{
		// case eMOVE:
		// point が P0-PX 線上の場合は、rtv.m_Manipulator.Axis = eX;
		// point が P0-PY 線上の場合は、rtv.m_Manipulator.Axis = eY;
		// point が P0-PZ 線上の場合は、rtv.m_Manipulator.Axis = eZ;
		// break;
	case eROTATE:
	{
		eAxis tbl[] = { eX, eZ, eY };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 50; j++) {
				float th = (i == 2) ? (6.28f * j / 50) : (3.14f * i / 2);
				float ph = (i == 2) ? (3.14f / 2) : (6.28f * j / 50);
				sp p = m * sp(2 * cos(th)*sin(ph), 2 * cos(ph), 2 * sin(th)*sin(ph));
				float dx = (P0.x + (r - 10) * (p - p0).e().x) - point.x;
				float dy = (P0.y + (r - 10) * (p - p0).e().y) - point.y;
				if (dx*dx + dy*dy < 20) {
					rtv.m_Manipulator.Axis = tbl[i];
					return true;
				}
			}
		}
	}

	return false;
	break;
	case eMOVE: // temp
	case ePIVOT_MOVE: // temp
	case eSCALE:
		if (::abs(PX.x - point.x) + ::abs(PX.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eX;
		else if (::abs(PY.x - point.x) + ::abs(PY.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eY;
		else if (::abs(PZ.x - point.x) + ::abs(PZ.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eZ;
		else if (::abs(P0.x - point.x) + ::abs(P0.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eNONE;
		else
			return false;
		break;
	default:
		return false;
	}
	return true;
}

void Selectable::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		for (int i = 1; i < 4; i++) {
			ar << m_Scale.get_data(i, i);
			ar << m_Move.get_data(i, 4);
			for (int j = 1; j < 4; j++)
				ar << m_Rotate.get_data(i, j);
		}
	} else {
		for (int i = 1; i < 4; i++) {
			float value;
			ar >> value;
			m_Scale.set_data(i, i, value);
			ar >> value;
			m_Move.set_data(i, 4, value);
			for (int j = 1; j < 4; j++) {
				ar >> value;
				m_Rotate.set_data(i, j, value);
			}
		}
		m_Matrix = m_Move * m_Rotate * m_Scale;
	}
}
