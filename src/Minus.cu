#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Minus.cuh"

__device__
bool DevMinus::IsInside(const Sp& L) const
{
	return ((m_Member >= 1 && m_Node[0]->IsInside2(L)) && !(m_Member >= 2 && m_Node[1]->IsInside2(L)));
}

__device__
bool DevMinus::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	DevInfo	l_info;
	DevInfo	r_info;
	Sp		l = L;
	int		left, right;

	if (!(m_Member >= 1 && m_Node[0]->GetInfo2(K, l, l_info, pHint, fromOutSide)))
		return false;
	do {
		left = (m_Member >= 1) ? m_Node[0]->GetInfo2(K, l, l_info, pHint, fromOutSide) : 0;
		right = (m_Member >= 2) ? m_Node[1]->GetInfo2(K, l, r_info, pHint, fromOutSide) : 0;

		if (left == 0 && right == 0)
			return false;

		// 左が、右より近い場合。
		if (cmp_distance(l_info.Distance, r_info.Distance) < 0) {
			l = l_info.Cross;
			info.Material = l_info.Material;
		}
		else {
			l = r_info.Cross;
			info.Material = r_info.Material;
		}

	} while (!IsInside(m_Matrix * l));

	info.isEnter = 1;
	info.Distance = sqrt((L - l)*(L - l));
	info.Cross = l;

	if (info.Material.Diffuse.r < 0)
		info.Material = m_Material;

	return true;
}

__device__
int	DevMinus::cmp_distance(float a, float b) const
{
	if (a >= 0) {
		if (b >= 0)
			return (a < b) ? -1 : 1;
		else
			return -1;
	}
	else {
		if (b >= 0)
			return 1;
		else
			return 0;
	}
}

__global__
void newMinus(DevNode** out, DevNode** const root, const char* const Name)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevMinus(*root, Name);
}

bool newDevMinus(DevNode** out, DevNode** const root, const char* const Name)
{
	if (!mallocDev(out))
		return false;

	newMinus<<<1, 1>>>(out, root, Name);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}