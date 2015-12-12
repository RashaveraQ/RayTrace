#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Plane.cuh"

__device__
DevPlane::DevPlane(DevNode* const root, const char* const Name, const Sp Color)
	: DevNode(root, PLANE, Name, Color)
{

}

__device__
bool DevPlane::IsInside(const Sp& L) const
{
	return (L.z >= 0.0);
}

__device__
bool DevPlane::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float	t = (K.z) ? -L.z / K.z : ((L.z > 0) ? FLT_MAX : -FLT_MAX);

	if (t <= 0)
		return false;

	info.Cross = K * t + L;
	info.Vertical = Sp(0, 0, -1);
	info.Distance = t * sqrt(K * K);
	info.isEnter = (L.z < 0);
	info.Material = GetPixel(info.Cross.x, info.Cross.y).getMaterial();
	info.pNode = this;

	return true;
}

__global__
void newPlane(DevNode** out, DevNode** const root, const char* const Name, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevPlane(*root, Name, Sp(Material));
}

bool newDevPlane(DevNode** out, DevNode** const root, const char* const Name, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newPlane<<<1, 1>>>(out, root, Name, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}