#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Multiple.cuh"

__device__
bool DevMultiple::IsInside(const Sp& L) const
{
	if (!m_Member)
		return false;

	for (int i = 0; i < m_Member; i++) {
		if (!m_Node[i]->IsInside2(L))
			return false;
	}
	return true;
}

__device__
bool DevMultiple::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	if (!m_Member)
		return false;

	// すべての要素について、
	int	i;
	for (i = 0; i < m_Member; i++) {
		// 視点の先に、交点がない場合。
		if (!m_Node[i]->GetInfo2(K, L, info, pHint, fromOutSide))
			return false;

		int j;
		for (j = 0; j < m_Member; j++) {
			if (i == j)
				continue;
			if (!m_Node[j]->IsInside2(info.Cross))
				break;
		}

		if (j == m_Member)
			break;
	}

	if (i == m_Member)
		return false;

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

	return true;
}

__global__
void newMultiple(DevNode** out, DevNode** const root)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevMultiple(root);
}

bool newDevMultiple(DevNode*** out, DevNode** const root)
{
	if (!mallocDev(out))
		return false;

	newMultiple<<<1, 1>>>(*out, root);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}