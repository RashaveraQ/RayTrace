#pragma once
#include "Node.cuh"

class DevPolygon2 : public DevNode
{
	int	m_N;
	Sp	*m_P;
public:
	// コンストラクタ
	__device__ DevPolygon2(DevNode** const root = 0, const char* const Name = "Polygon", const Sp Color = Sp(255, 255, 255))
		: DevNode(root, POLYGON, Name, Color), m_N(0), m_P(0) {}

	__device__ DevPolygon2(const DevPolygon2& other);

	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const Sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevPolygon2(*this); }
};
