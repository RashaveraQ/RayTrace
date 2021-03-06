#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Teapot.cuh"

__device__
DevTeapot::DevTeapot(DevNode** const root, const Sp Color)
	: DevNode(root, TEAPOT, Color)
{

}

__device__
bool DevTeapot::IsInside(const Sp& L) const
{
	return (sqrt(L * L) <= 1.0);
}

__device__
bool DevTeapot::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	float	a = K * K;
	float	b = K * L;
	float	c = L * L - 1.0f;

	float	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return FALSE;

	float	t;
	float	t1 = (-b + sqrt(bb_ac)) / a;
	float	t2 = (-b - sqrt(bb_ac)) / a;

	info.isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info.isEnter = 1;
		}
		else
			t = t1;
	}
	else {
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	info.Cross = info.Vertical = K * t + L;
	info.Distance = t * sqrt(K * K);

	float x, y, z, th, phy;

	x = info.Vertical.x;
	y = info.Vertical.y;
	z = info.Vertical.z;

	th = atan2f(y, sqrtf(x*x + z*z)) / M_PI + .5f;
	phy = atan2f(x, -z) / (2 * M_PI) + .5f;

	info.Material = GetPixel(phy, th).getMaterial();
	info.pNode = this;

	return TRUE;
}

__global__
void newTeapot(DevNode** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevTeapot(root, Sp(Material));
}

bool newDevTeapot(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newTeapot<<<1, 1>>>(*out, root, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}