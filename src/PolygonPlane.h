#pragma once

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
};