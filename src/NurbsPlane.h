#ifndef __NURBS_PLANE_H
#define __NURBS_PLANE_H

class	NurbsPrimitive;
struct	matrix;

class NurbsPlane : public NurbsPrimitive
{
public:
	// コンストラクタ
	NurbsPlane(Node* const root = 0, const TCHAR* const Name = _T("NurbsPlane"), const sp Color = sp(255, 255, 255));
	NurbsPlane(const NurbsPlane& other);
};

#endif
