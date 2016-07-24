#ifndef __POLYGON_PLANE_H
#define __POLYGON_PLANE_H

class	PolygonPrimitive;
struct	matrix;

class PolygonPlane : public PolygonPrimitive
{
protected:
	DECLARE_SERIAL(PolygonPlane)

	int m_ControlVertexWidth;
	int m_ControlVertexHeight;
	Vertex& getControlVertex(int w, int h) const;

public:
	// コンストラクタ
	PolygonPlane(Node* const root = 0, const TCHAR* const Name = _T("PolygonPlane"), const sp Color = sp(255, 255, 255));
	PolygonPlane(const PolygonPlane& other);
};

#endif
