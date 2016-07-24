#ifndef __POLYGON_PLANE_H
#define __POLYGON_PLANE_H

class	PolygonPrimitive;
struct	matrix;

class PolygonPlane : public PolygonPrimitive
{
protected:
	DECLARE_SERIAL(PolygonPlane)
public:
	// コンストラクタ
	PolygonPlane(Node* const root = 0, const TCHAR* const Name = _T("PolygonPlane"), const sp Color = sp(255, 255, 255));
	PolygonPlane(const PolygonPlane& other);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
};

#endif
