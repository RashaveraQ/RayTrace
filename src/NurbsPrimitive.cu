#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "NurbsPrimitive.cuh"

DevNurbsPrimitive::DevNurbsPrimitive(DevNode** const root, const sp Color)
	: DevNode(root, ePLANE, Color)
{

}

bool DevNurbsPrimitive::IsInside(const sp& L) const
{
	return (L.y >= 0.0);
}

bool DevNurbsPrimitive::GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float	t = (K.y) ? -L.y / K.y : ((L.y > 0) ? FLT_MAX : -FLT_MAX);

	if (t <= 0)
		return false;

	info.Cross = K * t + L;
	info.Vertical = sp(0, 1, 0);
	info.Distance = t * sqrt(K * K);
	info.Material = GetPixel(info.Cross.x, info.Cross.z).getMaterial();
	info.pNode = this;

	return -3 < info.Cross.x && info.Cross.x < 3 && -3 < info.Cross.z && info.Cross.z < 3;
}

__global__
void newNurbsPrimitive(DevNode** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevNurbsPrimitive(root, sp(Material));
}

bool newDevNurbsPrimitive(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newNurbsPrimitive<<<1, 1>>>(*out, root, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}