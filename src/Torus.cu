#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Torus.cuh"
#include "Solve_Polynomial.cuh"

__device__
DevTorus::DevTorus(DevNode** const root, const char* const Name, const Sp Color)
	: DevNode(root, TORUS, Name, Color), m_R(0.7f), m_r(0.3f)
{

}

__device__
bool DevTorus::IsInside(const Sp& L) const
{
	float	d;

	d = m_R - sqrt(L.x * L.x + L.y * L.y);
	d *= d;

	return (m_r * m_r <= d + L.z * L.z);
}

__device__
bool DevTorus::GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const
{
	float	R2 = m_R * m_R;
	float	a = K * K;
	float	b = 2 * (K * L);
	float	c = L * L + R2 - m_r * m_r;

	float	k[20], r[50];
	int		n;

	k[4] = a * a;
	k[3] = 2 * a * b;
	k[2] = 2 * a * c + b * b - 4 * R2 * (K.x * K.x + K.y * K.y);
	k[1] = 2 * b * c - 8 * R2 * (K.x * L.x + K.y * L.y);
	k[0] = c * c - 4 * R2 * (L.x * L.x + L.y * L.y);

	n = Solve_Polynomial(4, k, 0, 20000, r);

	if (n == 0)
		return FALSE;

	if (pHint && pHint->pNode == this) {
		if (fromOutSide && n % 2 == 1) {
			if (n == 1)
				return false;
			r[0] = r[1];
		}
		if (!fromOutSide && n % 2 == 0) {
			r[0] = r[1];
		}
	}

	Sp		p;
	float	th;

	info.isEnter = IsInside(L) == TRUE ? 1 : 0;
	info.Cross = p = K * r[0] + L;

	if (p.x == 0.0) {
		info.Vertical = Sp(0, p.y - ((p.y > 0) ? 1 : -1) * m_R, p.z);
	}
	else {
		th = atan2(p.y, p.x);
		info.Vertical = Sp(p.x - m_R * cos(th), p.y - m_R * sin(th), p.z);
	}

	info.Distance = r[0] * sqrt(K * K);

	float x, y, z, phy;

	x = info.Vertical.x;
	y = info.Vertical.y;
	z = info.Vertical.z;

	th = acosf(y) / (2 * M_PI); if (x < 0) th = 1 - th;
	phy = acosf(z / sqrtf(x * x + z * z)) / (2 * M_PI);
	if (x < 0)
		phy = 1 - phy;

	info.Material = GetPixel(phy, th).getMaterial();
	info.pNode = this;

	return true;
}

__global__
void newTorus(DevNode** out, DevNode** const root, const char* const Name, const D3DMATERIAL9 Material)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		*out = new DevTorus(root, Name, Sp(Material));
}

bool newDevTorus(DevNode*** out, DevNode** const root, const char* const Name, const D3DMATERIAL9 Material)
{
	if (!mallocDev(out))
		return false;

	newTorus<<<1, 1>>>(*out, root, Name, Material);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}