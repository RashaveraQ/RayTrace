#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Cylinder.cuh"

__device__
DevCylinder::DevCylinder(DevNode** const root, const sp Color)
	: DevNode(root, eCYLINDER, Color)
{

}

__device__
bool DevCylinder::IsInside(const sp& L) const
{
	return (-1 <= L.y && L.y <= 1 && sqrt(L.x*L.x + L.z*L.z) <= 1.0);
}

__device__
bool DevCylinder::GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	if (L.y < -1) {
		if (K.y <= 0)
			return false;

		float t = -(1 + L.y) / K.y;
		if (t > 0) {
			sp	p = K * t + L;
			if (p.x * p.x + p.z * p.z <= 1) {
				info.Cross = p;
				info.Vertical = sp(0, -1, 0);
				info.Distance = t * sqrt(K * K);
				info.Material = GetPixel(.5f*(p.x + 1), .5f*(p.z + 1)).getMaterial();
				info.pNode = this;
				return true;
			}
		}
	}

	if (L.y > 1) {
		if (K.y >= 0)
			return false;

		float t = (1 - L.y) / K.y;
		if (t > 0) {
			sp	p = K * t + L;
			if (p.x * p.x + p.z * p.z <= 1) {
				info.Cross = p;
				info.Vertical = sp(0, 1, 0);
				info.Distance = t * sqrt(K * K);
				info.Material = GetPixel(.5f*(p.x + 1), .5f*(p.z + 1)).getMaterial();
				info.pNode = this;

				return true;
			}
		}
	}

	float	a, b, c, d, t, t1, t2;

	c = K.x * L.z - K.z * L.x;
	c *= c;
	a = K.x * K.x + K.z * K.z;
	d = a - c;

	if (d < 0)
		return FALSE;

	d = sqrt(d);
	b = -(K.x * L.x + K.z * L.z);

	t1 = (b + d) / a;
	t2 = (b - d) / a;

	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
		}
		else
			t = t1;
	}
	else {
		if (t2 > 0)
			t = t2;
		else
			return false;
	}

	sp p = K * t + L;

	if (p.y < -1 || 1 < p.y)
		return false;

	info.Cross = info.Vertical = p;
	info.Vertical.y = 0;
	info.Distance = t * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;

	return true;
}

__global__
void newCylinder(DevNode** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevCylinder(root, sp(Material));
}

bool newDevCylinder(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newCylinder<<<1, 1>>>(*out, root, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}