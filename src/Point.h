#pragma once
#include "node.h"
#include "Selectable.h"

class Point : public Selectable
{
public:
	Point(const sp& = sp());
	bool GetInfo(const struct sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	operator sp() const { return m_Matrix * sp(); }
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
	void Rotate(const sp& o, eAxis axis, float d);
	void Scale(const sp& o, eAxis axis, float d);
};
