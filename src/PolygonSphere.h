#pragma once

class	PolygonPrimitive;
struct	matrix;

class PolygonSphere : public PolygonPrimitive
{
protected:
	DECLARE_SERIAL(PolygonSphere)
public:
	// コンストラクタ
	PolygonSphere(Node* const root = 0, const TCHAR* const Name = _T("PolygonSphere"), const sp Color = sp(255, 255, 255));
	PolygonSphere(const PolygonSphere& other);
};