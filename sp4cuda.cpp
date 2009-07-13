#include <math.h>
#include "sp4cuda.h"
#include "matrix4cuda.h"

extern "C" __device__ __host__
sp4cuda sp_sp(float ix, float iy, float iz)
{
	sp4cuda ans;
	ans.x = ix;
	ans.y = iy;
	ans.z = iz;
	return ans;
}

extern "C" __device__
sp4cuda sp_sp2(sp4cuda in)
{
	return sp_sp(in.x, in.y, in.z);
}

extern "C" __device__
float sp_abs(sp4cuda in)
{
	return sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
}

extern "C" __device__
sp4cuda sp_sp3(matrix4cuda m)
{
	return sp_sp(m.m_data[0][0], m.m_data[1][0], m.m_data[2][0]);
}

extern "C" __device__
sp4cuda sp_plus(sp4cuda lhs, sp4cuda rhs)
{
	return sp_sp(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); 
}

extern "C" __device__
float sp_internal_multiple(sp4cuda lhs, sp4cuda rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; 
}

extern "C" __device__
sp4cuda sp_minus(sp4cuda lhs, sp4cuda rhs)
{
	return sp_sp(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

extern "C" __device__
sp4cuda sp_minus_signed(sp4cuda lhs)
{
	return sp_sp(-lhs.x, -lhs.y, -lhs.z);
}

extern "C" __device__
sp4cuda sp_multiple(sp4cuda lhs, float k)
{
	return sp_sp(lhs.x * k, lhs.y * k, lhs.z * k);
}

extern "C" __device__
sp4cuda sp_divide(sp4cuda lhs, float a)
{
	return sp_sp(lhs.x / a, lhs.y / a, lhs.z / a);
}

extern "C" __device__
sp4cuda sp_multiple2(float k, sp4cuda rhs)
{
	return sp_sp(k * rhs.x, k * rhs.y, k * rhs.z);
}

extern "C" __device__
sp4cuda sp_e(sp4cuda lhs)
{
	float r = sqrtf(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z);
	return sp_sp(lhs.x / r, lhs.y / r, lhs.z / r);
}

/*
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
*/