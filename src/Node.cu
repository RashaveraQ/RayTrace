#include <math.h>
#include "Node.cuh"
#include "Info.cuh"
#include "DoCuda.h"

__device__
DevNode::DevNode(DevNode** const root, node_type NodeType, const sp Color)
	: m_Root(root), m_pParent(0), m_NodeType(NodeType), m_Material(Color.getMaterial()), m_Reflect(0), m_Through(0), m_Refractive(1)
{
}

__device__
DevNode::DevNode(const DevNode& other)
	: m_Root(other.m_Root), m_Scale(other.m_Scale), m_Rotate(other.m_Rotate), m_Move(other.m_Move), m_Matrix(other.m_Matrix),
	m_NodeType(other.m_NodeType), m_Material(other.m_Material), m_Reflect(other.m_Reflect), m_Through(other.m_Through), m_Refractive(other.m_Refractive)
{
}

__device__
sp DevNode::GetColor(const sp& K, const sp& L, int nest, const DevInfo* pHint, bool fromOutSide)
{
	DevInfo	info;

	// 再帰数が１０を越える又は、交点が存在しない場合、
	if (nest > 10 || !GetInfo2(K, L, info, pHint, fromOutSide))
		return sp(127, 127, 127);

	sp k = K.e();
	sp v = info.Vertical.e();

	sp k2 = k - 2 * (v * k) * v;
	sp l2 = info.Cross;


	// 反射率がある場合、
	if (info.pNode->m_Reflect > 0) {
		// 反射した視線ベクトルから色を取得。
		sp c = (*m_Root)->GetColor(k2, l2, nest + 1, &info, true);
		// 反射率で色を混ぜる。
		info.Material = (info.pNode->m_Reflect * c + (1 - info.pNode->m_Reflect) * sp(info.Material)).getMaterial();
	}


	// 透過率がある場合、
	if (info.pNode->m_Through > 0) {
		float r = info.Refractive;
		float i = k * v;
		// 全反射でない場合、
		if (r > 1.0 || asin(r) > acos(-i)) {
			k2 = r * (k + v) - v;
			l2 = info.Cross;
			fromOutSide = !fromOutSide;
		}
		// 屈折した視線ベクトルから色を取得。
		sp c = (*m_Root)->GetColor(k2, l2, nest + 1, &info, fromOutSide);
		// 透過率で色を混ぜる。
		info.Material = (info.pNode->m_Through * c + (1 - info.pNode->m_Through) * sp(info.Material)).getMaterial();
	}

	sp Light = sp(1, 1, 1);
	// 光源より色を補正。
	float	x = -Light.e() * info.Vertical.e();
	x = (x > 0.0) ? x : 0.0;
	float t = 64 + 191 * sin(M_PI / 2 * x);
	float b = 191 * (1 - cos(M_PI / 2 * x));

	return (t - b) * sp(info.Material) / 255 + sp(b, b, b);
}

// 視線ベクトル(Kt+L)と交差する物体の情報infoを返す。
// 戻り値:true 交差あり,false 交差なし
__device__
bool DevNode::GetInfo2(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide)
{
	// START Boundary 
	/*
	float a = gK * gK;
	float b = (gL - m_Boundary.Center) * gK;
	float c = (m_Boundary.Center - gL) * (m_Boundary.Center - gL) - m_Boundary.Radius * m_Boundary.Radius;
	float bb_ac = b*b-a*c;

	if (bb_ac < 0)
	return FALSE;

	float t1, t2;

	t1 = (-b+sqrt(bb_ac))/a;
	t2 = (-b-sqrt(bb_ac))/a;

	if (t1 <= 0 || t2 <= 0)
	return FALSE;
	// End Boundary
	*/

	matrix& m = m_Matrix;
	const matrix& Inv_m = m.Inv();

	sp L2 = Inv_m * L;
	sp K2 = Inv_m * (K + L) - L2;

	if (!GetInfo(K2, L2, info, pHint, fromOutSide)) {
		return false;
	}

	info.Vertical = m_Scale.Inv() * info.Vertical;
	info.Vertical = m * (info.Vertical + info.Cross) - m * info.Cross;
	info.Cross = m * info.Cross;
	info.Distance = (info.Cross - L).abs();
	info.Refractive = info.pNode->m_Refractive / ((pHint) ? pHint->Refractive : 1.0);
	if (!info.isEnter)
		info.Refractive = 1 / info.Refractive;

	return true;
}

__device__
sp DevNode::GetPixel(float x, float y) const
{
//	COLORREF	c;

//	if (m_TextureFileName.IsEmpty())
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	//c = m_TextureDC.GetPixel((int)(x * m_TextureSize.cx), (int)(y * m_TextureSize.cy));

	//if (c == -1)
	//	return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	//return sp(GetRValue(c), GetGValue(c), GetBValue(c));
}

__device__
bool DevNode::IsInside2(const sp& L) {
	return IsInside(m_Matrix.Inv() * L);
}