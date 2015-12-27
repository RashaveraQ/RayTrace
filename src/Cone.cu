#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Cone.cuh"

__device__
DevCone::DevCone(DevNode** const root, const sp Color)
	: DevNode(root, CONE, Color)
{

}

__device__
bool DevCone::IsInside(const sp& L) const
{
	return (0 <= L.y && L.y <= 1 && sqrt(L.x * L.x + L.z * L.z) <= L.y);
}

__device__
bool DevCone::GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return FALSE;

	if (L.y > 1 && K.y >= 0)
		return FALSE;

	float t[2];
	sp     v[2];
	int i = 0;

	t[0] = (1 - L.y) / K.y;
	if (t[0] > 0) {
		sp p = K * t[0] + L;
		if (p.x * p.x + p.z * p.z <= 1) {
			v[i] = sp(0, 1, 0);
			info.Material = GetPixel(.5f * (p.x + 1), .5f * (p.z + 1)).getMaterial();
			info.pNode = this;
			info.Refractive = m_Refractive;
			i++;
		}
	}

	float	a, b, c, d, t1, t2;

	c = K.x * L.y - K.y * L.x, c *= c, d = c;
	c = K.z * L.y - K.y * L.z, c *= c, d += c;
	c = K.x * L.z - K.z * L.x, c *= c, d -= c;

	if (d >= 0) {
		d = sqrt(d);
		a = -(K.x * L.x + K.z * L.z - K.y * L.y);
		b = K.x * K.x + K.z * K.z - K.y * K.y;

		t1 = (a + d) / b;
		sp p = K * t1 + L;
		if (p.y < 0 || p.y > 1 || p.x * p.x + p.z * p.z > 1)
			t1 = -1;

		t2 = (a - d) / b;
		p = K * t2 + L;
		if (p.y < 0 || p.y > 1 || p.x * p.x + p.z * p.z > 1)
			t2 = -1;

		if (t1 > 0) {
			if (t2 > 0) {
				t[i] = (t1 < t2) ? t1 : t2;
				v[i] = K * t[i] + L;
				v[i].y *= -1;
				i++;
				t[i] = (t1 < t2) ? t2 : t1;
				v[i] = K * t[i] + L;
				v[i].y *= -1;
				i++;
			}
			else {
				t[i] = t1;
				v[i] = K * t[i] + L;
				v[i].y *= -1;
				i++;
			}
		}
		else {
			if (t2 > 0) {
				t[i] = t2;
				v[i] = K * t[i] + L;
				v[i].y *= -1;
				i++;
			}
		}
	}

	switch (i) {
	case 0:
		return FALSE;
	case 1:
		break;
	default:
		if (pHint && pHint->pNode == this) {
			if (t[0] < t[1]) {
				t[0] = t[1];
				v[0] = v[1];
			}
		}
		else if (t[0] > t[1]) {
			t[0] = t[1];
			v[0] = v[1];
		}
		break;
	}

	info.isEnter = (pHint && pHint->pNode == this) ? fromOutSide : !IsInside(L);
	info.Cross = K * t[0] + L;
	info.Vertical = info.isEnter ? v[0] : -v[0];
	info.Distance = t[0] * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;

	return TRUE;
}

__global__
void newCone(DevNode** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevCone(root, sp(Material));
}

bool newDevCone(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newCone<<<1, 1>>>(*out, root, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}