#pragma once
#include "Gathering.cuh"

class DevMinus : public DevGathering
{
public:
	// コンストラクタ
	__device__ DevMinus(DevNode** const root = 0) : DevGathering(root, eMINUS) {}
	__device__ DevMinus(const DevMinus& other) : DevGathering(other) {}

	__device__ virtual	bool GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevMinus(*this); }
	__device__ int cmp_distance(float, float) const;
};
