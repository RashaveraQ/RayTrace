#pragma once
#include "Gathering.cuh"

class DevMultiple : public DevGathering
{
public:
	// コンストラクタ
	__device__ DevMultiple(DevNode** const root = 0) : DevGathering(root, MULTIPLE) {}
	__device__ DevMultiple(const DevMultiple& other) : DevGathering(other) {}

	__device__ virtual	bool GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevMultiple(*this); }
};
