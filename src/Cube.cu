#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Cube.cuh"

__device__
DevCube::DevCube(DevNode** const root, const sp Color)
	: DevNode(root, CUBE, Color)
{

}

__device__
bool DevCube::IsInside(const sp& L) const
{
	return (-1.0 <= L.x && L.x <= 1.0 && -1.0 <= L.y && L.y <= 1.0 && -1.0 <= L.z && L.z <= 1.0);
}

__device__
bool DevCube::GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float t[6];
	sp	   v[6];
	int i = 0;

	info.isEnter = (pHint && pHint->pNode == this) ? fromOutSide : !IsInside(L);

	t[i] = (-1 - L.x) / K.x;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(-1, 0, 0);
			i++;
		}
	}

	t[i] = (1 - L.x) / K.x;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(1, 0, 0);
			i++;
		}
	}

	t[i] = (-1 - L.y) / K.y;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.x && p.x <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(0, -1, 0);
			i++;
		}
	}

	t[i] = (1 - L.y) / K.y;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.x && p.x <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(0, 1, 0);
			i++;
		}
	}

	t[i] = (-1 - L.z) / K.z;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.x && p.x <= 1) {
			v[i] = sp(0, 0, -1);
			i++;
		}
	}

	t[i] = (1 - L.z) / K.z;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.x && p.x <= 1) {
			v[i] = sp(0, 0, 1);
			i++;
		}
	}

	switch (i) {
	case 0:
		return false;
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

	info.Cross = K * t[0] + L;
	info.Vertical = info.isEnter ? v[0] : -v[0];
	info.Distance = t[0] * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;
	return true;
}

__global__
void newCube(DevNode** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevCube(root, sp(Material));
}

bool newDevCube(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newCube<<<1, 1>>>(*out, root, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}