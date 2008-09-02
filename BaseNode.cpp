TARGET sp BaseNode::GetPixel(double x, double y)
{
	COLORREF	c = -1;
	if (!m_TextureFileName)
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);
/*
	c = m_TextureDC.GetPixel((int)(x * m_TextureSize.cx), (int)(y * m_TextureSize.cy));
*/
	if (c == -1)
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	return sp(GetRValue(c),GetGValue(c),GetBValue(c));
}

#include "GetInfo.cpp"

TARGET BOOL BaseNode::IsInside(const sp* L)
{
	BOOL ans;
	switch (m_NodeType) {
	case SPHERE:
		ans = ((BaseSphere*)this)->IsInside(L);
		break;
	case PLANE:
		ans = ((BasePlane*)this)->IsInside(L);
		break;
	case PLUS:
		ans = ((BasePlus*)this)->IsInside(L);
		break;
	case MINUS:
		ans = ((BaseMinus*)this)->IsInside(L);
		break;
	case MULTIPLE:
		ans = ((BaseMultiple*)this)->IsInside(L);
		break;
	case CONE:
		ans = ((BaseCone*)this)->IsInside(L);
		break;
	case CYLINDER:
		ans = ((BaseCylinder*)this)->IsInside(L);
		break;
	case TORUS:
		ans = ((BaseTorus*)this)->IsInside(L);
		break;
	case POLYGON:
		ans = ((BasePolygon2*)this)->IsInside(L);
		break;
	case CUBE:
		ans = ((BaseCube*)this)->IsInside(L);
		break;
	case TEAPOT:
		ans = ((BaseTeapot*)this)->IsInside(L);
		break;
	}
	return ans;
}

TARGET BOOL BaseNode::GetInfo(const sp* K, const sp* L, Info* info)
{
	BOOL ans;
	switch (m_NodeType) {
	case SPHERE:
		ans = ((BaseSphere*)this)->GetInfo(K,L,info);
		break;
	case PLANE:
		ans = ((BasePlane*)this)->GetInfo(K,L,info);
		break;
	case PLUS:
		ans = ((BasePlus*)this)->GetInfo(K,L,info);
		break;
	case MINUS:
		ans = ((BaseMinus*)this)->GetInfo(K,L,info);
		break;
	case MULTIPLE:
		ans = ((BaseMultiple*)this)->GetInfo(K,L,info);
		break;
	case CONE:
		ans = ((BaseCone*)this)->GetInfo(K,L,info);
		break;
	case CYLINDER:
		ans = ((BaseCylinder*)this)->GetInfo(K,L,info);
		break;
	case TORUS:
		ans = ((BaseTorus*)this)->GetInfo(K,L,info);
		break;
	case POLYGON:
		ans = ((BasePolygon2*)this)->GetInfo(K,L,info);
		break;
	case CUBE:
		ans = ((BaseCube*)this)->GetInfo(K,L,info);
		break;
	case TEAPOT:
		ans = ((BaseTeapot*)this)->GetInfo(K,L,info);
		break;
	}
	return ans;
}

// 視線ベクトル(Kt+L)と交差する物体の情報infoを返す。
// 戻り値:true 交差あり,false 交差なし
TARGET BOOL BaseNode::GetInfo2(const sp* K, const sp* L, Info* info)
{
	// START Boundary 
/*
	double a = gK * gK;
	double b = (gL - m_Boundary.Center) * gK;
	double c = (m_Boundary.Center - gL) * (m_Boundary.Center - gL) - m_Boundary.Radius * m_Boundary.Radius;
	double bb_ac = b*b-a*c;

	if (bb_ac < 0)
		return FALSE;

	double t1, t2;

	t1 = (-b+sqrt(bb_ac))/a;
	t2 = (-b-sqrt(bb_ac))/a;

	if (t1 <= 0 || t2 <= 0)
		return FALSE;
	// End Boundary
*/
	matrix m = m_Move * m_Rotate * m_Scale;
	matrix Inv_m = m.Inv();

	sp L2 = Inv_m * (*L);
	sp K2 = Inv_m * ((*K) + (*L)) - L2;

	if (!GetInfo(&K2, &L2, info)) {
		return FALSE;
	}

	info->Vertical = m * (info->Vertical + info->Cross) - m * info->Cross;
	info->Cross = m * info->Cross;
	info->Distance = (info->Cross - (*L)).abs();
	info->Refractive = info->pNode->m_Refractive;
	if (info->isEnter)
		info->Refractive = 1 / info->Refractive;

	return TRUE;
}



