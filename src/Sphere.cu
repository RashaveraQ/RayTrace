#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Sphere.cuh"

__device__
DevSphere::DevSphere(DevNode* const root, const char* const Name, const Sp Color)
	: DevNode(root, SPHERE, Name, Color)
{

}

__device__
bool DevSphere::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return FALSE;

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
			if (pHint && pHint->pNode == this && !fromOutSide) {
				t = (t1 < t2) ? t2 : t1;
			}
			else {
				t = (t1 < t2) ? t1 : t2;
				info.isEnter = 1;
			}
		}
		else {
			t = t1;
		}
	}
	else {
		if (t2 > 0) {
			t = t2;
		}
		else
			return FALSE;
	}

	info.Cross = K * t + L;
	info.Vertical = info.isEnter ? info.Cross : -info.Cross;
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
void newSphere(DevNode** out)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevSphere();
}

bool newDevSphere(DevNode** out)
{
	if (!mallocDev(out))
		return false;

	newSphere<<<1, 1>>>(out);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}