#ifndef TARGET
#define TARGET
#endif	// TARGET

#include <math.h>
#include <d3dx9.h>

#include "sp4cuda.h"
#include "matrix4cuda.h"

TARGET
sp4cuda sp_sp(double ix, double iy, double iz)
{
	sp4cuda ans;
	ans.x = ix;
	ans.y = iy;
	ans.z = iz;
	return ans;
}

TARGET
sp4cuda sp_sp(const sp4cuda& in)
{
	return sp_sp(in.x, in.y, in.z);
}

TARGET
double sp_abs(const sp4cuda& in)
{
	return sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
}

TARGET
sp4cuda sp_sp(const matrix4cuda& m)
{
	return sp_sp(m.m_data[0][0], m.m_data[1][0], m.m_data[2][0]);
}

TARGET
sp4cuda sp_plus(const sp4cuda& lhs, const sp4cuda& rhs)
{
	return sp_sp(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); 
}

TARGET
double sp_multiple(const sp4cuda& lhs, const sp4cuda& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; 
}

TARGET
sp4cuda sp_minus(const sp4cuda& lhs, const sp4cuda& rhs)
{
	return sp_sp(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

TARGET
sp4cuda sp_minus(const sp4cuda& lhs)
{
	return sp_sp(-lhs.x, -lhs.y, -lhs.z);
}

TARGET
sp4cuda sp_multiple(const sp4cuda& lhs, double k)
{
	return sp_sp(lhs.x * k, lhs.y * k, lhs.z * k);
}

TARGET
sp4cuda sp_divide(const sp4cuda& lhs, double a)
{
	return sp_sp(lhs.x / a, lhs.y / a, lhs.z / a);
}

TARGET
sp4cuda sp_multiple(double k, const sp4cuda& rhs)
{
	return sp_sp(k * rhs.x, k * rhs.y, k * rhs.z);
}

TARGET
sp4cuda sp_e(const sp4cuda& lhs)
{
	double r = sqrt(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z);
	return sp_sp(lhs.x / r, lhs.y / r, lhs.z / r);
}

TARGET
D3DMATERIAL9 sp_getMaterial(const sp4cuda& s)
{
	D3DMATERIAL9	mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = (float)(s.x / 256);
	mtrl.Diffuse.g = mtrl.Ambient.g = (float)(s.y / 256);
	mtrl.Diffuse.b = mtrl.Ambient.b = (float)(s.z / 256);
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	return mtrl;
}

TARGET
sp4cuda sp_sp(const D3DMATERIAL9& mtrl)
{
	return sp_sp(256 * mtrl.Diffuse.r, 256 * mtrl.Diffuse.g, 256 * mtrl.Diffuse.b);
}