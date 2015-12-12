#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Plus.cuh"

__device__
bool DevPlus::IsInside(const Sp& L) const
{
	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->IsInside2(L))
			return TRUE;
	}

	return FALSE;
}

__device__
bool DevPlus::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	DevInfo	tmp;
	//int		n;
	float	l = -1;

	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->GetInfo2(K, L, tmp, pHint, fromOutSide)) {
			if (l == -1 || tmp.Distance < l) {
				l = tmp.Distance;
				//n = i;
				info = tmp;
			}
		}
	}

	if (l < 0)
		return false;

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

	return true;
}

__global__
void newPlus(DevNode** out, DevNode** const root, const char* const Name)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevPlus(*root, Name);
}

bool newDevPlus(DevNode** out, DevNode** const root, const char* const Name)
{
	if (!mallocDev(out))
		return false;

	newPlus<<<1, 1>>>(out, root, Name);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}