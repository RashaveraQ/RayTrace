#pragma once
#include "Node.cuh"

class DevPolygon2 : public DevNode
{
public:
	// コンストラクタ
	__device__ DevPolygon2(DevNode* const root = 0, const char* const Name = "Polygon2", const Sp Color = Sp(255, 255, 255));
	__device__ DevPolygon2(const DevPolygon2& other) : DevNode(other) {}

	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const Sp& L) const;
};
