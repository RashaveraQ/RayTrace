#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Polygon2.cuh"

__device__
DevPolygon2::DevPolygon2(const DevPolygon2& other) : DevNode(other)
{
	int	i;

	m_N = other.m_N;
	m_P = new sp[m_N];
	for (i = 0; i < m_N; i++)
		m_P[i] = other.m_P[i];
}

__device__
bool DevPolygon2::IsInside(const sp& L) const
{
	if (L.z < 0.0)
		return false;

	bool	r = false;
	int		i, j;

	for (i = 0, j = m_N - 1; i < m_N; j = i++)
	{
		if ((
			((m_P[i].y <= L.y) && (L.y < m_P[j].y))
			||
			((m_P[j].y <= L.y) && (L.y < m_P[i].y))
			)
			&&
			(
			L.x < (m_P[j].x - m_P[i].x) * (L.y - m_P[i].y) / (m_P[j].y - m_P[i].y) + m_P[i].x
			)
			)
			r = !r;
	}
	return r;
}

__device__
bool DevPolygon2::GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	float	a = K * K;
	float	b = K * L;
	float	c = L * L - 1.0f;

	float	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return false;

	float	t;
	float	t1 = (-b + sqrt(bb_ac)) / a;
	float	t2 = (-b - sqrt(bb_ac)) / a;

	if (t1 > 0)
	{
		if (t2 > 0)
		{
			t = (t1 < t2) ? t1 : t2;
		}
		else
			t = t1;
	}
	else
	{
		if (t2 > 0)
			t = t2;
		else
			return false;
	}

	info.Cross = info.Vertical = K * t + L;
	info.Distance = t * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;

	return true;
}

__global__
void newPolygon2(DevNode** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevPolygon2(root, sp(Material));
}

bool newDevPolygon2(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newPolygon2<<<1, 1>>>(*out, root, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}