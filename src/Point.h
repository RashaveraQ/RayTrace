#pragma once
#include "node.h"

class Point : public sp
{
public:
	Point(const sp& = sp());
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& m, const matrix& pivot) const;
};

