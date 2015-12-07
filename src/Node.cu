#include <math.h>
#include "Node.cuh"
#include "Info.cuh"

__device__
Sp DevNode::GetColor(const Sp& K, const Sp& L, int nest, const DevInfo* pHint, bool fromOutSide)
{
	DevInfo	info;

	// �ċA�����P�O���z���閔�́A��_�����݂��Ȃ��ꍇ�A
	if (nest > 10 || !GetInfo2(K, L, info, pHint, fromOutSide))
		return Sp(127, 127, 127);

	Sp k = K.e();
	Sp v = info.Vertical.e();

	Sp k2 = k - 2 * (v * k) * v;
	Sp l2 = info.Cross;


	// ���˗�������ꍇ�A
	if (info.pNode->m_Reflect > 0) {
		// ���˂��������x�N�g������F���擾�B
		Sp c = m_Root->GetColor(k2, l2, nest + 1, &info, true);
		// ���˗��ŐF��������B
		info.Material = (info.pNode->m_Reflect * c + (1 - info.pNode->m_Reflect) * Sp(info.Material)).getMaterial();
	}


	// ���ߗ�������ꍇ�A
	if (info.pNode->m_Through > 0) {
		double r = info.Refractive;
		double i = k * v;
		// �S���˂łȂ��ꍇ�A
		if (r > 1.0 || asin(r) > acos(-i)) {
			k2 = r * (k + v) - v;
			l2 = info.Cross;
			fromOutSide = !fromOutSide;
		}
		// ���܂��������x�N�g������F���擾�B
		Sp c = m_Root->GetColor(k2, l2, nest + 1, &info, fromOutSide);
		// ���ߗ��ŐF��������B
		info.Material = (info.pNode->m_Through * c + (1 - info.pNode->m_Through) * Sp(info.Material)).getMaterial();
	}

	Sp Light = Sp(1, 1, 1);
	// �������F��␳�B
	double	x = -Light.e() * info.Vertical.e();
	x = (x > 0.0) ? x : 0.0;
	double t = 64 + 191 * sin(M_PI / 2 * x);
	double b = 191 * (1 - cos(M_PI / 2 * x));

	return (t - b) * Sp(info.Material) / 255 + Sp(b, b, b);
}

// �����x�N�g��(Kt+L)�ƌ������镨�̂̏��info��Ԃ��B
// �߂�l:true ��������,false �����Ȃ�
__device__
bool DevNode::GetInfo2(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide)
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
	
	Matrix m = m_Matrix;
	Matrix Inv_m = m.Inv();

	Sp L2 = Inv_m * L;
	Sp K2 = Inv_m * (K + L) - L2;

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
