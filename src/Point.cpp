#include "StdAfx.h"
#include "Point.h"

Point::Point(const sp& isp)
{
	m_Move = move(isp.x, isp.y, isp.z);
	updateMatrix();
}

bool Point::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	float	a = K * K;
	float	b = K * L;
	float	c = L * L - 0.01f;

	float	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return FALSE;

	float	t;
	float	t1 = (-b + sqrt(bb_ac)) / a;
	float	t2 = (-b - sqrt(bb_ac)) / a;

	info.isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info.isEnter = 1;
		}
		else {
			t = t1;
		}
	}
	else {
		if (t2 > 0) {
			t = t2;
		}
		else
			return FALSE;
	}

	info.Cross = K * t + L;
	info.Vertical = info.isEnter ? info.Cross : -info.Cross;
	info.Distance = t * sqrt(K * K);
	info.pSelectable = this;

	return TRUE;
}

void Point::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	const CSize& size = raytraceview.m_ClientSize;
	//pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);
	POINT P;
	sp(mat * (sp)*this).getPOINT(P.x, P.y, size.cx, size.cy);
	P.x -= 3;
	P.y -= 3;
	CRect rect(P, CSize(6, 6));
	pDC->FillSolidRect(rect, m_Selected ? RGB(255,255, 0) : RGB(150, 0, 150));
	if (m_Selected)
		Selectable::Draw_Outline(pDC, raytraceview, mat * m_Matrix);
}

void Point::Rotate(const sp& o, eAxis axis, float d)
{
	if (!m_Selected)
		return;

	sp r = (m_Matrix * sp() - o);
	float l = r.abs();
	d /= 10;

	switch (axis)
	{
	case eNONE:
		break;
	case eX:
		Move(eY, r.y * (cosf(d) - 1.0f) - r.z * sinf(d));
		Move(eZ, r.z * (cosf(d) - 1.0f) + r.y * sinf(d));
		break;
	case eY:
		Move(eZ, r.z * (cosf(d) - 1.0f) - r.x * sinf(d));
		Move(eX, r.x * (cosf(d) - 1.0f) + r.z * sinf(d));
		break;
	case eZ:
		Move(eX, r.x * (cosf(d) - 1.0f) - r.y * sinf(d));
		Move(eY, r.y * (cosf(d) - 1.0f) + r.x * sinf(d));
		break;
	default:
		break;
	}
	//Selectable::Rotate(o, axis, d);
}

void Point::Scale(const sp& o, eAxis axis, float d)
{
	if (!m_Selected)
		return;

	sp o0 = d * (m_Matrix * sp() - o);
	Move(eX, o0.x);
	Move(eY, o0.y);
	Move(eZ, o0.z);
}

bool Point::getManipulatorMatrix(matrix& mat) const
{
	if (!m_Selected)
		return false;
	mat = mat * m_Move * m_Pivot;
	return true;
}
