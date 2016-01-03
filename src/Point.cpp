#include "StdAfx.h"
#include "Point.h"

Point::Point(const sp& isp)
{
	m_Scale = expand(0.1f);
	m_Move = move(isp.x, isp.y, isp.z);
	updateMatrix();
}

bool Point::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	float	a = K * K;
	float	b = K * L;
	float	c = L * L - 1.0f;

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
	const Selectable* pNode = raytraceview.m_SelectedNode;
	//pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);
	POINT P;
	sp(mat * (sp)*this).getPOINT(P.x, P.y, size.cx, size.cy);
	P.x -= 3;
	P.y -= 3;
	CRect rect(P, CSize(6, 6));
	pDC->FillSolidRect(rect, (pNode == this) ? RGB(255,255,255) : RGB(150, 0, 150));
	if (pNode == this)
		Selectable::Draw_Outline(pDC, raytraceview, mat * m_Matrix);
}
